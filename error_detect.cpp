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
        error_dump <<  "In cycle " << dec << cyc << ": Write $0 Error" << endl;
    }
}

//detect addition OVF
void Error_OVF(string inst)
{
    if( inst == "ADD" )
    {
        if( ALU_rs_value >= 0 && ALU_rt_value >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && ALU_rt_value < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    }
    else if ( inst == "SUB" )
    {
        if( ALU_rs_value >= 0 && (~ALU_rt_value+1) >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && (~ALU_rt_value+1) < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    }
    else if ( inst == "ADDI" || inst == "LW" || inst == "LH" || inst == "LHU" ||
              inst == "LB"   || inst == "LBU"|| inst == "SW" || inst == "SH"  || inst == "SW" )
    {
        if( ALU_rs_value >= 0 && simmediate >= 0 && EX_ALU_outcome < 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
        if( ALU_rs_value < 0 && simmediate < 0 && EX_ALU_outcome >= 0 )
            error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    }
    // switch( opcode )
    // {
    // /*R-type Instruction OVF detect*/
    // case 0x00 :
    //     switch( funct )
    //     {
    //     case 0x20 : //add
    //         if( read_data1 >= 0 && read_data2 >= 0 && (read_data1+read_data2) < 0 )
    //             error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    //         if( read_data1 < 0 && read_data2 < 0 && (read_data1+read_data2) >= 0 )
    //             error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    //     break;
    //     case 0x22 : //sub
    //         if( read_data1 >=0 && (~read_data2+1) >= 0 && (read_data1-read_data2) < 0 )
    //             error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    //         if( read_data1 < 0 && (~read_data2+1) < 0 && (read_data1-read_data2) >= 0)
    //             error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    //     break;
    //     default :
    //     break;
    //     }
    // break;
    //
    // /*I-type Instruction OVF detect*/
    // case 0x08 : //addi
	// case 0x23 : //lw
	// case 0x21 : //lh
	// case 0x25 : //lhu
	// case 0x20 : //lb
	// case 0x24 : //lbu
	// case 0x2b : //sw
	// case 0x29 : //sh
	// case 0x28 : //sb
	//     if( read_data1 >= 0 && simmediate >= 0 && (read_data1+simmediate) < 0 )
    //         error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    //     if( read_data1 < 0 && simmediate < 0 && (read_data1+simmediate) >= 0 )
    //         error_dump << "In cycle " << dec << cyc << ": Number Overflow" << endl;
    // break;
    //
    // /*I-type Instruction OVF detect (PC+4+4*simmediate)*/
    // case 0x04 : //beq
	// case 0x05 : //bne
	// case 0x07 : //bgtz
    // break;
    //
    // /*Default part*/
    // default :
    // break;
    // }
}

//detect HI, LO OVW
void Error_OVW()
{
    if( flag_OVW == false )
        error_dump << "In cycle " << dec << cyc << ": Overwrite HI-LO registers" << endl;
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
            error_dump <<  "In cycle " << dec << cyc << ": Address Overflow" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Half Word */
    if ( inst == "LH" || inst == "LHU" || inst == "SH" )
    {
        if( 0 > err_addr || err_addr > 1022 )
        {
            error_dump <<  "In cycle " << dec << cyc << ": Address Overflow" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Byte */
    if ( inst == "LB" || inst =="LBU" || inst == "SB" )
    {
        if( 0 > err_addr || err_addr > 1023 )
        {
            error_dump <<  "In cycle " << dec << cyc << ": Address Overflow" << endl;
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
            error_dump <<  "In cycle " << dec << cyc << ": Misalignment Error" << endl;
            error_halt = 1 ; //Switch the Error Flag
        }
    }
    /* Half Word */
    if ( inst == "LH" || inst == "LHU" || inst == "SH" )
    {
        if( err_addr % 2 != 0 )
        {
            error_dump <<  "In cycle " << dec << cyc << ": Misalignment Error" << endl;
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
