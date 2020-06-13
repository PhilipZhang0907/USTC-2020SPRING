// IF/ID stage
module BTB_ID_SEG(
    input clk, bubbleD, flushD,
    input btb_if,
    input [31:0] predict_if,
    output reg btb_id,
    output reg [31:0] predict_id 
);
    initial
    begin
        btb_id <= 0;
        predict_id <= 0;
        always@(posedge clk)
        begin
            if(!bubbleD)
            begin
                if(flushD)
                begin
                    btb_id <= 1'b0;
                    predict_id <= 32'b0;
                end
                else
                begin
                    btb_id <= btb_if;
                    predict_id <= predict_if;
                end
            end  
        end
    end
endmodule

module BHT_ID_SEG(
    input clk, bubbleD, flushD,
    input bht_if,
    input [31:0] final_predict_if,
    output reg bht_id,
    output reg [31:0] final_predict_id
);
    initial 
    begin
        bht_id <= 0;
        final_predict_id <= 0;
    end
        always@(posedge clk)
        if (!bubbleD) 
        begin
            if(flushD)
                begin
                    bht_id <= 1'b0;
                    final_predict_id <= 32'b0;
                end
            else 
                begin
                    bht_id <= bht_if;
                    final_predict_id <= final_predict_if;
                end
        end
endmodule

// ID/EX stage
module BTB_EX_SEG(
    input clk, bubbleE, flushE,
    input btb_id;
    input [31:0] predict_id;
    output reg btb_ex;
    output reg [31:0] predict_ex;
);
    initial 
    begin
        btb_ex <= 0;
        predict_ex <= 0;
    end
        always@(posedge clk)
        if (!bubbleE) 
        begin
            if(flushE)
                begin
                    btb_ex <= 1'b0;
                    predict_ex <= 32'b0;
                end
            else 
                begin
                    btb_ex <= btb_id;
                    predict_ex <= predict_id;
                end
        end
endmodule

module BHT_EX_SEG(
    input clk, bubbleE, flushE,
    input bht_id,
    input [31:0] final_predict_id,
    output reg bht_ex,
    output reg [31:0] final_predict_ex
);
    initial 
    begin
        bht_ex <= 0;
        final_predict_ex <= 0;
    end
        always@(posedge clk)
        if (!bubbleE) 
        begin
            if(flushE)
                begin
                    bht_ex <= 1'b0;
                    final_predict_ex <= 32'b0;
                end
            else 
                begin
                    bht_ex <= bht_id;
                    final_predict_ex <= final_predict_id;
                end
        end
endmodule