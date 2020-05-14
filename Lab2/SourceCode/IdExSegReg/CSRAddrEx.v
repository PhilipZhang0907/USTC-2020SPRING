`timescale 1ns / 1ps
// author @ Philip-Chang

module CsrAddrEx(
    input wire clk, bubbleE, flushE,
    input wire [2:0] func3_in,
    output reg [2:0] func3_out,
    input wire [4:0] zimm_in,
    output reg [4:0] zimm_out,
    input wire [11:0] csr_addr_in,
    output reg [11:0] csr_addr_out,
    input wire csr_write_en_in,
    output reg csr_write_en_out,
    input wire is_csr_in,
    output reg is_csr_out
    ); 
    //ID EX stage reg
    always@(posedge clk)
        if (!bubbleE) 
        begin
            if(flushE)
                begin
                csr_addr_out <= 0;
                func3_out <= 0;
                zimm_out <= 0;
                csr_write_en_out <= 1'b0;
                is_csr_out <= 1'b0;
                end
            else 
                begin
                csr_addr_out <= csr_addr_in;
                func3_out <= func3_in;
                zimm_out <= zimm_in;
                csr_write_en_out <= csr_write_en_in;
                is_csr_out <= is_csr_in;
                end
        end
endmodule