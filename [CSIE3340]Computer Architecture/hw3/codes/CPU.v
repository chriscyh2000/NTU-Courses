module CPU
(
    clk_i, 
    rst_i,
    start_i
);

// Ports
input               clk_i;
input               rst_i;
input               start_i;

parameter FOUR = 4;
wire [31:0]         pc_in;
wire [31:0]         pc_out;
wire [31:0]         instr;
wire [1:0]          ALUOp;
wire                ALUSrc;
wire                RegWrite;   
wire [31:0]         imm; 
wire [31:0]         RS1data; 
wire [31:0]         RS2data; 
wire [31:0]         ALU_data1; 
wire [31:0]         ALU_data2; 
wire [31:0]         ALU_result;
wire [2:0]          ALUCtrl;

Control Control(
    .Op_i       (instr[6:0]),
    .ALUOp_o    (ALUOp),
    .ALUSrc_o   (ALUSrc),
    .RegWrite_o (RegWrite)
);



Adder Add_PC(
    .data1_in   (pc_out),
    .data2_in   (FOUR),
    .data_o     (pc_in)
);


PC PC(
    .clk_i      (clk_i),
    .rst_i      (rst_i),
    .start_i    (start_i),
    .pc_i       (pc_in),
    .pc_o       (pc_out)
);

Instruction_Memory Instruction_Memory(
    .addr_i     (pc_out), 
    .instr_o    (instr)
);

Registers Registers(
    .clk_i       (clk_i),
    .RS1addr_i   (instr[19:15]),
    .RS2addr_i   (instr[24:20]),
    .RDaddr_i    (instr[11:7]), 
    .RDdata_i    (ALU_result),
    .RegWrite_i  (RegWrite), 
    .RS1data_o   (ALU_data1), 
    .RS2data_o   (RS2data) 
);


MUX32 MUX_ALUSrc(
    .data1_i    (RS2data),
    .data2_i    (imm),
    .select_i   (ALUSrc),
    .data_o     (ALU_data2)
);



Sign_Extend Sign_Extend(
    .data_i     (instr[31:20]),
    .data_o     (imm)
);
  


ALU ALU(
    .data1_i    (ALU_data1),
    .data2_i    (ALU_data2),
    .ALUCtrl_i  (ALUCtrl),
    .data_o     (ALU_result),
    .Zero_o     ()
);



ALU_Control ALU_Control(
    .funct_i    ({instr[31:25], instr[14:12]}),
    .ALUOp_i    (ALUOp),
    .ALUCtrl_o  (ALUCtrl)
);


endmodule