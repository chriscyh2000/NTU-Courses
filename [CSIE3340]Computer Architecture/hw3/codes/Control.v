module Control(
    input  wire [6:0] Op_i,
    output wire [1:0] ALUOp_o,
    output wire       ALUSrc_o,
    output wire       RegWrite_o
);
    assign ALUOp_o = ((Op_i == 7'b0110011) ? 2'b00 : 2'b01);
    assign ALUSrc_o = ((Op_i == 7'b0010011) ? 1'b1 : 1'b0);
    assign RegWrite_o = 1'b1;
endmodule