.globl __start

.rodata
  space: .string " "    

.text

__start:
    # Read first operand
    li a0, 5
    ecall
    mv x9, a0          # x9: input size(n)
    
    beq x9, x0, exit

    li a0, 9
    slli t1, x9, 2     # allocate a x9 * 4 bytes memory and assign the addr to a0 
    add a1, x0, t1
    ecall 
    
    addi t2, x0, 0     # t2: input reading counter
    addi x18, a0, 0    # x18: array address

jal x1, read_arr
addi x12, x0, 0
jal x1, print_inorder
addi sp, sp, 16
beq x0, x0, exit

read_arr:
    blt t2, x9, read_loop
    jalr x0, 0(x1)

read_loop: 
    li a0, 5
    ecall
    mv t0, a0           # t0: value of arr[R[t2]]
    
    slli t1, t2, 2      # t1: t2 << 2
    add t3, x18, t1     # t3: memory address of arr[R[t2]]
    sw t0, 0(t3)
    addi t2, t2, 1
    beq x0, x0, read_arr

print_inorder:
    addi sp, sp, -16
    sw x1, 8(sp)        # x1: return address
    sw x12, 0(sp)       # x12: function param (i)    
    blt x12, x9, L1
    jalr x0, 0(x1)
    
L1:
    slli x12, x12, 1
    addi x12, x12, 1  
    jal x1, print_inorder

    addi sp, sp, 16
    
    lw x12, 0(sp)
    slli t1, x12, 2     # t1: x12 << 2
    add t3, x18, t1
    lw s3, 0(t3)
    jal x1, output
    jal x1, output_space

    slli x12, x12, 1
    addi x12, x12, 2  

    jal x1, print_inorder
    addi sp, sp, 16
    lw x12, 0(sp)
    lw x1, 8(sp)
    
    jalr x0, 0(x1)

output:
    # Output the result
    li a0, 1
    mv a1, s3
    ecall
    jalr x0, 0(x1)

output_space:
    li a0, 4
    la a1, space
    ecall
    jalr x0, 0(x1)

exit:
    # Exit program(necessary)
    li a0, 10
    ecall
