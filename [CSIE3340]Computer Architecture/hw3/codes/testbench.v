`define CYCLE_TIME 50            

module TestBench;

reg                Clk;
reg                Reset;
reg                Start;
integer            i, outfile, counter;

always #(`CYCLE_TIME/2) Clk = ~Clk;    

CPU CPU(
    .clk_i  (Clk),
    .rst_i  (Reset),
    .start_i(Start)
);
  
initial begin
    counter = 0;
    
    // initialize instruction memory
    for(i=0; i<256; i=i+1) begin
        CPU.Instruction_Memory.memory[i] = 32'b0;
    end
    
        
    // initialize Register File
    for(i=0; i<32; i=i+1) begin
        CPU.Registers.register[i] = 32'b0;
    end
    
    // Load instructions into instruction memory
    $readmemb("instruction.txt", CPU.Instruction_Memory.memory);
    
    // Open output file
    outfile = $fopen("output.txt") | 1;
    
    Clk = 0;
    Reset = 0;
    Start = 0;
    
    #(`CYCLE_TIME/4) 
    Reset = 1;
    Start = 1;
        
    
end
  
always@(posedge Clk) begin
    if(counter == 30)    // stop after 30 cycles
        $finish;
        
    // print PC
    $fdisplay(outfile, "PC = %d", CPU.PC.pc_o);
    
    // print Registers
    $fdisplay(outfile, "Registers");
    $fdisplay(outfile, "x0     = %d, x8(s0)  = %d, x16(a6) = %d, x24(s8)  = %d", CPU.Registers.register[0], CPU.Registers.register[8] , CPU.Registers.register[16], CPU.Registers.register[24]);
    $fdisplay(outfile, "x1(ra) = %d, x9(s1)  = %d, x17(a7) = %d, x25(s9)  = %d", CPU.Registers.register[1], CPU.Registers.register[9] , CPU.Registers.register[17], CPU.Registers.register[25]);
    $fdisplay(outfile, "x2(sp) = %d, x10(a0) = %d, x18(s2) = %d, x26(s10) = %d", CPU.Registers.register[2], CPU.Registers.register[10], CPU.Registers.register[18], CPU.Registers.register[26]);
    $fdisplay(outfile, "x3(gp) = %d, x11(a1) = %d, x19(s3) = %d, x27(s11) = %d", CPU.Registers.register[3], CPU.Registers.register[11], CPU.Registers.register[19], CPU.Registers.register[27]);
    $fdisplay(outfile, "x4(tp) = %d, x12(a2) = %d, x20(s4) = %d, x28(t3)  = %d", CPU.Registers.register[4], CPU.Registers.register[12], CPU.Registers.register[20], CPU.Registers.register[28]);
    $fdisplay(outfile, "x5(t0) = %d, x13(a3) = %d, x21(s5) = %d, x29(t4)  = %d", CPU.Registers.register[5], CPU.Registers.register[13], CPU.Registers.register[21], CPU.Registers.register[29]);
    $fdisplay(outfile, "x6(t1) = %d, x14(a4) = %d, x22(s6) = %d, x30(t5)  = %d", CPU.Registers.register[6], CPU.Registers.register[14], CPU.Registers.register[22], CPU.Registers.register[30]);
    $fdisplay(outfile, "x7(t2) = %d, x15(a5) = %d, x23(s7) = %d, x31(t6)  = %d", CPU.Registers.register[7], CPU.Registers.register[15], CPU.Registers.register[23], CPU.Registers.register[31]);
    
    $fdisplay(outfile, "\n");
    
    counter = counter + 1;
end

  
endmodule
