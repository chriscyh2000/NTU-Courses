.globl __start

.rodata
    division_by_zero: .string "division by zero"

.text
__start:
    # Read first operand
    li a0, 5
    ecall
    mv s0, a0
    # Read operation
    li a0, 5
    ecall
    mv s1, a0
    # Read second operand
    li a0, 5
    ecall
    mv s2, a0

###################################
#  TODO: Develop your calculator  #
#                                 #
###################################

# addiction
add s3, s0, s2
beq s1, x0, output  # if(s1 == 0): output result0

# substraction
li t1, 1
sub s3, s0, s2
beq s1, t1, output   # if(s1 == 1): output result1

# multiplication
li t1, 2
mul s3, s0, s2
beq s1, t1, output   # if(s1 == 2): output result2

# division
li t1, 3
beq s1, t1, division # if(s1 == 3): perform division

# minimum
li t1, 4
beq s1, t1, minimum

# power
li t1, 5
beq s1, t1, power

# factorial
li t1, 6
beq s1, t1, factorial

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

division:
    beq s2, x0, division_by_zero_except # if(s2 == 0): exception
    div s3, s0, s2                      # output quotient
    beq x0, x0, output

division_by_zero_except:
    li a0, 4
    la a1, division_by_zero
    ecall
    jal zero, exit

minimum:
    add s3, x0, s0
    blt s0, s2, output
    add s3, x0, s2
    beq x0, x0, output
  
power:
    li t2, 0
    li s3, 1
    beq x0, x0, power_loop
    
power_loop:
    beq s2, t2, output
    addi t2, t2, 1
    mul s3, s3, s0
    beq x0, x0, power_loop

factorial:
    li t2, 0
    li s3, 1
    beq x0, x0, factorial_loop
    
factorial_loop:
    bge t2, s0, output
    addi t2, t2, 1
    mul s3, s3, t2
    beq x0, x0, factorial_loop