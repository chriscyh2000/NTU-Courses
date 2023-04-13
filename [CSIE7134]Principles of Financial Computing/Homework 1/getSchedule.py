import pandas as pd
from pathlib import Path
from argparse import Namespace, ArgumentParser


def main(args):
    L, r1, r2, n1, n, m = args.L, args.r1 / 100, args.r2 / 100, args.n1, args.n, args.m
    
    payment = L / ((1 - ((1 + r1 / m) ** (-n1))) / (r1 / m) + (1 - ((1 + r2 / m) ** (-m * n + n1))) / (r2 / m) * ((1 + r1 / m) ** -n1))
    data = {
        "Time": [t for t in range(m * n + 1)],
        "Payment": [0] + [round(payment, 2) for _ in range(1, m * n + 1)],
        "Interest": [0],
        "Principal": [0],
        "Remaining principal": [L]
    }
    
    for t in range(1, m * n + 1):
        interest = L * ((r1 if t <= n1 else r2) / m)
        principal = payment - interest
        L -= principal
        
        data["Interest"].append(round(interest, 2))
        data["Principal"].append(round(principal, 2))
        data["Remaining principal"].append(round(abs(L), 2))
        
    pd.DataFrame(data).to_csv(args.output_file, index=False)
    print(f"Total interest paid: {round(sum(data['Interest']), 2)}")
    
def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--L",
        type=float,
        required=True,
        help="L (loan amount in dollars)"
    )
    parser.add_argument(
        "--r1",
        type=float,
        required=True,
        help="r1 (annual interest rate in percent for the first n1 periods, so 12 instead of 0.12)"
    )
    parser.add_argument(
        "--r2",
        type=float,
        required=True,
        help="r2 (annual interest rate in percent for the remaining periods)"
    )
    parser.add_argument(
        "--n1",
        type=int,
        required=True,
        help="n1 (number of periods when the interest rate is r1)"
    )
    parser.add_argument(
        "--n",
        type=int,
        required=True,
        help=" n (duration of the loan in years)",
    )
    parser.add_argument(
        "--m",
        type=int,
        required=True,
        help="m (the number of payments per annum)"
    )
    parser.add_argument(
        "--output_file",
        type=Path,
        default="./amortization.csv",
        help="the name of the amortization schedule file"
    )
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_args()
    main(args)