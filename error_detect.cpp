#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

//detect Register[0]
void Error_R0()
{

    if( WB_rd == 0 )
    {
        reg[0].cur = 0; // reset the Register[0]
        error_dump <<  "In cycle " << dec << cyc+1 << ": Write $0 Error" << endl;
    }
}

//detect addition OVF
void Error_OVF(string inst)
{
    if( inst == "ADD" )
    {
        if( ALU_rs_value >= 0 && ALU_rt_value >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && ALU_rt_value < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
    }
    else if ( inst == "SUB" )
    {
        if( ALU_rs_value >= 0 && (~ALU_rt_value+1) >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && (~ALU_rt_value+1) < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
    }
    else if ( inst == "ADDI" || inst == "LW" || inst == "LH" || inst == "LHU" ||
              inst == "LB"   || inst == "LBU"|| inst == "SW" || inst == "SH"  || inst == "SW" )
    {
        if( ALU_rs_value >= 0 && simmediate >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && simmediate < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc+1 << ": Number Overflow" << endl;
    }
}

//detect HI, LO OVW
void Error_OVW()
{
    if( flag_OVW == false )
        error_dump << "In cycle " << dec << cyc+1 << ": Overwrite HI-LO registers" << endl;
    flag_OVW = false;
}

//detect D-memory boundary
void Error_Dmem(string inst,int err_addr)
{
    /* Word */
    if ( inst == "LW" || inst == "SW" )
    {
        if( 0 > err_addr || err_addr > 1020 )
        {
            error_dump <<  "In cycle " << dec << cyc+1 << ": Address Overflow" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Half Word */
    if ( inst == "LH" || inst == "LHU" || inst == "SH" )
    {
        if( 0 > err_addr || err_addr > 1022 )
        {
            error_dump <<  "In cycle " << dec << cyc+1 << ": Address Overflow" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Byte */
    if ( inst == "LB" || inst =="LBU" || inst == "SB" )
    {
        if( 0 > err_addr || err_addr > 1023 )
        {
            error_dump <<  "In cycle " << dec << cyc+1 << ": Address Overflow" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
}

//detect D-memory misaligned
void Error_Misaligned(string inst,int err_addr)
{
    /* Word */
    if ( inst == "LW" || inst == "SW" )
    {
        if( err_addr % 4 != 0 )
        {
            error_dump <<  "In cycle " << dec << cyc+1 << ": Misalignment Error" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Half Word */
    if ( inst == "LH" || inst == "LHU" || inst == "SH" )
    {
        if( err_addr % 2 != 0 )
        {
            error_dump <<  "In cycle " << dec << cyc+1 << ": Misalignment Error" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* byte */
    // Won't Misaligned
}

//detect instructions ( mfhi and mflo )
void Flag_OVW()
{
    flag_OVW = true;
}
