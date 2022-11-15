.globl __start

.text
__start:
    # input n
    li a0, 5
    ecall
    mv s0, a0

jal x1, fib
addi sp, sp, 24
addi s3, x10, 0
beq x0, x0, output

fib:
    addi sp, sp, -24
    sw x1, 16(sp)
    sw x10, 8(sp)
    sw s0, 0(sp)
    addi t0, s0, -1
    bgt t0, x0, L1
    addi x10, s0, 0
    jalr x0, 0(x1)
L1:
    addi s0, s0, -1
    jal x1, fib
    addi sp, sp, 24
    lw s0, 0(sp)
    addi s0, s0, -2
    jal x1, fib
    lw t0, 8(sp)
    addi sp, sp, 24
    lw x1, 16(sp)
    add x10, x10, t0
    jalr x0, 0(x1)

output:
    # Output the result
    li a0, 1
    mv a1, s3
    ecall
    beq x0, x0, exit

exit:
    # Exit program(necessary)
    li a0, 10
    ecall
