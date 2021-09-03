`timescale 1ns/1ps
`include "gates.v"
`include "adders.v"

module lab1_main();
	reg[2:0] a, b;
	reg c0;
	wire[2:0] s, s_gl; 
	wire c3, c3_gl;
	adder_rtl adder(c3, s, a, b, c0);

	// TODO:: Try to connect the module into our test bench

	rca_gl eDay(c3_gl, s_gl, a, b, c0);
	//cla_gl eDay_eDay(c3_gl, s_gl, a, b, c0);
	

	// track the maximum delays
	integer delay, max_delay=0, time_max;
	always @(c3_gl, s_gl) begin
		delay = $time%100;
		if(delay>max_delay) begin
			max_delay = delay;
			// if i-->j, time_max = {i, j, 1}
			// if j-->i, time_max = {i, j, 0}
			time_max = $time/100;
		end
	end
	
	// simulate
	integer i, j, time0, time1;
	initial begin
		
		// dump files for GTKWave to read
		$dumpfile("lab1.vcd");
		$dumpvars(0, lab1_main);
		
		// print a line if the listed variables change value
		// $display("   time    a     b   c0  {c3 s}   gl");
		// $monitor("%7d / %b / %b / %b / %b%b / %b%b",
		//  	 $time, a, b, c0, c3, s, c3_gl, s_gl);
		
		// loop through all possible transitions
		{a, b, c0} <= 0;
		for(i=0; i<128; i=i+1) begin
			for(j=0; j<128; j=j+1) begin
				// $display("======================================");
				// $display("%7d / %b / %b / %b / %b%b / %b%b",
				//   	 $time, a, b, c0, c3, s, c3_gl, s_gl);
				{a, b, c0} <= i;
				#100;
				{a, b, c0} <= j;
				#100;
			end
		end
		
		// retrieve the inputs when maximum delay occur
		if(time_max%2==1) begin // i-->j
			time0 = (time_max/2)/128;
			time1 = (time_max/2)%128;
		end
		else begin              // j-->i
			time0 = (time_max/2-1)%128;
			time1 = (time_max/2)/128;
		end
		$write("The maximum delay is %2d ticks on transition", max_delay);
		a = time0/16; b = (time0%16)/2; c0 = time0%2;
		$write(" %b+%b+%b", a, b, c0);
		a = time1/16; b = (time1%16)/2; c0 = time1%2;
		$write(" --> %b+%b+%b\n", a, b, c0);
		$finish;
	end
endmodule
