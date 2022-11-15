module Sign_Extend(
    input  wire [11:0] data_i,
    output wire [31:0] data_o
);
    assign data_o[31:12] = {20{data_i[11]}};
    assign data_o[11:0] = data_i;
endmodule