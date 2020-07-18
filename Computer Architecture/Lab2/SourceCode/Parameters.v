`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: Parameters
// Tool Versions: Vivado 2017.4.1
// Description: Const Parameters
// 
//////////////////////////////////////////////////////////////////////////////////

// 功能说明
    // 为了代码可读性，定义了常量值
// 实验要求  
    // 无需修改

`ifndef CONST_VALUES
`define CONST_VALUES
// ALU_func[3:0]
    `define SLL  4'd0
    `define SRL  4'd1
    `define SRA  4'd2
    `define ADD  4'd3
    `define SUB  4'd4
    `define XOR  4'd5
    `define OR  4'd6
    `define AND  4'd7
    `define SLT  4'd8
    `define SLTU  4'd9
    `define LUI  4'd10
// br_type[2:0]
    `define NOBRANCH  3'd0
    `define BEQ  3'd1
    `define BNE  3'd2
    `define BLT  3'd3
    `define BLTU  3'd4
    `define BGE  3'd5
    `define BGEU  3'd6
// imm_type[2:0]
    `define RTYPE  3'd0
    `define ITYPE  3'd1
    `define STYPE  3'd2
    `define BTYPE  3'd3
    `define UTYPE  3'd4
    `define JTYPE  3'd5  
// load_type[2:0]  six kind of ways to save values to Register
    `define NOREGWRITE  3'b0	//	Do not write Register
    `define LB  3'd1			//	load 8bit from Mem then signed extended to 32bit
    `define LH  3'd2			//	load 16bit from Mem then signed extended to 32bit
    `define LW  3'd3			//	write 32bit to Register
    `define LBU  3'd4			//	load 8bit from Mem then unsigned extended to 32bit
    `define LHU  3'd5			//	load 16bit from Mem then unsigned extended to 32bit
// opcode
    // `define SHIFT_IMM 7'b0010011
    `define ALG_LOGIC       7'b0110011
    `define ALG_LOGIC_IMM   7'b0010011
    `define Inst_LUI        7'b0110111
    `define AUIPC           7'b0010111 
    `define JALR            7'b1100111 
    `define JAL             7'b1101111 
    `define BRANCH          7'b1100011 
    `define LOAD            7'b0000011  
    `define STORE           7'b0100011 
    `define CSR             7'b1110011
    //CSR funct
    `define CSRRW           3'b001
    `define CSRRS           3'b010
    `define CSRRC           3'b011
    `define CSRRWI          3'b101
    `define CSRRSI          3'b110
    `define CSRRCI          3'b111
    //CSR Params
    `define CSR_NUM         32
    `define CSR_ADDR        5
// BTB
    `define BTB_UPDATE      2'b01
    `define BTB_ADD         2'b10
    `define BTB_REMOVE      2'b11
    `define BTB_NONE        2'b00
`endif

