`timescale 1ns / 1ps
`include "Parameters.v"

module ControlAndStatusRegister(
    inout wire clk,
    inout wire rst,
    input wire is_csr,
    input wire [31:0]reg1,
    input wire [4:0]zimm,
    input wire [2:0]func3,
    input wire [11:0]read_reg_addr,
    output reg [31:0]read_reg_data,
    input wire csr_write_en 
);
 
    reg [`CSR_NUM-1:0] control_status_register_file [31:0];
    
    integer i;
    initial
    begin
        for(i = 0; i < `CSR_NUM; i = i + 1) 
            control_status_register_file[i][31:0] <= 32'b0;
    end

    always@(posedge clk or posedge rst) 
    begin
        if(rst)
            read_reg_data <= 32'b0;
        else
            read_reg_data <= control_status_register_file[read_reg_addr];
    end

    always@(negedge clk or posedge rst) 
    begin 
        if (rst)
            for (i = 0; i < 32; i = i + 1) 
                control_status_register_file[i][31:0] <= 32'b0;
        else if(csr_write_en)
            begin
                case(func3)
                    `CSRRW:  control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= reg1;   
                    `CSRRS:  control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= reg1 | control_status_register_file[read_reg_addr]; // set
                    `CSRRC:  control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= (~reg1) & control_status_register_file[read_reg_addr]; //clear
                    `CSRRWI: control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= {27'b0,zimm};
                    `CSRRSI: control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= {27'b0,zimm} | control_status_register_file[read_reg_addr];
                    `CSRRCI: control_status_register_file[read_reg_addr[`CSR_ADDR-1:0]] <= (~{27'b0,zimm}) & control_status_register_file[read_reg_addr];
                endcase
            end
    end  
endmodule