#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
// to include the var
#include "var.h"
// to decode the instruction
#include "decode.h"
// to load the register data
#include "register.h"
// to implement the instruction
#include "alu.h"
// to detect the error
#include "error_detect.h"


void Stage_WB()
{
    IF_info = ID_info = EX_sinfo = EX_tinfo = ""; // Init the Info of each stage
    Inst_WB = Inst_DM; // DFF D4( clk, DM, WB)
    WB_rd = DM_rd; // register destination
    WB_ALU_outcome = DM_ALU_outcome; // DFF
    WB_Memory_data = DM_Memory_data; // DFF
     /* R - Type WB instruction */
    if ( Inst_WB == "ADD"  || Inst_WB == "ADDU" || Inst_WB == "SUB"  ||
         Inst_WB == "AND"  || Inst_WB == "OR"   || Inst_WB == "XOR"  ||
         Inst_WB == "NOR"  || Inst_WB == "NAND" || Inst_WB == "SLT"  ||
         Inst_WB == "SLL"  || Inst_WB == "SRL"  || Inst_WB == "SRA"  ||
         Inst_WB == "ADD"  )
    {
        WB_Forward_data = reg[WB_rd].cur = DM_ALU_outcome; // R-type data rd <- data ( arithmetic )
        Error_R0();
    }
    else if ( Inst_WB == "MFHI" )
    {
        WB_Forward_data = reg[WB_rd].cur = DM_HI;
        Error_R0();
    }
    else if ( Inst_WB == "MFLO" )
    {
        WB_Forward_data = reg[WB_rd].cur = DM_LO;
        Error_R0();
    }
    else if ( Inst_WB == "ADDI" || Inst_WB == "ADDIU" || Inst_WB == "LUI"  ||
              Inst_WB == "ANDI" || Inst_WB == "ORI"   || Inst_WB == "NORI" ||
              Inst_WB == "SLTI" )
    {
        WB_Forward_data = reg[WB_rd].cur = DM_ALU_outcome; // I-type data rd <- data ( arithmetic )
        Error_R0();
    }
    else if ( Inst_WB == "LW" || Inst_WB == "LH" || Inst_WB == "LHU" ||
              Inst_WB == "LB" || Inst_WB == "LBU" )
    {
        WB_Forward_data = reg[WB_rd].cur = DM_Memory_data; // I-type data rd <- data ( memory )
        // cout << "WB_rd :" << dec << WB_rd << endl;
        // cout << "WB_Forward_data :" << dec << WB_Forward_data << endl;
        Error_R0();
    }
    else if ( Inst_WB == "JAL" )
    {
        WB_Forward_data = reg[31].cur = DM_ALU_outcome; // Register 32
        WB_rd = 31;
    }
    else
        WB_rd = 404; // the instruction won't overwrite the Register
}

void Stage_DM()
{
    Inst_DM = Inst_EX;
    DM_rd = EX_rd;

    /* Register Value */
    DM_ALU_outcome = EX_ALU_outcome;
    DM_HI = HI.cur;
    DM_LO = LO.cur;
    DM_PC = EX_PC;


    // Write memory
    if ( Inst_DM == "SW" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1020 )
        {
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur >> 24) & 0x000000ff;
            data_mem[EX_ALU_outcome+1] = (reg[EX_rt].cur >> 16) & 0x000000ff;
            data_mem[EX_ALU_outcome+2] = (reg[EX_rt].cur >>  8) & 0x000000ff;
            data_mem[EX_ALU_outcome+3] = (reg[EX_rt].cur      ) & 0x000000ff;
        }
        // cout << dec << EX_rt << endl;
        // cout << hex << reg[EX_rt].cur << endl;
        // cout << "SW-Mem-addr : " << EX_ALU_outcome << endl;
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "SH" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1022 )
        {
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur >>  8) & 0x000000ff;
            data_mem[EX_ALU_outcome+1] = (reg[EX_rt].cur      ) & 0x000000ff;
        }
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "SB" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1023 )
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur      ) & 0x000000ff;
        Error_Dmem(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "LW" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1020 )
            DM_Memory_data = (data_mem[EX_ALU_outcome  ] << 24) + (data_mem[EX_ALU_outcome+1] << 16)
                           + (data_mem[EX_ALU_outcome+2] <<  8) + (data_mem[EX_ALU_outcome+3]      );
        // cout << "LW-Mem-addr : " << EX_ALU_outcome << endl;
        // cout << "data_mem :" << hex << DM_Memory_data << " start from " << EX_ALU_outcome << endl;
        // cout << "sim" << dec << simmediate << endl;
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "LH" || Inst_DM == "LHU" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1022 )
        {
            DM_Memory_data = (data_mem[EX_ALU_outcome  ] << 8) + (data_mem[EX_ALU_outcome+1]     );
            if( Inst_DM == "LH" && (DM_Memory_data & 0x00008000))
                DM_Memory_data = DM_Memory_data | 0xffff0000;
        }
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "LB" || Inst_DM == "LBU" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1023 )
        {
            DM_Memory_data = data_mem[EX_ALU_outcome];
            if( Inst_DM == "LB" && (DM_Memory_data & 0x00000080))
                DM_Memory_data = DM_Memory_data | 0xffffff00;
        }
        Error_Dmem(Inst_DM, DM_ALU_outcome);
    }
}

