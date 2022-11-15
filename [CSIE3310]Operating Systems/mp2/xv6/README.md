# NTU OS 2022 -  MP2

## About the Tests

There are 5 test programs for this assignment, respectively named `mp2_N` with `N = 1, ..., 5`. The source code can be found at `user/mp2_N.c`.


- `mp2_1`

  `vmprint()` test.

- `mp2_2`

  `sbrk()` test and page fault test.

- `mp2_3`

  `madvise()` test with `MADV_NORMAL` and `MADV_DONTNEED` options.

- `mp2_4`

  `madvise()` test with `MADV_WILLNEED` options.

- `mp2_4`

  Page fault test on swapped page.

## Run Tests

Run this command to launch all test programs at once. The stdout will be saved to `mp2_N.out` files.

```sh
./run_mp2.py
```

Example output files can be found in `mp2_output/` directory.

## Run Tests Manually

To enter the xv6 shell on QEMU,

```sh
make clean
make qemu
```

Then, you can run `mp2_N` programs manually.


To escape from the QEMU, open another terminal and run:

```sh
killall qemu-system-riscv64
```
