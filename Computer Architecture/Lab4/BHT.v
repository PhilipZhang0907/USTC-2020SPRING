`timescale 1ns / 1ps

module bht
# (
    parameter BHT_ADDR_LEN = 7
)
(
    input  clk, rst,  
    input  br_ex,        
    input  [BHT_ADDR_LEN-1:0] raddr,     
    input  [BHT_ADDR_LEN-1:0] waddr,  
    output reg prediction_taken      
);

localparam BHT_SIZE     = 1 << BHT_ADDR_LEN;


reg [1:0] predictated_states   [BHT_SIZE:0]; 

//Predict whether branch or not
always@(*)
begin
    if((predictated_states[raddr] == 2'b11) || (predictated_states[raddr] == 2'b10))
        prediction_taken = 1'b1;
    else 
        prediction_taken = 1'b0;
end

always @ (posedge clk or posedge rst)
begin //write data(update BTB)
    if(rst)
    begin
        for(integer i=0; i<BHT_SIZE; i++)
        begin
                predictated_states[i]=0;
                prediction_taken = 0;
        end
    end
    else
    begin //state transitions
        if(br_ex)
            case(predictated_states[waddr])
            2'b11:
                predictated_states[waddr] = 2'b11;
            2'b10:
                predictated_states[waddr] = 2'b11;
            2'b01:
                predictated_states[waddr] = 2'b11;
            2'b00:
                predictated_states[waddr] = 2'b01;
            endcase   
        else
            case(predictated_states[waddr])
            2'b11:
                predictated_states[waddr] = 2'b10;
            2'b10:
                predictated_states[waddr] = 2'b00;
            2'b01:
                predictated_states[waddr] = 2'b00;
            2'b00:
                predictated_states[waddr] = 2'b00;
            endcase  
    end
end
endmodule
