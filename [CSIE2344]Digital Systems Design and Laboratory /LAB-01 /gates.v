`ifndef GATES
`define GATES

module NOT(output Y, input A);
	not #1 g0(Y, A);
endmodule

module NAND(output Y, input A, B);
	nand #2 g0(Y, A, B);
endmodule

module AND(output Y, input A, B);
	and #3 g0(Y, A, B);
endmodule

module NOR(output Y, input A, B);
	nor #2 g0(Y, A, B);
endmodule

module OR(output Y, input A, B);
	or #3 g0(Y, A, B);
endmodule

module XOR(output Y, input A, B);
	xor #5 g0(Y, A, B);
endmodule

module AND4(output Y, input A, B, C, D);
	wire w1, w2;
	AND g0(w1, A, B);
	AND g1(w2, C, D);
	AND g2(Y, w1, w2);
endmodule

module OR4(output Y, input A, B, C, D);
	wire w1, w2;
	OR g0(w1, A, B);
	OR g1(w2, C, D);
	OR g2(Y, w1, w2);
endmodule

`endif