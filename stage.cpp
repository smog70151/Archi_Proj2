#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
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
    IF_info = ID_info = EX_info = ""; // Init the Info of each stage
    Inst_WB = Inst_DM; // DFF D4( clk, DM, WB)
    WB_rd = DM_rd; // register destination
     /* R - Type WB instruction */
    if ( Inst_WB == "ADD"  || Inst_WB == "ADDU" || Inst_WB == "SUB"  ||
         Inst_WB == "AND"  || Inst_WB == "OR"   || Inst_WB == "XOR"  ||
         Inst_WB == "NOR"  || Inst_WB == "NAND" || Inst_WB == "SLT"  ||
         Inst_WB == "SLL"  || Inst_WB == "SRL"  || Inst_WB == "SRA"  ||
         Inst_WB == "ADD"  )
    {
        reg[WB_rd].cur = DM_ALU_outcome; // R-type data rd <- data ( arithmetic )
        Error_R0();
    }
    else if ( Inst_WB == "MFHI" )
    {
        reg[WB_rd].cur = DM_HI;
        Error_R0();
        Flag_OVW();
    }
    else if ( Inst_WB == "MFLO" )
    {
        reg[WB_rd].cur = DM_LO;
        Error_R0();
        Flag_OVW();
    }
    else if ( Inst_WB == "ADDI" || Inst_WB == "ADDIU" || Inst_WB == "LUI"  ||
              Inst_WB == "ANDI" || Inst_WB == "ORI"   || Inst_WB == "NORI" ||
              Inst_WB == "SLTI" )
    {
        reg[WB_rd].cur = DM_ALU_outcome; // I-type data rd <- data ( arithmetic )
        Error_R0();
    }
    else if ( Inst_WB == "LW" || Inst_WB == "LH" || Inst_WB == "LHU" ||
              Inst_WB == "LB" || Inst_WB == "LBU" )
    {
        reg[WB_rd].cur = DM_Memory_data; // I-type data rd <- data ( memory )
        Error_R0();
    }
    else if ( Inst_WB == "JAL" )
    {
        reg[WB_rd].cur = DM_PC; // Register 32
        Error_R0();
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
        // if( WB_rd == DM_rt ) // Fowarding DM/WB -> DM
        // {
        //     if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1020 )
        //     {
        //         data_mem[EX_ALU_outcome  ] = (reg[WB_rd].cur >> 24) & 0x000000ff;
        //         data_mem[EX_ALU_outcome+1] = (reg[WB_rd].cur >> 16) & 0x000000ff;
        //         data_mem[EX_ALU_outcome+2] = (reg[WB_rd].cur >>  8) & 0x000000ff;
        //         data_mem[EX_ALU_outcome+3] = (reg[WB_rd].cur      ) & 0x000000ff;
        //
        //     }
        // }
        // else
        // {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1020 )
        {
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur >> 24) & 0x000000ff;
            data_mem[EX_ALU_outcome+1] = (reg[EX_rt].cur >> 16) & 0x000000ff;
            data_mem[EX_ALU_outcome+2] = (reg[EX_rt].cur >>  8) & 0x000000ff;
            data_mem[EX_ALU_outcome+3] = (reg[EX_rt].cur      ) & 0x000000ff;
        }
        // }
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "SH" )
    {
        // if( WB_rd == DM_rt ) // Fowarding DM/WB -> DM
        // {
        //     if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1022 )
        //     {
        //         data_mem[EX_ALU_outcome  ] = (reg[WB_rd].cur >>  8) & 0x000000ff;
        //         data_mem[EX_ALU_outcome+1] = (reg[WB_rd].cur      ) & 0x000000ff;
        //     }
        // }
        // else
        // {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1022 )
        {
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur >>  8) & 0x000000ff;
            data_mem[EX_ALU_outcome+1] = (reg[EX_rt].cur      ) & 0x000000ff;
        }
        // }
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        Error_Misaligned(Inst_DM, DM_ALU_outcome);
    }
    else if ( Inst_DM == "SB" )
    {
        // if( WB_rd == DM_rt )
        // {
        //     if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1023 )
        //         data_mem[EX_ALU_outcome  ] = (reg[WB_rd].cur      ) & 0x000000ff;
        // }
        // else
        // {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1023 )
            data_mem[EX_ALU_outcome  ] = (reg[EX_rt].cur      ) & 0x000000ff;
        Error_Dmem(Inst_DM, DM_ALU_outcome);
        // }
    }
    else if ( Inst_DM == "LW" )
    {
        if( 0 <= EX_ALU_outcome && EX_ALU_outcome <= 1020 )
            DM_Memory_data = (data_mem[EX_ALU_outcome  ] << 24) + (data_mem[EX_ALU_outcome+1] << 16)
                           + (data_mem[EX_ALU_outcome+2] <<  8) + (data_mem[EX_ALU_outcome+3]      );
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
            Error_Dmem(Inst_DM, DM_ALU_outcome);
        }
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
    Inst_EX = Inst_ID;
    EX_rd = ID_rd;
    /* ALU input port */
    ALU_rs_value = ID_rs_value;
    ALU_rt_value = ID_rt_value;
    if ( Inst_DM == "ADD"  || Inst_DM == "ADDU"  || Inst_DM == "SUB"  ||
         Inst_DM == "AND"  || Inst_DM == "OR"    || Inst_DM == "XOR"  ||
         Inst_DM == "NOR"  || Inst_DM == "NAND"  || Inst_DM == "SLT"  ||
         Inst_DM == "SLL"  || Inst_DM == "SRL"   || Inst_DM == "SRA"  ||
         Inst_DM == "ADD"  || Inst_DM == "MFHI"  || Inst_DM == "MFLO" ||
         Inst_DM == "ADDI" || Inst_DM == "ADDIU" || Inst_DM == "LUI"  ||
         Inst_DM == "ANDI" || Inst_DM == "ORI"   || Inst_DM == "NORI" ||
         Inst_DM == "SLTI" || Inst_DM == "JAL")
    {
        /* Fowarding */
        if( DM_rd == rs )
            ALU_rs_value = DM_ALU_outcome;
        if( DM_rd == rt)
            ALU_rt_value = DM_ALU_outcome;
    }
    ALU();
}