void Stage_EX()
{
    if(isIDStalled)
    {
        Inst_EX = "NOP";
        EX_rd = 0;
        ALU_rs_value = 0;
        ALU_rt_value = 0;
        EX_ALU_outcome = 0;
        return;
    }
    isRSForwarded = isRTForwarded = false;
    Inst_EX = Inst_ID;
    EX_rd = ID_rd;
    EX_rt = rt;
    /* ALU input port */
    ALU_rs_value = ID_rs_value;
    ALU_rt_value = ID_rt_value;
    stringstream srs, srt;
    srs << rs;
    srt << rt;
    /* EX-DM Fowarding */
    if ( Inst_DM == "ADD"  || Inst_DM == "ADDU" || Inst_DM == "SUB" ||
         Inst_DM == "AND"  || Inst_DM == "OR"   || Inst_DM == "XOR" ||
         Inst_DM == "NOR"  || Inst_DM == "NAND" || Inst_DM == "SLT" ||
         Inst_DM == "SLL"  || Inst_DM == "SRL"  || Inst_DM == "SRA" ||
         Inst_DM == "MFHI" || Inst_DM == "MFLO" || Inst_DM == "ADDI"||
         Inst_DM == "ADDIU"|| Inst_DM == "LUI"  || Inst_DM == "ANDI"||
         Inst_DM == "ORI"  || Inst_DM == "NORI" || Inst_DM == "SLTI"||
         Inst_DM == "JAL")
    {
        if(Inst_DM == "JAL") DM_rd == 31;
        /* RS and RT Section FWD from EX-DM */
        if( Inst_EX == "ADD"  || Inst_EX == "ADDU" || Inst_EX == "SUB" ||
            Inst_EX == "AND"  || Inst_EX == "OR"   || Inst_EX == "XOR" ||
            Inst_EX == "NOR"  || Inst_EX == "NAND" || Inst_EX == "SLT" ||
            Inst_EX == "MULT" || Inst_EX == "MULTU"|| //R-type
            Inst_EX == "SW"   || Inst_EX == "SH"   || Inst_EX == "SB"  )
        {
            if( DM_rd == rs && DM_rd != 0 )
            {
                isRSForwarded = true;
                ALU_rs_value = DM_ALU_outcome;
                EX_sinfo = EX_sinfo + " fwd_EX-DM_rs_$" + srs.str();
            }
            if( DM_rd == rt && DM_rd != 0 )
            {
                isRTForwarded = true;
                ALU_rt_value = DM_ALU_outcome;
                EX_tinfo = EX_tinfo + " fwd_EX-DM_rt_$" + srt.str();
            }
        }
        /* RS Section FWD from EX-DM */
        if ( Inst_EX == "ADDI" || Inst_EX == "ADDIU" || Inst_EX == "LW" ||
             Inst_EX == "LH"   || Inst_EX == "LHU"   || Inst_EX == "LB" ||
             Inst_EX == "LBU"  || Inst_EX == "ANDI"  || Inst_EX == "ORI"||
             Inst_EX == "NORI" || Inst_EX == "SLTI"  )
        {
            if( DM_rd == rs && DM_rd != 0 )
            {
                isRSForwarded = true;
                ALU_rs_value = DM_ALU_outcome;
                EX_sinfo = EX_sinfo + " fwd_EX-DM_rs_$" + srs.str();
            }
        }
        /* RT Section FWD from EX-DM */
        if ( Inst_EX == "SLL" || Inst_EX == "SRL" || Inst_EX == "SRA" )
        {
            if( DM_rd == rt && DM_rd != 0 )
            {
                isRTForwarded = true;
                ALU_rt_value = DM_ALU_outcome;
                EX_tinfo = EX_tinfo + " fwd_EX-DM_rt_$" + srt.str();
            }
        }
    }
    /* EX-DM Fowarding */
    /* DM-WB Fowarding */
    if ( Inst_WB == "ADD"  || Inst_WB == "ADDU" || Inst_WB == "SUB" ||
         Inst_WB == "AND"  || Inst_WB == "OR"   || Inst_WB == "XOR" ||
         Inst_WB == "NOR"  || Inst_WB == "NAND" || Inst_WB == "SLT" ||
         Inst_WB == "SLL"  || Inst_WB == "SRL"  || Inst_WB == "SRA" ||
         Inst_WB == "MFHI" || Inst_WB == "MFLO" || Inst_WB == "ADDI"||
         Inst_WB == "ADDIU"|| Inst_WB == "LUI"  || Inst_WB == "ANDI"||
         Inst_WB == "ORI"  || Inst_WB == "NORI" || Inst_WB == "SLTI"||
         Inst_WB == "LW"   || Inst_WB == "LH"   || Inst_WB == "LHU" ||
         Inst_WB == "LB"   || Inst_WB == "LBU"  || Inst_WB == "JAL")
    {
        if(Inst_WB == "JAL") WB_rd == 31;
        /* RS and RT Section FWD from DM-WB */
        if( Inst_EX == "ADD"  || Inst_EX == "ADDU" || Inst_EX == "SUB" ||
            Inst_EX == "AND"  || Inst_EX == "OR"   || Inst_EX == "XOR" ||
            Inst_EX == "NOR"  || Inst_EX == "NAND" || Inst_EX == "SLT" ||
            Inst_EX == "MULT" || Inst_EX == "MULTU"|| //R-type
            Inst_EX == "SW"   || Inst_EX == "SH"   || Inst_EX == "SB"  )
        {
            if( WB_rd == rs && !isRSForwarded && WB_rd != 0)
            {
                isRSForwarded = true;
                ALU_rs_value = WB_Forward_data;
                EX_sinfo = EX_sinfo + " fwd_DM-WB_rs_$" + srs.str();
            }
            if( WB_rd == rt && !isRTForwarded && WB_rd != 0)
            {
                isRTForwarded = true;
                ALU_rt_value = WB_Forward_data;
                EX_tinfo = EX_tinfo + " fwd_DM-WB_rt_$" + srt.str();
            }
        }
        /* RS Section FWD from EX-DM */
        if ( Inst_EX == "ADDI" || Inst_EX == "ADDIU" || Inst_EX == "LW" ||
             Inst_EX == "LH"   || Inst_EX == "LHU"   || Inst_EX == "LB" ||
             Inst_EX == "LBU"  || Inst_EX == "ANDI"  || Inst_EX == "ORI"||
             Inst_EX == "NORI" || Inst_EX == "SLTI"  )
        {
            if( WB_rd == rs && !isRSForwarded && WB_rd != 0)
            {
                isRSForwarded = true;
                ALU_rs_value = WB_Forward_data;
                EX_sinfo = EX_sinfo + " fwd_DM-WB_rs_$" + srs.str();
            }
        }
        /* RT Section FWD from EX-DM */
        if ( Inst_EX == "SLL" || Inst_EX == "SRL" || Inst_EX == "SRA" )
        {
            if( WB_rd == rt && !isRTForwarded && WB_rd != 0)
            {
                isRTForwarded = true;
                ALU_rt_value = WB_Forward_data;
                EX_tinfo = EX_tinfo + " fwd_DM-WB_rt_$" + srt.str();
            }
        }
    }
    /* DM-WB Fowarding */
    ALU();
}

