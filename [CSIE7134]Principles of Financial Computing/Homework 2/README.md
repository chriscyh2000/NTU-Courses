# NTU Principles of Financial Computing 2023 - Homework 2

[course website](https://www.csie.ntu.edu.tw/~lyuu/finance1.html)

### Homework Description

Write a binomial tree program to price a **Bermudan option**. The early exercise time points are $\frac{1}{4}T$ and $\frac{3}{4}T$ from now, where $T$ is the time to maturity. The payoff function is $max(K - S + 1,0)$. Inputs: (1) $S$ (stock price), (2) $K$ (strike price), (3) $r$ (annual interest rate continuously compounded), (4) $s$ (annual volatility), (5) $T$ (time to maturity in years), (6) $n$ (number of time steps). Output: Option price. For example, suppose that $S=100$, $K=100$, $r=5(\%)$, $s=30(\%)$, and $T=0.5$ (years). The option price is about $7.8052$ at $n = 100$ and $7.8015$ at $n = 200$. **IMPORTANT NOTES**: (1) The interest rate and volatility should be **in percent**. For example, if the interest rate is $5\%$ and volatility $30\%$, the inputs are $5$ and $30$, respectively. (2) No need to make sure the early exercise dates are aligned with time steps of the tree.

### Usage of `priceCalculator.py`

```shell
python3 priceCalculator.py --S [STOCK PRICE] --K [STRIKE PRICE] --r [ANNUAL INTEREST RATE] --s [ANNUAL VOLATILITY] --T [TIME TO MATURITY] --n [NUMBER OF TIME STEPS]
```

You can use `python3 getSchedule.py` to see the details about each options as follow.
```shell
usage: priceCalculator.py [-h] --S S --K K --r R --s S --T T --n N

optional arguments:
  -h, --help  show this help message and exit
  --S S       S (stock price at time t=0)
  --K K       K (strike price).
  --r R       r (annual interest rate in percent (continuously compounded))
  --s S       s (annual volatility in percent)
  --T T       T (time to maturity in years)
  --n N       n (number of time steps, n should not be smaller than 4)
```

For example, if you want to calculate the price of a Bermudan option with the inputs of (1) $S=100$, (2) $K=100$, (3) $r=5%$, (4) $s=30%$, (5) $T=0.5$ years, and (6) $n=100$, you can use the following command to obtain the result.
```shell
python3 priceCalculator.py --S 100 --K 100 --r 5 --s 30 --T 0.5 --n 100
```

### Explanation of the Logic Behind the Codes

First, let's define a few variables as follows.

| Variables | Definition                                           |
| --------- | ---------------------------------------------------- |
| $u$       | Rate of return of stock if the stock price goes up   |
| $d$       | Rate of return of stock if the stock price goes down |
| $P_u$     | probability for price going up after one period      |
| $P_d$     | probability for price going down after one period    |

Note that $P_d = 1 - P_u$ , and $u\cdot d = 1$, according to CRR binomial tree model.

We can use **backward induction** to solve the pricing problem. Based on the setting of $u\cdot d = 1$, we can reduce the number of node to $O(n^2)$ by overlapping the nodes with the same result (please refer to the graph below) instead of spanning the whole binomial tree from the root with $O(2^n)$ number of nodes. 

<img src="https://i.ibb.co/2kPK5BJ/Screenshot-2023-04-13-at-8-04-13-PM.png" alt="Screenshot 2023-04-13 at 8.04.13 PM" style="zoom:50%;" />



Starting from $t=n$, we have $n+1$ states, and we can use the equation below to calculate the option price of each state at the previous time step until we derive the option price at $t=0$:
$$
\begin{align*}
C = \frac{P_u \cdot C_u + P_d \cdot C_d}{e^{r\frac{T}{n}}}
\end{align*}
$$
Here, $C$ is the option price of a state at the previous time step $t-1$, and $C_u$ and $C_d$ are the option prices if the stock price goes up and down at time step $t$, respectively.

Note that if $t=n$, the equation should be modified as follows, where $max(K - S_t + 1,0)$ is the payoff if the option is executed at time step $t$:
$$
\begin{align*}
C = max(K - S_n + 1,0)
\end{align*}
$$
Also, since we can execute the option at time steps $\frac{1}{4}T$ and $\frac{3}{4}T$, the option price at these two time steps should be modified as follows:
$$
\begin{align*}
C = max(max(K - S_n + 1,0), \frac{P_u \cdot C_u + P_d \cdot C_d}{e^{r\frac{T}{n}}})
\end{align*}
$$
In the implementation of the algorithm, I only used two $O(n)$ arrays in total to record the option prices at each time step during the process of backward induction. And since we have $O(n^2)$ states, and the price in each state takes only constant time to calculate, the time complexity of the algorithm is $O(n^2)$.
