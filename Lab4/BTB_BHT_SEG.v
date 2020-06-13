`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/05/21 02:02:44
// Design Name: 
// Module Name: branch_prediction_pipeline_reg
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

//IF ID stage
module btb_id_seg(
input clk,bubbleD,flushD,
input btb_if,
input [31:0] prediction_if,
output reg btb_id,
output reg [31:0] prediction_id
    );
    initial 
    begin
    btb_id <= 0;
    prediction_id <= 0;
    end
        always@(posedge clk)
        if (!bubbleD) 
        begin
            if(flushD)
                begin
                btb_id <= 1'b0;
                prediction_id <= 32'b0;
                end
            else 
                begin
                btb_id <= btb_if;
                prediction_id <= prediction_if;
                end
        end
endmodule

//ID EX stage
module btb_ex_seg(
input clk,bubbleE,flushE,
input btb_id,
input [31:0] prediction_id,
output reg btb_ex,
output reg [31:0] prediction_ex
    );
    
    initial 
    begin
    btb_ex <= 0;
    prediction_ex <= 0;
    end
        always@(posedge clk)
        if (!bubbleE) 
        begin
            if(flushE)
                begin
                btb_ex <= 1'b0;
                prediction_ex <= 32'b0;
                end
            else 
                begin
                btb_ex <= btb_id;
                prediction_ex <= prediction_id;
                end
        end
endmodule

module bht_ex_seg(
input clk,bubbleE,flushE,
input bht_id,
input [31:0] final_prediction_id,
output reg bht_ex,
output reg [31:0] final_prediction_ex
    );
    
    initial 
    begin
    bht_ex <= 0;
    final_prediction_ex <= 0;
    end
        always@(posedge clk)
        if (!bubbleE) 
        begin
            if(flushE)
                begin
                bht_ex <= 1'b0;
                final_prediction_ex <= 32'b0;
                end
            else 
                begin
                bht_ex <= bht_id;
                final_prediction_ex <= final_prediction_id;
                end
        end
endmodule

module bht_id_seg(
input clk,bubbleD,flushD,
input bht_if,
input [31:0] final_prediction_if,
output reg bht_id,
output reg [31:0] final_prediction_id
    );
    
    initial 
    begin
    bht_id <= 0;
    final_prediction_id <= 0;
    end
        always@(posedge clk)
        if (!bubbleD) 
        begin
            if(flushD)
                begin
                bht_id <= 1'b0;
                final_prediction_id <= 32'b0;
                end
            else 
                begin
                bht_id <= bht_if;
                final_prediction_id <= final_prediction_if;
                end
        end
endmodule