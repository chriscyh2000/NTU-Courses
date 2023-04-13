# NTU Principles of Financial Computing 2023 - Homework 1

[course website](https://www.csie.ntu.edu.tw/~lyuu/finance1.html)

### Homework Description

Write a program to generate an amortization schedule for repaying a loan. There are two interest rates. Inputs: (1) $L$ (loan amount in dollars), (2) $r_1$ (annual interest rate in percent for the first $n_1$ periods, so $12$ instead of $0.12$), (3) $r_2$ (annual interest rate in percent for the remaining periods), (4) $n_1$ (number of periods when the interest rate is $r_1$), (5) $n$ (duration of the loan in years), and (6) $m$ (the number of payments per annum). Output: A csv file for the amortization schedule and the total interest paid. The schedule shall have five columns: (1) Time (0, 1, 2, ...), (2) The level payment amount, (3) Interest (the interest part of each payment), (4) Principal (the principal part of each payment), and (5) Remaining principal. For example, if $L = 10,000,000$, $r_1 = 8\%$, $r_2 = 3\%$, $n_1 = 120$, $n = 20$, and $m = 12$, the example output file is [here](https://drive.google.com/file/d/1FZG36kg5W6TFRLlKKl2aokMXBm8_pwzx/view?usp=share_link) and the total interest is $8593339.37$. **IMPORTANT** notes: (1) The **Time** column must be integers (no floating-point numbers). (2) The **Payment**, **Interest**, **Principal**, and **Remaining principal** columns must be floating-point numbers up to 2 decimal points. (3) The order of the columns must be respected. (4) The headers of the columns must be as in the sample file. (5) Start from Time 0 instead of Time 1. This means the value of the first row will be 0, 0, 0, 0, $L$. 

### Usage of `getSchedule.py`

```shell
python3 getSchedule.py [-h] --L L --r1 R1 --r2 R2 --n1 N1 --n N --m M [--output_file OUTPUT_FILE]
```

You can use `python3 getSchedule.py -h` to see the details about each options as follow.
```shell
optional arguments:
  -h, --help            show this help message and exit
  --L L                 L (loan amount in dollars)
  --r1 R1               r1 (annual interest rate in percent for the first n1 periods, so 12 instead of 0.12)
  --r2 R2               r2 (annual interest rate in percent for the remaining periods)
  --n1 N1               n1 (number of periods when the interest rate is r1)
  --n N                 n (duration of the loan in years)
  --m M                 m (the number of payments per annum)
  --output_file OUTPUT_FILE
                        the name of the amortization schedule file
```

For example, if we want to get the amortization schedule when $L=10,000,000$, $r_1=8\%$, $r_2=3\%$, $n_1=120$, $n=20$ and $m=12$, and you want to name the output csv file as `amortization.csv`, you can use the following command:
```shell
python3 getSchedule.py --L 10000000 --r1 8 --r2 3 --n1 120 --n 20 --m 12 --output_file amortization.csv
```

### Explanation of the Logic Behind the Codes

From the equation for calculating the present value of a general annuities, we know that for a loan with interest rate $r_1$ in the first $n_1$ periods and $r_2$ in the remaining periods, we have:
$$
L = \Sigma_{t=1}^{n_1}C\frac{1}{(1+\frac{r_1}{m})^{t}}+\Sigma_{t=n_1+1}^{mn}C\frac{1}{(1+\frac{r_1}{m})^{n_1}}\cdot\frac{1}{(1+\frac{r_2}{m})^t}
$$
while $n$ is the duration of the loan in years, $m$ is the number of payments per annum and $C$ is the payment in every period.

We can rewrite the above equation to get the payment $C$:
$$
C=L / (\frac{1-(1+\frac{r_1}{m})^{-n_1}}{\frac{r_1}{m}}+\frac{1}{(1+\frac{r_1}{m})^{n_1}}\cdot\frac{1-(1+\frac{r_2}{m})^{-(mn-n_1)}}{\frac{r_2}{m}})
$$
Besides, we know that:

- A month's interest payment = (previous month's remaining principal) $\times$ (monthly interest rate)
- A month's principal payment = (monthly payment) $-$ (interest payment in this month)
- A month's remaining principal = (previous month's remaining principal) $-$ (principal payment in this month)

So we can combine all the information above to construct the program to calculate the amortization schedule for repaying a loan.
