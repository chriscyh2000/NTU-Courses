"""
This python script is used to price a Bermudan option (the early exercise time points are T/4 and 3T/4 from now) by binomial tree. 
"""

from argparse import Namespace, ArgumentParser
from math import exp, sqrt

def main(args: Namespace):
    S, K, r, s, T, n = args.S, args.K, args.r / 100, args.s / 100, args.T, args.n
    
    period = T / n
    u = exp(s * sqrt(period)) # rate of return if the stock price goes up
    d = 1 / u                 # rate of return if the stock price goes down
    R = exp(r * period)       # R = e^(r * period) (risk-free rate of return for one period)
    Pu = (R - d) / (u - d)    # probability for price going up after one period
    Pd = 1 - Pu               # probability for price going down after one period
    
    payoff = [0 for _ in range(n + 2)]      # "+2" is just for easy implementation
    tmp_payoff = [0 for _ in range(n + 1)]  # "+1" is just for easy implementation
    
    for num_priod in range(n, -1, -1):
        num_state = num_priod + 1
        if num_priod == n // 4 or num_priod == 3 * n // 4 or num_priod == n:
            if (num_state & 1):
                stock_price = S
                for i in range(num_state >> 1, -1, -1):
                    tmp_payoff[i] = max(K - stock_price + 1, 0)
                    stock_price *= (u * u)
                stock_price = S
                for i in range(num_state >> 1, num_state):
                    tmp_payoff[i] = max(K - stock_price + 1, 0)
                    stock_price *= (d * d)
            else:
                stock_price = S * u
                for i in range((num_state >> 1) - 1, -1, -1):
                    tmp_payoff[i] = max(K - stock_price + 1, 0)
                    stock_price *= (u * u)
                stock_price = S * d
                for i in range(num_state >> 1, num_state):
                    tmp_payoff[i] = max(K - stock_price + 1, 0)
                    stock_price *= (d * d)
                
            for i in range(num_state):
                tmp_payoff[i] = max(tmp_payoff[i], (Pu * payoff[i] + Pd * payoff[i+1]) / R)
                
        else:
            for i in range(num_state):
                tmp_payoff[i] = (Pu * payoff[i] + Pd * payoff[i+1]) / R
                
        payoff, tmp_payoff = tmp_payoff, payoff
        
    print(f"Option Price = {payoff[0]:.4f}")
    
def parse_args() -> Namespace:
    parser = ArgumentParser()
    
    parser.add_argument(
        "--S",
        type=float,
        required=True,
        help="S (stock price at time t=0)"
    )
    parser.add_argument(
        "--K",
        type=float,
        required=True,
        help="K (strike price)."
    )
    parser.add_argument(
        "--r",
        type=float,
        required=True,
        help="r (annual interest rate in percent (continuously compounded))"
    )
    parser.add_argument(
        "--s",
        type=float,
        required=True,
        help="s (annual volatility in percent)"
    )
    parser.add_argument(
        "--T",
        type=float,
        required=True,
        help="T (time to maturity in years)"
    )
    parser.add_argument(
        "--n",
        type=int,
        required=True,
        help="n (number of time steps, n should not be smaller than 4)",
    )
    
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_args()
    main(args)