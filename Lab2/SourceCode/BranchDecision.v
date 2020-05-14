`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: Branch Decision
// Tool Versions: Vivado 2017.4.1
// Description: Decide whether to branch
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    //  判断是否branch
// 输入
    // reg1               寄存器1
    // reg2               寄存器2
    // br_type            branch类型
// 输出
    // br                 是否branch
// 实验要求
    // 补全模块

`include "Parameters.v"   
module BranchDecision(
    input wire [31:0] reg1, reg2,
    input wire [2:0] br_type,
    output reg br
    );

    wire signed [31:0] reg1Signed = $signed(reg1);
    wire signed [31:0] reg2Signed = $signed(reg2);

    always@(*)
    begin
        case(br_type)
        `NOBRANCH: br <= 0;
        `BEQ:
        begin
            if(reg1 == reg2)
                br <= 1'b1;
            else br <= 1'b0;
        end
        `BNE:
        begin
            if(reg1 != reg2)
                br <= 1'b1;
            else br <= 1'b0;
        end
        `BLT:
        begin
            if(reg1Signed < reg2Signed)
                br <= 1'b1;
            else br <= 1'b0;
        end
        `BLTU:
        begin
            if(reg1 < reg2)
                br <= 1'b1;
            else br <= 1'b0;
        end
        `BGE:
        begin
            if(reg1Signed >= reg2Signed)
                br <= 1'b1;
            else br <= 1'b0;
        end
        `BGEU:
        begin
            if(reg1 >= reg2)
                br <= 1'b1;
            else br <= 1'b0;
        end
        endcase
    end
    // TODO: Complete this module

endmodule
