`timescale 1ns / 1ps

module cpu_tb();
    reg clk = 1'b1;
    reg rst = 1'b1;
    
    always  #2 clk = ~clk;
    initial #8 rst = 1'b0;
    wire miss;
    wire debug_cache_hit;
    wire [31:0]total_count;
    integer miss_count = 0;
    
    always@(posedge miss)
        begin
            miss_count = miss_count + 1;
        end

    RV32ICore RV32ICore_tb_inst(
        .CPU_CLK    ( clk          ),
        .CPU_RST    ( rst          ),
        .debug_miss  (miss),
        .debug_cache_hit(debug_cache_hit),
        .total_count(total_count)
    );
    
endmodule
