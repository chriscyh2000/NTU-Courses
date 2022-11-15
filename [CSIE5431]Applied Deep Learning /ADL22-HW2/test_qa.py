import argparse
import os
import pandas as pd
from pathlib import Path

import datasets
import numpy as np
import torch
from datasets import load_dataset
from torch.utils.data import DataLoader
from tqdm.auto import tqdm

import transformers
from accelerate import Accelerator
from accelerate.logging import get_logger
from accelerate.utils import set_seed
from transformers import (
    MODEL_MAPPING,
    AutoConfig,
    AutoModelForQuestionAnswering,
    AutoTokenizer,
    DataCollatorWithPadding,
    EvalPrediction,
    default_data_collator,
)
from utils_qa import postprocess_qa_predictions

logger = get_logger(__name__)
# You should update this to your particular problem to have better documentation of `model_type`
MODEL_CONFIG_CLASSES = list(MODEL_MAPPING.keys())
MODEL_TYPES = tuple(conf.model_type for conf in MODEL_CONFIG_CLASSES)


def parse_args():
    parser = argparse.ArgumentParser(description="Finetune a transformers model on a Question Answering task")
    parser.add_argument(
        "--preprocessing_num_workers", type=int, default=1, help="A csv or a json file containing the training data."
    )
    parser.add_argument(
        "--test_file", type=str, default="./processed_data/qa/test.json", help="A csv or a json file containing the Prediction data."
    )
    parser.add_argument(
        "--tokenizer_name_or_path",
        type=str,
        help="Directory to the pretrained tokenizer.",
        default="./ckpt/pretrained/macbert/",
    )
    parser.add_argument(
        "--max_seq_length",
        type=int,
        default=512,
        help=(
            "The maximum total input sequence length after tokenization. Sequences longer than this will be truncated,"
            " sequences shorter will be padded if `--pad_to_max_lengh` is passed."
        ),
    )
    parser.add_argument(
        "--pad_to_max_length",
        action="store_true",
        help="If passed, pad all samples to `max_seq_length`. Otherwise, dynamic padding is used.",
    )
    parser.add_argument(
        "--model_save_dir",
        type=str,
        help="Path to finetuned model.",
        default="./ckpt/trained/qa",
        required=False,
    )
    parser.add_argument("--seed", type=int, default=None, help="A seed for reproducible training.")
    parser.add_argument(
        "--doc_stride",
        type=int,
        default=128,
        help="When splitting up a long document into chunks how much stride to take between chunks.",
    )
    parser.add_argument(
        "--n_best_size",
        type=int,
        default=20,
        help="The total number of n-best predictions to generate when looking for an answer.",
    )
    parser.add_argument(
        "--null_score_diff_threshold",
        type=float,
        default=0.0,
        help=(
            "The threshold used to select the null answer: if the best answer has a score that is less than "
            "the score of the null answer minus this threshold, the null answer is selected for this example. "
            "Only useful when `version_2_with_negative=True`."
        ),
    )
    parser.add_argument(
        "--version_2_with_negative",
        action="store_true",
        help="If true, some of the examples do not have an answer.",
    )
    parser.add_argument(
        "--max_answer_length",
        type=int,
        default=64,
        help=(
            "The maximum length of an answer that can be generated. This is needed because the start "
            "and end predictions are not conditioned on one another."
        ),
    )
    parser.add_argument(
        "--overwrite_cache", action="store_true", help="Overwrite the cached training and evaluation sets"
    )
    parser.add_argument(
        "--model_type",
        type=str,
        default=None,
        help="Model type to use if training from scratch.",
        choices=MODEL_TYPES,
    )
    # pred
    parser.add_argument("--pred_file", type=Path, default="./pred/pred.csv")
    args = parser.parse_args()

    args.output_dir = args.pred_file.parent

    # Sanity checks
    if args.test_file is None:
        raise ValueError("Need either a dataset name or a training/validation/test file.")
    else:
        if args.test_file is not None:
            extension = args.test_file.split(".")[-1]
            assert extension in ["csv", "json"], "`test_file` should be a csv or a json file."

    return args


