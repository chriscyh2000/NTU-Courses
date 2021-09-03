`ifndef ADDERS
`define ADDERS
`include "gates.v"

// half adder, gate level modeling
module HA(output C, S, input A, B);
	XOR g0(S, A, B);
	AND g1(C, A, B);
endmodule

// full adder, gate level modeling
module FA(output CO, S, input A, B, CI);
	wire c0, s0, c1, s1;
	HA ha0(c0, s0, A, B);
	HA ha1(c1, s1, s0, CI);
	assign S = s1;
	OR or0(CO, c0, c1);
endmodule

// adder without delay, register-transfer level modeling
module adder_rtl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);
	assign {C3, S} = A+B+C0;
endmodule

//  ripple-carry adder, gate level modeling
//  Do not modify the input/output of module
module rca_gl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);
	// TODO:: Implement gate-level RCA
	FA fa0(C1, S[0], A[0], B[0], C0);
	FA fa1(C2, S[1], A[1], B[1], C1);
	FA fa2(C3, S[2], A[2], B[2], C2);
endmodule

// carry-lookahead adder, gate level modeling
// Do not modify the input/output of module
module cla_gl(
	output C3,       // carry output
	output[2:0] S,   // sum
	input[2:0] A, B, // operands
	input C0         // carry input
	);

	// TODO:: Implement gate-level CLA
	wire[2:0] p;
	wire[2:0] g;
	AND and0(g[0], A[0], B[0]);
	OR or0(p[0], A[0], B[0]);
	AND and1(g[1], A[1], B[1]);
	OR or1(p[1], A[1], B[1]);
	AND and2(g[2], A[2], B[2]);
	OR or2(p[2], A[2], B[2]);
	// C1
	AND g1(aa1, p[0], C0);
	OR g2(C1, g[0], aa1);
	// C2
	AND4 g3(bb1, C0, p[0], p[1], 1'b1);
	AND g4(bb2, g[0], p[1]);
	OR4 g5(C2, g[1], bb1, bb2, 1'b0);
	// C3
	AND4 g6(cc1, C0, p[0], p[1], p[2]);
	AND4 g7(cc2, g[0], p[1], p[2], 1'b1);
	AND g8(cc3, g[1], p[2]);
	OR4 g9(C3, g[2], cc1, cc2, cc3);
	FA fa1(eDay, S[0], A[0], B[0], C0);
	FA fa2(eDay_eDay, S[1], A[1], B[1], C1);
	FA fa3(eDay_eDay_eDay, S[2], A[2], B[1], C2);
endmodule

`endif
