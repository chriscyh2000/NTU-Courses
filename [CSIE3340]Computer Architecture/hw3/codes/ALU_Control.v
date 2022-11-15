module ALU_Control(
    input  wire [9:0] funct_i,
    input  wire [1:0] ALUOp_i,
    output wire [2:0] ALUCtrl_o
);
    `define AND  3'b000
    `define XOR  3'b001
    `define SLL  3'b010
    `define ADD  3'b011
    `define SUB  3'b100
    `define MUL  3'b101
    `define ADDI 3'b110
    `define SRAI 3'b111

    assign ALUCtrl_o = ((ALUOp_i == 2'b00) ? (
                        (funct_i == 10'b0000000111) ? `AND : 
                        (funct_i == 10'b0000000100) ? `XOR :
                        (funct_i == 10'b0000000001) ? `SLL :
                        (funct_i == 10'b0000000000) ? `ADD :
                        (funct_i == 10'b0100000000) ? `SUB : `MUL) : 
                        ((funct_i == 10'b0100000101) ? `SRAI : `ADDI));
endmodule