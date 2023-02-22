echo "greedy" >> q3_result.txt
python q3.py --device $1 --greedy --pred_file ./pred/pred_report.jsonl
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt

echo "beam_search 3" >> q3_result.txt
python q3.py --device $1 --beam_search --generation_param 3 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt
echo "beam_search 7" >> q3_result.txt
python q3.py --device $1 --beam_search --generation_param 7 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt

echo "top_k 5" >> q3_result.txt
python q3.py --device $1 --top_k --generation_param 5 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt
echo "top_k 10" >> q3_result.txt
python q3.py --device $1 --top_k --generation_param 10 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt

echo "top_p 0.4" >> q3_result.txt
python q3.py --device $1 --top_p --generation_param 0.4 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt
echo "top_p 0.8" >> q3_result.txt
python q3.py --device $1 --top_p --generation_param 0.8 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt

echo "temperature 0.8" >> q3_result.txt
python q3.py --device $1 --temperature --generation_param 0.8 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt
echo "temperature 1.2" >> q3_result.txt
python q3.py --device $1 --temperature --generation_param 1.2 --pred_file ./pred/pred_report.jsonl 
python eval.py -r ./data/public.jsonl -s ./pred/pred_report.jsonl >> q3_result.txt