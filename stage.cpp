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

using namespace std;

// IF
void PC_decision();
// ID
void ID_Flush();
void ID_PC_Adder();
void Control();
void Hazard();
// EX
void EX_Get_inst();
void EX_Mux_Reg_rs();
void EX_Mux_Reg_rt();
void EX_Mux_rd(); // to determine which register save the value

void Stage_IF()
{
    PC_decision(); // determine the PC.cur
    PC.IF = PC.cur; // take PC.cur refer the datapath
    IF_inst = inst_mem[PC.IF]; // Instruction Fetch
    IF_PC_Adder(); // IF_PC_Adder
}
void Stage_ID()
{
    ID_Flush(); //to check flush flag
    PC.ID = PC.ifa-4; //Adder wire
    Decode(ID_instruction); // Decode it and get the information
    Read_Reg(); //Read rs, rt register value
    ID_PC_Adder(); // ID_PC_Adder
    Control(); // Control signal
    Hazard(); //Hazard detection Unit
}
void Stage_EX()
{
    EX_Get_inst(); // get opcode and funct
    EX_Mux_Reg_rs();
    EX_Mux_Reg_rt();
    EX_Mux_rd(); // to determine which register save the value
    ALU();
}
void Stage_DM();
void Stage_WB();

/* IF section */
void PC_decision()
{
    if(reset)
        reset = false;
    else
    {
        PC.cur = PC.IFa;
        if(ID_Flag_PC)
            PC.cur = PC.IDa;
        ID_Flag_PC = false;
    }
}

/* ID section */
void ID_Flush()
{
    if(IF_Flush)
    {
        ID_inst = "NOP";
        ID_instruction = 0x00000000; //NOP
    }
    else
    {
        ID_instruction = IF_inst;
    }
}

void ID_PC_Adder()
{
    PC.IDa = PC.ID + simmediate;
}

void Control()
{
    /* breanch */
    if((ID_inst == "beq" || ID_inst == "bne") && isReg_Eq )
        IF_Flush = true;
    if(ID_inst == "bgtz" && ID_Reg_rs > 0 && immediate != 0)
        IF_Flush = true;
}

void Hazard()
{
    if(ID_instruction)
}

/* EX section */
void EX_Get_inst()
{
    EX_inst = ID_inst;
    EX_opcode = opcode;
    EX_funct = funct;
    EX_shamt = shamt;
    EX_immediate = immediate;
    EX_simmediate = simmediate;
}

void EX_Mux_Reg_rs()
{
    EX_Reg_rs = ID_Reg_rs;
    if(WB_rd == EX_rs)
    {
        EX_Reg_rs = WB_Reg_rd;
        isWBforward = true;
    }
    if(DM_rd == EX_rs)
    {
        EX_Reg_rs = DM_Reg_rd;
        isDMforward = true;
        isWBforward = false; //reset it
    }
}

void EX_Mux_Reg_rt()
{
    EX_Reg_rt = ID_Reg_rt;
    if(WB_rd == EX_rt)
    {
        EX_Reg_rt = WB_Reg_rd;
        isWBforward = true;
    }
    if(DM_rd == EX_rt)
    {
        EX_Reg_rt = DM_Reg_rd;
        isDMforward = true;
        isWBforward = false; //reset it
    }
}

void EX_Mux_rd() // to determine which register save the value
{
    EX_rd = rt;
    if(EX_opcode == 0x00)
        EX_rd = rd;
}
