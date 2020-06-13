`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: NPC Generator
// Tool Versions: Vivado 2017.4.1
// Description: RV32I Next PC Generator
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    //  根据跳转信号，决定执行的下一条指令地�??
    //  debug端口用于simulation时批量写入数据，可以忽略
// 输入
    // PC                指令地址（PC + 4, 而非PC�??
    // jal_target        jal跳转地址
    // jalr_target       jalr跳转地址
    // br_target         br跳转地址
    // jal               jal == 1时，有jal跳转
    // jalr              jalr == 1时，有jalr跳转
    // br                br == 1时，有br跳转
// 输出
    // NPC               下一条执行的指令地址
// 实验要求  
    // 实现NPC_Generator

module NPC_Generator(
    //input wire clk,
    input wire [31:0] PC, jal_target, jalr_target, br_target,
    input wire jal, jalr, br,
    output reg [31:0] NPC,
    //altered for lab4
    input wire btb_if,btb_ex,if_prediction_true,
    input wire [31:0]prediction_if,PC_EX,
    //lab4 phase2
    input wire bht_if,bht_ex
    );

    // TODO: Complete this module
    //assign NPC = (jal==1 ? jal_target : (jalr==1 ? jalr_target : (br==1 ? br_target : PC+2)));
    // PC is connected to PC_4
    //assign NPC <= (jal==1 ? jal_target : (jalr==1 ? jalr_target : (br==1 ? br_target: PC+4));
    //always@(negedge clk)
        //NPC = (jal==1 ? jal_target : (jalr==1 ? jalr_target : (br==1 ? br_target : PC+4)));
    always @(*)
    begin
        if(jalr)
            NPC <= jalr_target;
        else if(br)
        //altered for lab 4 
            begin
                if(if_prediction_true)
                    NPC <= PC;
                else
                    NPC <= br_target;
            end
        else if(~br && btb_ex && bht_ex) //branch not taken but wrongly branched 
            begin
            NPC <= PC_EX;
            end
        else if(jal)
            NPC <= jal_target;
        else
        //altered for lab 4 
            begin
            if(btb_if && bht_if)
                NPC <= prediction_if;
            else
                NPC <= PC;
            end
    end
endmodule