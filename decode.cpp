#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

void Decode(unsigned int inst)
{
    /* Decode instruction */
    address = inst%(1<<26); //address : 26-bits
    immediate = inst%(1<<16); //immediate : 16-bits
    opcode = inst>>26; //opcode : 6-bits
    funct = inst%(1<<6); //funct : 6-bits
    rt = (inst>>16)%(1<<5); //rt : 5-bits
    rs = (inst>>21)%(1<<5); //rs : 5-bits
    rd = (inst>>11)%(1<<5); //rd : 5-bits
    shamt = (inst>>6)%(1<<5); //shamt : 5-bits
    if(immediate&0x00008000) //simmediate : 32-bits
        simmediate = immediate | 0xffff0000;
    else
        simmediate = immediate & 0x0000ffff;
    ID_rd = rt;

    switch(opcode)
    {
    case 0x00 :
        switch(funct)
        {
        case 0x20 : //add $d,$s,$t
            Inst_ID = "ADD";
            ID_rd = rd;
        break;
        case 0x21 : //addu $d,$s,$t
            Inst_ID = "ADDU";
            ID_rd = rd;
        break;
        case 0x22 : //sub $d,$s,$t
            Inst_ID = "SUB";
            ID_rd = rd;
        break;
        case 0x24 : //and $d,$s,$t
            Inst_ID = "AND";
            ID_rd = rd;
        break;
        case 0x25 : //or $d,$s,$t
            Inst_ID = "OR";
            ID_rd = rd;
        break;
        case 0x26 : //xor $d,$s,$t
            Inst_ID = "XOR";
            ID_rd = rd;
        break;
        case 0x27 : //nor $d,$s,$t
            Inst_ID = "NOR";
            ID_rd = rd;
        break;
        case 0x28 : //nand $d,$s,$t
            Inst_ID = "NAND";
            ID_rd = rd;
        break;
        case 0x2a : //slt $d,$s,$t
            Inst_ID = "SLT";
            ID_rd = rd;
        break;
        case 0x00 : //sll $d,$t,C
            Inst_ID = "SLL";
            if ( rt == 0 && rd == 0 && shamt == 0 )
                Inst_ID = "NOP";
            ID_rd = rd;
        break;
        case 0x02 : //srl $d,$t,C
            Inst_ID = "SRL";
            ID_rd = rd;
        break;
        case 0x03 : //sra $d,$t,C
            Inst_ID = "SRA";
            ID_rd = rd;
        break;
        case 0x08 : //jr $s
            Inst_ID = "JR";
        break;
        case 0x18 : //mult $s $t
            Inst_ID = "MULT";
        break;
        case 0x19 : //multu $s $t
            Inst_ID = "MULTU";
        break;
        case 0x10 : //mfhi $d
            Inst_ID = "MFHI";
            ID_rd = rd;
        break;
        case 0x12 : //mflo $d
            Inst_ID = "MFLO";
            ID_rd = rd;
        break;
        default :
            cout << "illegal instruction found at 0xaddress" ;
            error_halt = 1;
        break;
        }
    break;
    case 0x08 : //addi $t,$s,C
        Inst_ID = "ADDI";
    break;
    case 0x09 : //addiu $t,$s,C
        Inst_ID = "ADDIU";
    break;
    case 0x23 : //lw $t,C($s)
        Inst_ID = "LW";
        ID_rd = rt;
    break;
    case 0x21 : //lh $t,C($s)
        Inst_ID = "LH";
        ID_rd = rt;
    break;
    case 0x25 : //lhu $t,C($s)
        Inst_ID = "LHU";
        ID_rd = rt;
    break;
    case 0x20 : //lb $t,C($s)
        Inst_ID = "LB";
        ID_rd = rt;
    break;
    case 0x24 : //lbu $t,C($s)
        Inst_ID = "LBU";
        ID_rd = rt;
    break;
    case 0x2b : //sw $t,C($s)
        Inst_ID = "SW";
    break;
    case 0x29 : //sh $t,C($s)
        Inst_ID = "SH";
    break;
    case 0x28 : //sb $t,C($s)
        Inst_ID = "SB";
    break;
    case 0x0f : //lui $t,C
        Inst_ID = "LUI";
    break;
    case 0x0c : //andi $t,C
        Inst_ID = "ANDI";
    break;
    case 0x0d : //ori $t,C
        Inst_ID = "ORI";
    break;
    case 0x0e : //nori $t,C
        Inst_ID = "NORI";
    break;
    case 0x0a : //slti $t,C
        Inst_ID = "SLTI";
    break;
    case 0x04 : //beq $s,$t,C
        Inst_ID = "BEQ";
    break;
    case 0x05 : //bne $s,$t,C
        Inst_ID = "BNE";
    break;
    case 0x07 : //bgtz $s,$t,C
        Inst_ID = "BGTZ";
    break;
    case 0x02 : //j C
        Inst_ID = "J";
    break;
    case 0x03 : //jal C
        Inst_ID = "JAL";
        ID_rd = 31;
    break;
    case 0x3f : //halt
        Inst_ID = "HALT";
    break;
    default :
        cout << "illegal instruction found at 0xaddress" ;
        error_halt = 1;
    break;
    }
}

void PC_ALU()
{
    PC.cur = PC.cur + 4;
}
