module ALU (
    input  wire [31:0]  data1_i,
    input  wire [31:0]  data2_i,
    input  wire [2:0]   ALUCtrl_i,
    output wire [31:0]  data_o,
    output wire         Zero_o
);
    `define AND  3'b000
    `define XOR  3'b001
    `define SLL  3'b010
    `define ADD  3'b011
    `define SUB  3'b100
    `define MUL  3'b101
    `define ADDI 3'b110
    `define SRAI 3'b111

    assign data_o = ((ALUCtrl_i == `AND) ? data1_i & data2_i : 
                     (ALUCtrl_i == `XOR) ? data1_i ^ data2_i :
                     (ALUCtrl_i == `SLL) ? data1_i << data2_i :
                     (ALUCtrl_i == `ADD) ? data1_i + data2_i :
                     (ALUCtrl_i == `SUB) ? data1_i - data2_i :
                     (ALUCtrl_i == `MUL) ? data1_i * data2_i :
                     (ALUCtrl_i == `ADDI) ? data1_i + data2_i : data1_i >>> data2_i[4:0]);
    assign Zero_o = ((data1_i - data2_i == 0) ? 1 : 0);
endmodule