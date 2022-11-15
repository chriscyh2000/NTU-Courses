module Adder(
    input  wire [31:0] data1_in,
    input  wire [31:0] data2_in,
    output wire [31:0] data_o
);
    assign data_o = data1_in + data2_in;
endmodule