void Stage_ID()
{
    if(isIDStalled)
    {
        isIDStalled = false;
        isFlushed = false;
        return;
    }
    if(isFlushed)
        Inst_IF = 0x00000000;
    isFlushed = false; // Reset the Branch Flag
    Decode(Inst_IF);

    /* Stall Condition */
    if( rs == EX_rd || rt == EX_rd || rs == DM_rd || rt == DM_rd )
    {
        if( Inst_EX == "LW" || Inst_EX == "LH" || Inst_EX == "LHU" ||
            Inst_EX == "LB" || Inst_EX == "LBU" )
        {
            isIDStalled = true;
            isIFStalled = true;
            return;
        }
        if( Inst_DM == "LW" || Inst_DM == "LH" || Inst_DM == "LHU" ||
            Inst_DM == "LB" || Inst_DM == "LBU" )
        {
            isIDStalled = true;
            isIFStalled = true;
            return;
        }
    }

    /* Fowarding */
    Read_Reg();
    if ( Inst_DM == "ADD"  || Inst_DM == "ADDU"  || Inst_DM == "SUB"  ||
         Inst_DM == "AND"  || Inst_DM == "OR"    || Inst_DM == "XOR"  ||
         Inst_DM == "NOR"  || Inst_DM == "NAND"  || Inst_DM == "SLT"  ||
         Inst_DM == "SLL"  || Inst_DM == "SRL"   || Inst_DM == "SRA"  ||
         Inst_DM == "ADD"  || Inst_DM == "MFHI"  || Inst_DM == "MFLO" ||
         Inst_DM == "ADDI" || Inst_DM == "ADDIU" || Inst_DM == "LUI"  ||
         Inst_DM == "ANDI" || Inst_DM == "ORI"   || Inst_DM == "NORI" ||
         Inst_DM == "SLTI" || Inst_DM == "JAL")
    {
        /* Fowarding */
        if( DM_rd == rs )
            ID_rs_value = DM_ALU_outcome;
        if( DM_rd == rt)
            ID_rt_value = DM_ALU_outcome;
    }
    if( Inst_ID == "BEQ" )
    {

        if ( ID_rs_value == ID_rt_value )
        {
            isFlushed = true;
            ID_PC = PC.cur + 4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "BNE" )
    {
        if( ID_rs_value != ID_rt_value )
        {
            isFlushed = true;
            ID_PC = PC.cur + 4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "BGTZ" )
    {
        if ( ID_rs_value > 0 )
        {
            isFlushed = true;
            ID_PC = PC.cur + 4 + (simmediate<<2);
        }
        return;
    }
    if( Inst_ID == "JR" )
    {
        isFlushed = true;
        ID_PC = ID_rs_value;
        return;
    }
    if( Inst_ID == "J" )
    {
        isFlushed = true;
        ID_PC = ((PC.cur+4)>>27) | (immediate<<2);
        return;
    }
    if( Inst_ID == "JAL" )
    {
        isFlushed = true;
        ID_PC = ((PC.cur+4)>>27) | (immediate<<2);
        return;
    }
}

void Stage_IF()
{
    if(isIFStalled)
    {
        return;
        isStalled = false;
    }
    IF_PC = PC.cur;
    Inst_IF = inst_mem[PC.cur];
    PC.cur = PC.cur+4;
    if(isFlushed)
        PC.cur = ID_PC;
}