def main():
    args = parse_args()
    args.pred_file.parent.resolve().mkdir(parents=True, exist_ok=True)

    # Initialize the accelerator. We will let the accelerator handle device placement for us in this example.
    # If we're using tracking, we also need to initialize it here and it will by default pick up all supported trackers
    # in the environment
    accelerator_log_kwargs = {}

    accelerator = Accelerator(**accelerator_log_kwargs)

    if accelerator.is_local_main_process:
        datasets.utils.logging.set_verbosity_warning()
        transformers.utils.logging.set_verbosity_info()
    else:
        datasets.utils.logging.set_verbosity_error()
        transformers.utils.logging.set_verbosity_error()

    # If passed along, set the training seed now.
    if args.seed is not None:
        set_seed(args.seed)

    accelerator.wait_for_everyone()
    
    data_files = {}
    data_files["test"] = args.test_file
    extension = args.test_file.split(".")[-1]
    raw_datasets = load_dataset(extension, data_files=data_files, field="data")

    config = AutoConfig.from_pretrained(args.model_save_dir)
    tokenizer = AutoTokenizer.from_pretrained(args.tokenizer_name_or_path, use_fast=True)
    model = AutoModelForQuestionAnswering.from_pretrained(
            args.model_save_dir,
            from_tf=bool(".ckpt" in args.model_save_dir),
            config=config,
        )
    
    # Preprocessing the datasets.
    # Preprocessing is slighlty different for training and evaluation.
    column_names = raw_datasets["test"].column_names
    question_column_name = "question" if "question" in column_names else column_names[0]
    context_column_name = "context" if "context" in column_names else column_names[1]
    answer_column_name = "answers" if "answers" in column_names else column_names[2]

    # Padding side determines if we do (question|context) or (context|question).
    pad_on_right = tokenizer.padding_side == "right"
  
    if args.max_seq_length > tokenizer.model_max_length:
        logger.warning(
            f"The max_seq_length passed ({args.max_seq_length}) is larger than the maximum length for the"
            f"model ({tokenizer.model_max_length}). Using max_seq_length={tokenizer.model_max_length}."
        )

    max_seq_length = min(args.max_seq_length, tokenizer.model_max_length)

   
    # Validation preprocessing
    def prepare_validation_features(examples):
        # Some of the questions have lots of whitespace on the left, which is not useful and will make the
        # truncation of the context fail (the tokenized question will take a lots of space). So we remove that
        # left whitespace
        examples[question_column_name] = [q.lstrip() for q in examples[question_column_name]]

        # Tokenize our examples with truncation and maybe padding, but keep the overflows using a stride. This results
        # in one example possible giving several features when a context is long, each of those features having a
        # context that overlaps a bit the context of the previous feature.
        tokenized_examples = tokenizer(
            examples[question_column_name if pad_on_right else context_column_name],
            examples[context_column_name if pad_on_right else question_column_name],
            truncation="only_second" if pad_on_right else "only_first",
            max_length=max_seq_length,
            stride=args.doc_stride,
            return_overflowing_tokens=True,
            return_offsets_mapping=True,
            padding="max_length" if args.pad_to_max_length else False,
        )

        # Since one example might give us several features if it has a long context, we need a map from a feature to
        # its corresponding example. This key gives us just that.
        sample_mapping = tokenized_examples.pop("overflow_to_sample_mapping")

        # For evaluation, we will need to convert our predictions to substrings of the context, so we keep the
        # corresponding example_id and we will store the offset mappings.
        tokenized_examples["example_id"] = []

        for i in range(len(tokenized_examples["input_ids"])):
            # Grab the sequence corresponding to that example (to know what is the context and what is the question).
            sequence_ids = tokenized_examples.sequence_ids(i)
            context_index = 1 if pad_on_right else 0

            # One example can give several spans, this is the index of the example containing this span of text.
            sample_index = sample_mapping[i]
            tokenized_examples["example_id"].append(examples["id"][sample_index])

            # Set to None the offset_mapping that are not part of the context so it's easy to determine if a token
            # position is part of the context or not.
            tokenized_examples["offset_mapping"][i] = [
                (o if sequence_ids[k] == context_index else None)
                for k, o in enumerate(tokenized_examples["offset_mapping"][i])
            ]

        return tokenized_examples
    
    predict_examples = raw_datasets["test"]
    # Predict Feature Creation
    with accelerator.main_process_first():
        predict_dataset = predict_examples.map(
            prepare_validation_features,
            batched=True,
            num_proc=args.preprocessing_num_workers,
            remove_columns=column_names,
            load_from_cache_file=not args.overwrite_cache,
            desc="Running tokenizer on prediction dataset",
        )
    
    # DataLoaders creation:
    if args.pad_to_max_length:
        # If padding was already done ot max length, we use the default data collator that will just convert everything
        # to tensors.
        data_collator = default_data_collator
    else:
        # Otherwise, `DataCollatorWithPadding` will apply dynamic padding for us (by padding to the maximum length of
        # the samples passed). When using mixed precision, we add `pad_to_multiple_of=8` to pad all tensors to multiple
        # of 8s, which will enable the use of Tensor Cores on NVIDIA hardware with compute capability >= 7.5 (Volta).
        data_collator = DataCollatorWithPadding(tokenizer, pad_to_multiple_of=(8 if accelerator.use_fp16 else None))

   
    predict_dataset_for_model = predict_dataset.remove_columns(["example_id", "offset_mapping"])
    predict_dataloader = DataLoader(
        predict_dataset_for_model, collate_fn=data_collator, batch_size=1
    )

    model, predict_dataloader= accelerator.prepare(model, predict_dataloader)
    
    # Post-processing:
    def post_processing_function(examples, features, predictions):
        # Post-processing: we match the start logits and end logits to answers in the original context.
        predictions = postprocess_qa_predictions(
            examples=examples,
            features=features,
            predictions=predictions,
            version_2_with_negative=args.version_2_with_negative,
            n_best_size=args.n_best_size,
            max_answer_length=args.max_answer_length,
            null_score_diff_threshold=args.null_score_diff_threshold,
        )
        # Format the result to the format the metric expects.
        if args.version_2_with_negative:
            formatted_predictions = [
                {"id": k, "prediction_text": v, "no_answer_probability": 0.0} for k, v in predictions.items()
            ]
        else:
            formatted_predictions = [{"id": k, "prediction_text": v} for k, v in predictions.items()]

        references = [{"id": ex["id"], "answers": ex[answer_column_name]} for ex in examples]
        return EvalPrediction(predictions=formatted_predictions, label_ids=references)


    # Create and fill numpy array of size len_of_validation_data * max_length_of_output_tensor
    def create_and_fill_np_array(start_or_end_logits, dataset, max_len):
        """
        Create and fill numpy array of size len_of_validation_data * max_length_of_output_tensor
        Args:
            start_or_end_logits(:obj:`tensor`):
                This is the output predictions of the model. We can only enter either start or end logits.
            eval_dataset: Evaluation dataset
            max_len(:obj:`int`):
                The maximum length of the output tensor. ( See the model.eval() part for more details )
        """

        step = 0
        # create a numpy array and fill it with -100.
        logits_concat = np.full((len(dataset), max_len), -100, dtype=np.float64)
        # Now since we have create an array now we will populate it with the outputs gathered using accelerator.gather_for_metrics
        for output_logit in start_or_end_logits:  # populate columns
            # We have to fill it such that we have to take the whole tensor and replace it on the newly created array
            # And after every iteration we have to change the step

            batch_size = output_logit.shape[0]
            cols = output_logit.shape[1]

            if step + batch_size < len(dataset):
                logits_concat[step : step + batch_size, :cols] = output_logit
            else:
                logits_concat[step:, :cols] = output_logit[: len(dataset) - step]

            step += batch_size

        return logits_concat

    # Prediction 
    logger.info("***** Running Prediction *****")
    logger.info(f"  Num examples = {len(predict_dataset)}")
    logger.info(f"  Batch size = 1")

    all_start_logits = []
    all_end_logits = []

    model.eval()

    for batch in tqdm(predict_dataloader):
        with torch.no_grad():
            outputs = model(**batch)
            start_logits = outputs.start_logits
            end_logits = outputs.end_logits

            if not args.pad_to_max_length:  # necessary to pad predictions and labels for being gathered
                start_logits = accelerator.pad_across_processes(start_logits, dim=1, pad_index=-100)
                end_logits = accelerator.pad_across_processes(end_logits, dim=1, pad_index=-100)

            all_start_logits.append(accelerator.gather_for_metrics(start_logits).cpu().numpy())
            all_end_logits.append(accelerator.gather_for_metrics(end_logits).cpu().numpy())

    max_len = max([x.shape[1] for x in all_start_logits])  # Get the max_length of the tensor
    # concatenate the numpy array
    start_logits_concat = create_and_fill_np_array(all_start_logits, predict_dataset, max_len)
    end_logits_concat = create_and_fill_np_array(all_end_logits, predict_dataset, max_len)

    # delete the list of numpy arrays
    del all_start_logits
    del all_end_logits

    outputs_numpy = (start_logits_concat, end_logits_concat)
    prediction = post_processing_function(predict_examples, predict_dataset, outputs_numpy)

    ids = []
    answers = []
    for pred in prediction[0]:
        ids.append(pred['id'])
        answers.append(pred['prediction_text'])
    df = pd.DataFrame({'id': ids, 'answer': answers})
    df.to_csv(args.pred_file, index=False)

if __name__ == "__main__":
    main()