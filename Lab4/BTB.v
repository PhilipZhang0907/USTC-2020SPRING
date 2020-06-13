`timescale 1ns / 1ps
`include "Parameters.v"

module Branch_Target_Buffer
# (
    parameter BUFFER_ADDR_LEN = 7
)
(
    input clk, rst,
    output reg hit,                 // whether entry of PCF exists
    input [31:0] raddr,             // PCF
    output reg [31:0] rd_data,      // predicted target
    input [1:0] opr_type,           // BTB_update
    input [31:0] waddr,             // update addr
    input [31:0] wr_data
);
    localparam BUFFER_SIZE = 1 << BUFFER_ADDR_LEN;
    reg [31:              0] predicted_PC [BUFFER_SIZE];
    reg [31:BUFFER_ADDR_LEN] BTB_Tags     [BUFFER_SIZE];
    reg valid  [BUFFER_SIZE];

    wire [BUFFER_ADDR_LEN-1:0] read_BTB_addr;
    wire [BUFFER_ADDR_LEN-1:0] write_BTB_addr;
    wire [31: BUFFER_ADDR_LEN] read_TAG_addr;
    wire [31: BUFFER_ADDR_LEN] write_TAG_addr;

    assign {read_TAG_addr, read_BTB_addr} = raddr;
    assign {write_TAG_addr, write_BTB_addr} = waddr;

    always@(*)
    begin
        if(valid[read_BTB_addr] == 1'b1 && BTB_Tags[read_BTB_addr] == read_TAG_addr)
        begin
            rd_data = predicted_PC[read_BTB_addr];
            hit = 1'b1;
        end
        else
        begin
            rd_data = raddr + 4;
            hit = 1'b0;
        end
    end

    integer i;
    always@(posedge clk or posedge rst)
    begin
        if(rst)
        begin
            for(i = 0; i < BUFFER_SIZE; i = i + 1)
            begin
                BTB_Tags[i] = 0;
                valid[i] = 0;
                predicted_PC[i] = 0;
            end
            hit = 0;
            rd_data = 0;
        end
        else
        begin
            case(opr_type)
                `BTB_UPDATE:
                begin
                    predicted_PC[write_BTB_addr] = wr_data;
                end
                `BTB_ADD:
                begin
                    predicted_PC[write_BTB_addr] = wr_data;
                    BTB_Tags[write_BTB_addr] = write_TAG_addr;
                    valid[write_BTB_addr] = 1'b1;
                end
                `BTB_REMOVE:
                begin
                    valid[write_BTB_addr] = 1'b0;
                end
            endcase
        end
    end
endmodule

