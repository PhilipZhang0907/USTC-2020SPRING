`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/05/20 23:05:26
// Design Name: 
// Module Name: btb
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

`include "Parameters.v" 
module branch_target_buffer
# (
    parameter BUFFER_ADDR_LEN = 7
)
(
    input  clk, rst,
    output reg hit,         //whether entry of raddr(PCF) exists
    input  [31:0] raddr,        //PCF
    output reg [31:0] rd_data,  //Predicted target     
    input  [1:0]  operation_type,          //BTB_Update type
    input  [31:0] waddr,        //PCE(Update addr)
    input  [31:0] wr_data       //BrcPC
);
    localparam BUFFER_SIZE = 1 << BUFFER_ADDR_LEN;
    reg [31:0               ] predicted_pc  [BUFFER_SIZE:0];
    reg [31:BUFFER_ADDR_LEN ] btb_tags      [BUFFER_SIZE:0];
    reg valid   [BUFFER_SIZE:0];

    wire [BUFFER_ADDR_LEN-1:0   ] read_btb_addr;
    wire [31:BUFFER_ADDR_LEN    ] read_tag_addr;
    wire [BUFFER_ADDR_LEN-1:0   ] write_btb_addr;
    wire [31:BUFFER_ADDR_LEN    ] write_tag_addr;

    assign {read_tag_addr, read_btb_addr} = raddr;   
    assign {write_tag_addr, write_btb_addr} = waddr;  

    always@(*)
    begin
        if(valid[read_btb_addr] == 1'b1 && (btb_tags[read_btb_addr] == read_tag_addr))
        begin
            rd_data = predicted_pc[read_btb_addr];  
            hit = 1'b1;
        end
        else 
        begin
            rd_data = raddr+4;
            hit = 1'b0;
        end
    end  
    
    integer i;
    always @ (posedge clk or posedge rst) 
    begin //(update BTB)
        if(rst) 
        begin
            for(i=0; i<BUFFER_SIZE; i= i+1) 
            begin
                    btb_tags[i]=0;
                    valid[i]=0;
                    predicted_pc[i]=0;
                    hit = 0;
                    rd_data = 0;
            end
        end
        else
            case(operation_type)
            `BTB_UPDATE:  begin   //need to update branch target 
                    predicted_pc[write_btb_addr] = wr_data;
                    end
            `BTB_ADD:  begin   //need to add entry
                    predicted_pc[write_btb_addr] = wr_data;
                    btb_tags[write_btb_addr] = write_tag_addr;
                    valid[write_btb_addr] = 1'b1;
                    end
            `BTB_REMOVE:  begin   //need to remove entry(invalidate entry)
                    valid[write_btb_addr] = 1'b0;
                    end   
            //2'b00 do nothing
            endcase
    end
endmodule

