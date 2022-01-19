import subprocess
from pathlib import Path
import csv

test_cases = [
    ["1", "8", "323"]
]

sample_outputs = [
    "1 3\n2 3\n3 1\n4 1\n5 3\n6 3\n7 3\n8 3\n"
]

def run_command(command, timeout=60, combine_err=True):
    if combine_err:
        completed_process = subprocess.run(
            command,
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            timeout=timeout,
        )
        return completed_process.stdout
    else:
        completed_process = subprocess.run(
            command,
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout,
        )
        return completed_process.stdout, completed_process.stderr


def judge_one_case(chiffon_args, sample_output, timeout):
    run_command("rm -f ./target/*.tmp")
    print(f"$ bash chiffon.sh -m {chiffon_args[0]} -n {chiffon_args[1]} -l {chiffon_args[2]}")
    stdout, stderr = run_command(
        f"bash chiffon.sh -m {chiffon_args[0]} -n {chiffon_args[1]} -l {chiffon_args[2]}",
        timeout=timeout,
        combine_err=False,
    )
    print("stdout ---")
    print(stdout.decode())
    print("stderr ---")
    print(stderr.decode())

    return (stdout.decode() == sample_output)

if __name__ == "__main__":

    run_command("rm -f ./host ./player")

    print("$ make")
    output = run_command("make")
    print(output.decode())

    if Path("./host").is_file() and Path("./player").is_file():
        print("--- ./host and ./player exist")
    else:
        print("--- ./host or ./player doesn't exist ---")
        exit(1)

    timeout = 20
    for i, case in enumerate(test_cases):
        AC = False
        try:
            AC = judge_one_case(case, sample_outputs[i], timeout)
        except subprocess.TimeoutExpired:
            print("--- TLE")
        if AC:
            print("--- Yeah Boi (AC)")
        else:
            print("--- QQQ (WA)")

    run_command("rm -f ./host ./player")