void Stage_ID()
{
    if(isIDStalled)
    {
        isIDStalled = false;
        isFlushed = false;
        Inst_ID_value = Inst_ID_value;
    }
    else
        Inst_ID_value = Inst_IF;

    if(isFlushed)
    {
        isFlushed = false;
        isBranch = true;
        Inst_ID_value = 0x00000000;
    }
    EX_PC = ID_PC;

    Decode(Inst_ID_value);
    //cout << "Inst : " << Inst_ID << "  ID_PC : " << setw(8) << hex << ID_PC << endl;

    /* Stall Condition */
    // if(cyc==75) cout << Inst_ID << endl;
    // if(cyc==75) cout << " Register s : "<< setw(2) << setfill('0') << dec << rs << endl;
    // if(cyc==75) cout << " Register t : "<< setw(2) << setfill('0') << dec << rt << endl;
    // if(cyc==75) cout << " Register d : "<< setw(2) << setfill('0') << dec << rd << endl;
    // if(cyc==75) cout << " Register ID_rd : "<< setw(2) << setfill('0') << dec << ID_rd << endl;
    // if(cyc==75) cout << " Register EX_rd : "<< setw(2) << setfill('0') << dec << EX_rd << endl;
    // if(cyc==75) cout << " instruction Value : "<< setw(8) << setfill('0') << hex << Inst_ID_value << endl;

    if( rs == EX_rd || rt == EX_rd || rs == DM_rd || rt == DM_rd )
    {
        /* EX LW -> ID - rs  */
        if((Inst_EX == "LW"  || Inst_EX == "LH"  || Inst_EX == "LHU" ||
            Inst_EX == "LB"  || Inst_EX == "LBU"  ) &&
           (Inst_ID == "LW"  || Inst_ID == "LH"  || Inst_ID == "LHU" ||
            Inst_ID == "LB"  || Inst_ID == "LBU" || Inst_ID == "JR"  ||
            Inst_ID == "ADDI"|| Inst_ID =="ADDIU"|| Inst_ID == "ANDI"||
            Inst_ID == "ORI" || Inst_ID == "NORI"|| Inst_ID == "SLTI"))
        {
            if( rs == EX_rd && EX_rd != 0 )
            {
                ID_info = " to_be_stalled";
                IF_info = " to_be_stalled";
                isIDStalled = true;
                isIFStalled = true;
                return;
            }
        }
        /* EX LW -> ID - rt  */
        if((Inst_EX == "LW"  || Inst_EX == "LH"  || Inst_EX == "LHU" ||
            Inst_EX == "LB"  || Inst_EX == "LBU"  ) &&
           (Inst_ID == "SLL" || Inst_ID == "SRL" || Inst_ID == "SRA" ))
        {
            if( rt == EX_rd && EX_rd != 0  )
            {
                ID_info = " to_be_stalled";
                IF_info = " to_be_stalled";
                isIDStalled = true;
                isIFStalled = true;
                return;
            }
        }
        /* EX LW -> ID rs rt */
        if((Inst_EX == "LW"  || Inst_EX == "LH"  || Inst_EX == "LHU" ||
            Inst_EX == "LB"  || Inst_EX == "LBU"  ) &&
           (Inst_ID == "ADD"  || Inst_ID == "ADDU" || Inst_ID == "SUB" ||
            Inst_ID == "AND"  || Inst_ID == "OR"   || Inst_ID == "XOR" ||
            Inst_ID == "NOR"  || Inst_ID == "NAND" || Inst_ID == "SLT" ||
            Inst_ID == "MULT" || Inst_ID == "MULTU"|| Inst_ID == "SW"  ||
            Inst_ID == "SH"  || Inst_ID == "SB" ))
        {
            if(( rs == EX_rd || rt == EX_rd ) && EX_rd != 0 )
            {
                ID_info = " to_be_stalled";
                IF_info = " to_be_stalled";
                isIDStalled = true;
                isIFStalled = true;
                return;
            }
        }
        if((Inst_EX == "LW"  || Inst_EX == "LH"  || Inst_EX == "LHU" ||
            Inst_EX == "LB"  || Inst_EX == "LBU" || Inst_EX == "ADD" ||
            Inst_EX == "ADDU"|| Inst_EX == "SUB" || Inst_EX == "AND" ||
            Inst_EX == "OR"  || Inst_EX == "XOR" || Inst_EX == "NOR" ||
            Inst_EX == "NAND"|| Inst_EX == "SLT" || Inst_EX == "SLL" ||
            Inst_EX == "SRL" || Inst_EX == "SRA" || Inst_EX == "MFHI"||
            Inst_EX == "MFLO"|| Inst_EX == "ADDI"|| Inst_EX =="ADDIU"||
            Inst_EX == "LUI" || Inst_EX == "ANDI"|| Inst_EX == "ORI" ||
            Inst_EX == "NORI"|| Inst_EX == "SLTI" ) &&
           (Inst_ID == "BEQ"|| Inst_ID == "BNE"|| Inst_ID == "BGTZ"  ||
            Inst_ID == "JR"))
        {
            if( Inst_ID == "JR" || Inst_ID == "BGTZ" )
            {
                if( rs == EX_rd && EX_rd != 0 )
                {
                    ID_info = " to_be_stalled";
                    IF_info = " to_be_stalled";
                    isIDStalled = true;
                    isIFStalled = true;
                    return;
                }
            }
            else
            {
                if((rs == EX_rd || rt == EX_rd ) && EX_rd != 0 )
                {
                    ID_info = " to_be_stalled";
                    IF_info = " to_be_stalled";
                    isIDStalled = true;
                    isIFStalled = true;
                    return;
                }
            }
        }
        /* DM Stall*/
        if((Inst_DM == "LW" || Inst_DM == "LH" || Inst_DM == "LHU" ||
            Inst_DM == "LB" || Inst_DM == "LBU" ) &&
           (Inst_ID == "BEQ"|| Inst_ID == "BNE"|| Inst_ID == "BGTZ"))
        {
            if(( rs == DM_rd || rt == DM_rd ) && DM_rd != 0 )
            {
                ID_info = " to_be_stalled";
                IF_info = " to_be_stalled";
                isIDStalled = true;
                isIFStalled = true;
                return;
            }
        }
        if((Inst_DM == "LW" || Inst_DM == "LH" || Inst_DM == "LHU" ||
            Inst_DM == "LB" || Inst_DM == "LBU" ) &&
            Inst_ID == "JR")
        {
            if( rs == DM_rd && DM_rd != 0 )
            {
                ID_info = " to_be_stalled";
                IF_info = " to_be_stalled";
                isIDStalled = true;
                isIFStalled = true;
                return;
            }
        }
        /* DM Stall */
    }
    /* EX-DM Fowarding */
    Read_Reg();

    if ( Inst_DM == "ADD"  || Inst_DM == "ADDU" || Inst_DM == "SUB" ||
         Inst_DM == "AND"  || Inst_DM == "OR"   || Inst_DM == "XOR" ||
         Inst_DM == "NOR"  || Inst_DM == "NAND" || Inst_DM == "SLT" ||
         Inst_DM == "SLL"  || Inst_DM == "SRL"  || Inst_DM == "SRA" ||
         Inst_DM == "MFHI" || Inst_DM == "MFLO" || Inst_DM == "ADDI"||
         Inst_DM == "ADDIU"|| Inst_DM == "LUI"  || Inst_DM == "ANDI"||
         Inst_DM == "ORI"  || Inst_DM == "NORI" || Inst_DM == "SLTI"||
         Inst_DM == "JAL"  )
    {
        if ( Inst_ID == "BEQ" || Inst_ID == "BNE" )
        {
            stringstream srs, srt;
            srs << rs;
            srt << rt;
            if( DM_rd == rs && rs != 0 )
            {
                ID_rs_value = DM_ALU_outcome;
                ID_info = ID_info + " fwd_EX-DM_rs_$" + srs.str();
            }
            if( DM_rd == rt && rt != 0 )
            {
                ID_rt_value = DM_ALU_outcome;
                ID_info = ID_info + " fwd_EX-DM_rt_$" + srt.str();
            }
        }
        if ( Inst_ID == "BGTZ" || Inst_ID == "JR")
        {
            stringstream srs, srt;
            srs << rs;
            if( DM_rd == rs && rs != 0 )
            {
                ID_rs_value = DM_ALU_outcome;
                ID_info = ID_info + " fwd_EX-DM_rs_$" + srs.str();
            }
        }
    }
    /* EX-DM Fowarding */

    if( Inst_ID == "BEQ" )
    {
        if ( ID_rs_value == ID_rt_value )
        {
            IF_info = " to_be_flushed";
            isFlushed = true;
            Branch = ID_PC + 4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "BNE" )
    {
        if( ID_rs_value != ID_rt_value )
        {
            IF_info = " to_be_flushed";
            isFlushed = true;
            Branch = ID_PC + 4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "BGTZ" )
    {
        if ( ID_rs_value > 0 )
        {
            IF_info = " to_be_flushed";
            isFlushed = true;
            Branch = ID_PC +  4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "JR" )
    {
        IF_info = " to_be_flushed";
        isFlushed = true;
        Branch = ID_rs_value;
        return;
    }
    if( Inst_ID == "J" )
    {
        IF_info = " to_be_flushed";
        isFlushed = true;
        Branch = ((ID_PC+4)>>27) | (immediate<<2);
        return;
    }
    if( Inst_ID == "JAL" )
    {
        IF_info = " to_be_flushed";
        isFlushed = true;
        Branch = ((ID_PC+4)>>27) | (immediate<<2);
        return;
    }
}

void Stage_IF()
{
    if(isIFStoped)
    {
        isIFStoped = false;
        if(isIFStalled)
        {
            isIFStalled = false;
            isIFStoped = true;
        }
        else
            ID_PC = IF_PC;
        return;
    }
    else
    {
        if(isIFStalled)
        {
            //cout << IF_PC << endl;
            isIFStalled = false;
            isIFStoped = true;
        }
    }

    // cout << IF_PC << endl;
    IF_PC = PC.cur;
    if(isBranch)
    {
        IF_PC = PC.cur = Branch ;
        isBranch = false;
    }
    if(!isIFStoped)
        ID_PC = IF_PC;
    Inst_IF = inst_mem[IF_PC/4];
    PC.cur = PC.cur+4;


}
