`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: Controller Decoder
// Tool Versions: Vivado 2017.4.1
// Description: Controller Decoder Module
// 
//////////////////////////////////////////////////////////////////////////////////

//  功能说明
    //  对指令进行译码，将其翻译成控制信号，传输给各个部件
// 输入
    // Inst              待译码指令
// 输出
    // jal               jal跳转指令
    // jalr              jalr跳转指令
    // op2_src           ALU的第二个操作数来源。为1时，op2选择imm，为0时，op2选择reg2
    // ALU_func          ALU执行的运算类型
    // br_type           branch的判断条件，可以是不进行branch
    // load_npc          写回寄存器的值的来源（PC或者ALU计算结果）, load_npc == 1时选择PC
    // wb_select         写回寄存器的值的来源（Cache内容或者ALU计算结果），wb_select == 1时选择cache内容
    // load_type         load类型
    // src_reg_en        指令中src reg的地址是否有效，src_reg_en[1] == 1表示reg1被使用到了，src_reg_en[0]==1表示reg2被使用到了
    // reg_write_en      通用寄存器写使能，reg_write_en == 1表示需要写回reg
    // cache_write_en    按字节写入data cache
    // imm_type          指令中立即数类型
    // alu_src1          alu操作数1来源，alu_src1 == 0表示来自reg1，alu_src1 == 1表示来自PC
    // alu_src2          alu操作数2来源，alu_src2 == 2’b00表示来自reg2，alu_src2 == 2'b01表示来自reg2地址，alu_src2 == 2'b10表示来自立即数
// 实验要求
    // 补全模块


`include "Parameters.v"   
module ControllerDecoder(
    input wire [31:0] inst,
    output wire jal,                // done
    output wire jalr,               // done
    output wire op2_src,            // done
    output reg [3:0] ALU_func,
    output reg [2:0] br_type,       // done
    output wire load_npc,           // done
    output wire wb_select,          // done
    output reg [2:0] load_type,
    output reg [1:0] src_reg_en,    // done
    output reg reg_write_en,        // done
    output reg [3:0] cache_write_en,
    output wire alu_src1,           // done
    output wire [1:0] alu_src2,     // done
    output reg [2:0] imm_type
    );
    wire [6:0]func7;
    wire [3:0]func3;
    wire [6:0]opcode;

    assign func7 = inst[31:25];
    assign func3 = inst[14:12];
    assign opcode = inst[6:0];

    assign load_npc = jal | jalr;
    assign jal = (opcode == `JAL) ? 1'b1 : 1'b0;
    assign jalr = (opcode == `JALR) ? 1'b1 : 1'b0;
    assign wb_select = (opcode == `LOAD) ? 1'b1 : 1'b0;

    assign alu_src1 = (opcode == `AUIPC) ? 1'b1 : 1'b0;
    assign alu_src2 = ((opcode == `ALG_LOGIC_IMM) && (func3[1:0] == 2'b01)) ? (2'b01) : (((opcode == `ALG_LOGIC) || (opcode == `BRANCH)) ? 2'b00 : 2'b10);

    always@(*)
    begin
        case(opcode)
        `ALG_LOGIC:     reg_write_en <= 1'b1;
        `ALG_LOGIC_IMM: reg_write_en <= 1'b1;
        `LUI:           reg_write_en <= 1'b1;
        `AUIPC:         reg_write_en <= 1'b1;
        `JALR:          reg_write_en <= 1'b1;
        `JAL:           reg_write_en <= 1'b1;
        `BRANCH:        reg_write_en <= 1'b0;
        `LOAD:          reg_write_en <= 1'b1;
        `STORE:         reg_write_en <= 1'b0;
        //TODO: CSR
        `CSR:           reg_write_en <= 1'b1;
        endcase
    end

    always@(*)
    begin
        case(opcode)
        `ALG_LOGIC:     op2_src <= 1'b0;
        `ALG_LOGIC_IMM: op2_src <= 1'b1;
        `LUI:           op2_src <= 1'b1;
        `AUIPC:         op2_src <= 1'b1;
        `JALR:          op2_src <= 1'b1;
        `JAL:           op2_src <= 1'b1;
        `BRANCH:        op2_src <= 1'b1;
        `LOAD:          op2_src <= 1'b1;
        `STORE:         op2_src <= 1'b1;
        //TODO: CSR
        `CSR:           op2_src <= 1'b1;
        endcase
    end

    always@(*)
    begin
        case(imm_type)
        `RTYPE:         src_reg_en <= 2'b11;
        `ITYPE:         src_reg_en <= 2'b10;
        `STYPE:         src_reg_en <= 2'b11;
        `BTYPE:         src_reg_en <= 2'b11;
        `UTYPE:         src_reg_en <= 2'b00;
        `JTYPE:         src_reg_en <= 2'b00;
        default:        src_reg_en <= 2'b00;                                      
        endcase
    end

    always@(*)
    begin
        if(opcode == `BRANCH)
        begin   
            case(func3)
            3'b000:     br_type <= `BEQ;    //BEQ
            3'b001:     br_type <= `BNE;    //BNE
            3'b100:     br_type <= `BLT;    //BLT
            3'b101:     br_type <= `BGE;    //BGE 
            3'b110:     br_type <= `BLTU;   //BLTU
            3'b111:     br_type <= `BGEU;
            default:    br_type <= `BGEU;   //BGEU 3'b111                                                      
            endcase
        end
        else            br_type <= `NOBRANCH;
    end

    always@(*)
    case(opcode)
        `CSR:
        begin
            imm_type        <= `ITYPE;
            load_type       <= `LW;
            ALU_func        <= `ADD;
            cache_write_en  <= 4'b0000;
        end
        `ALG_LOGIC_IMM:
        begin   
            load_type       <= `LW;
            cache_write_en  <= 4'b0000;
            imm_type        <= `ITYPE;
            case(func3)
                3'b000:     ALU_func <= `ADD;  //ADDI
                3'b001:     ALU_func <= `SLL;  //SLLI
                3'b010:     ALU_func <= `SLT;  //SLTI
                3'b011:     ALU_func <= `SLTU; //SLTIU
                3'b100:     ALU_func <= `XOR;  //XORI
                3'b101:
                begin
                    if(func7[5] == 1)
                            ALU_func<=`SRA;    //SRAI
                    else
                            ALU_func<=`SRL;    //SRLI
                end
                3'b110:     ALU_func<=`OR;     //ORI
                default:    ALU_func<=`AND;    //ANDI                                                 
            endcase
        end
        `ALG_LOGIC:
        begin    
            load_type       <= `LW;
            cache_write_en  <= 4'b0000;
            imm_type        <= `RTYPE;
            case(func3)
                3'b000:
                begin
                    if(func7[5] == 1)
                        ALU_func <= `SUB;   //SUB
                    else
                        ALU_func <= `ADD;   //ADD
                end
                3'b001: ALU_func <= `SLL;   //SLL
                3'b010: ALU_func <= `SLT;   //SLT
                3'b011: ALU_func <= `SLTU;  //SLTU
                3'b100: ALU_func <= `XOR;   //XOR
                3'b101:
                begin
                    if(func7[5] == 1)
                        ALU_func <= `SRA;   //SRA
                    else
                        ALU_func <= `SRL;   //SRL
                end  
                3'b110: ALU_func <= `OR;    //OR
                default:ALU_func <= `AND;   //AND                                      
            endcase
        end
        `LOAD:
        begin
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ADD;
            imm_type        <= `ITYPE;
            case(func3)
                3'b000: load_type <= `LB;     //LB
                3'b001: load_type <= `LH;     //LH
                3'b010: load_type <= `LW;     //LW
                3'b100: load_type <= `LBU;    //LBU
                3'b101: load_type <= `LHU;    //LHU  3'b101                                                            
            endcase
        end
        `STORE:
        begin   
            load_type   <= `NOREGWRITE;
            ALU_func    <= `ADD;
            imm_type    <= `STYPE; 
            case(func3)
                3'b000: cache_write_en <= 4'b0001;    //SB
                3'b001: cache_write_en <= 4'b0011;    //SH
                3'b010: cache_write_en <= 4'b1111;    //SW                                                 
            endcase
        end
        `LUI:
        begin    //LUI
            load_type       <= `LW;
            //ALU_func<=`ADD;
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ALU_LUI;
            imm_type        <= `UTYPE;     
        end 
        `AUIPC:
        begin    //AUIPC
            load_type       <= `LW;
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ADD;
            imm_type        <= `UTYPE;
        end
        `JAL:
        begin    //JAL
            load_type       <= `LW;
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ADD;
            imm_type        <= `JTYPE;       
        end
        `JALR:
        begin    //JALR      
            load_type       <= `LW;
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ADD;
            imm_type        <= `ITYPE;         
        end
        `BRANCH:
        begin    
            load_type       <= `NOREGWRITE;
            cache_write_en  <= 4'b0000;
            imm_type        <= `BTYPE;
            ALU_func        <= `ADD;   
        end
        default:
        begin     
            load_type       <= `NOREGWRITE;
            cache_write_en  <= 4'b0000;
            ALU_func        <= `ADD;
            imm_type        <= `ITYPE;
        end
    endcase
    /*
    always@(*)
    begin
        case(inst[6:0])
            // R type instructions
            7'b0110011:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 0;               // op2 use reg2
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b11;        // both reg1 and reg2 are used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b00;          // from reg2
                imm_type <= `RTYPE;         // R type imm
                case(inst[31:25])
                    // func7
                    7'b0000000:
                    begin
                        case(inst[14:12])
                            // func3
                            3'b000: ALU_func <= `ADD;
                            3'b001: ALU_func <= `SLL;
                            3'b010: ALU_func <= `SLT;
                            3'b011: ALU_func <= `SLTU;
                            3'b100: ALU_func <= `XOR;
                            3'b101: ALU_func <= `SRL;
                            3'b110: ALU_func <= `OR;
                            3'b111: ALU_func <= `AND;
                        endcase
                    end
                    7'b0100000:
                    begin
                        case(inst[14:12])
                            //func3
                            3'b000: ALU_func <= `SUB;
                            3'b101: ALU_func <= `SRA;
                        endcase
                    end
                endcase
            end
            // I type ALU instructions
            7'b0010011:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `ITYPE;         // I type imm
                case(inst[14:12])
                    // func3
                    3'b000: ALU_func <= `ADD;
                    3'b001: ALU_func <= `SLL;
                    3'b010: ALU_func <= `SLT;
                    3'b011: ALU_func <= `SLTU;
                    3'b100: ALU_func <= `XOR;
                    3'b101:
                    begin
                        case(inst[31:25])
                            7'b0000000: ALU_func <= `SRL;
                            7'b0100000: ALU_func <= `SRA;
                        endcase
                    end
                    3'b110: ALU_func <= `OR;
                    3'b111: ALU_func <= `AND;
                endcase
            end
            // I type Load instructions
            7'b0000011:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 1;             // write back cache
                ALU_func <= `ADD;           // ALU_func choose ADD            
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `ITYPE;         // I type imm
                case(inst[14:12])
                    3'b000: load_type <= `LB;
                    3'b001: load_type <= `LH;
                    3'b010: load_type <= `LW;
                    3'b100: load_type <= `LBU;
                    3'b101: load_type <= `LHU;
                endcase
            end
            // S type Store instructions
            7'b0100011:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b11;        // both reg1 and reg2 are used
                reg_write_en <= 0;          // do not write back register file
                ALU_func <= `ADD;           // ALU_func use add
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `STYPE;         // S type imm
                case(inst[14:12])
                    3'b000: cache_write_en <= 4'b0001;
                    3'b001: cache_write_en <= 4'b0011;
                    3'b010: cache_write_en <= 4'b1111;
                endcase
            end
            // branches
            7'b1100011:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                ALU_func <= 
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b11;        // both reg1 and reg2 are used
                reg_write_en <= 0;          // do not write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b00;          // from reg2
                imm_type <= `BTYPE;         // B type imm
                case(inst[14:12])
                    3'b000: br_type <= `BEQ;
                    3'b001: br_type <= `BNE;
                    3'b100: br_type <= `BLT;
                    3'b101: br_type <= `BGE;
                    3'b110: br_type <= `BLTU;
                    3'b111: br_type <= `BGEU;
                endcase
            end
            // JALR
            7'b1100111:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 1;                  // jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 1;              // write back PCE
                wb_select <= 0;             // write back PCE
                load_type <= ;              
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `ITYPE;         // I type imm
                ALU_func <= `ADD;           // ALU_func use add
            end
            // JAL
            7'b1101111:
            begin
                jal <= 1;                   // jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 1;              // write back PCE
                wb_select <= 0;             // write back PCE
                load_type <= ;              
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `JTYPE;         // J type imm
                ALU_func <= ;
            end
            // LUI
            7'b0110111:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `UTYPE;         // I type imm
            end
            // AUIPC
            7'b0010111:
            begin
                jal <= 0;                   // no jal branch signal
                jalr <= 0;                  // no jalr branch signal
                op2_src <= 1;               // op2 use imm
                br_type <= `NOBRANCH;       // no branch
                load_npc <= 0;              // write back ALU_out
                wb_select <= 0;             // write back ALU_out
                load_type <= ;              
                src_reg_en <= 2'b10;        // reg2 is not used
                reg_write_en <= 1;          // write back register file
                cache_write_en <= 4'b0000;  // do not write data cache
                alu_src1 <= 0;              // from reg1
                alu_src2 <= 2'b10;          // from imm
                imm_type <= `UTYPE;         // I type imm  
            end
        endcase
    end
    */
    // TODO: Complete this module
endmodule