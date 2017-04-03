#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

void Get_Inst_name();
void Signed();

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

    Signed();
    Get_Inst_name();
}

void Get_Inst_name()
{
    switch(opcode)
    {
    case 0x00 :
        switch(funct)
        {
        case 0x20 : //add $d,$s,$t
            ID_inst = "add";
        break;
        case 0x21 : //addu $d,$s,$t
            ID_inst = "addu";
        break;
        case 0x22 : //sub $d,$s,$t
            ID_inst = "sub";
        break;
        case 0x24 : //and $d,$s,$t
            ID_inst = "and";
        break;
        case 0x25 : //or $d,$s,$t
            ID_inst = "or";
        break;
        case 0x26 : //xor $d,$s,$t
            ID_inst = "xor";
        break;
        case 0x27 : //nor $d,$s,$t
            ID_inst = "nor";
        break;
        case 0x28 : //nand $d,$s,$t
            ID_inst = "nand";
        break;
        case 0x2a : //slt $d,$s,$t
            ID_inst = "slt";
        break;
        case 0x00 : //sll $d,$t,C
            ID_inst = "sll";
        break;
        case 0x02 : //srl $d,$t,C
            ID_inst = "srl";
        break;
        case 0x03 : //sra $d,$t,C
            ID_inst = "sra";
        break;
        case 0x08 : //jr $s
            ID_inst = "jr";
        break;
        case 0x18 : //mult $s $t
            ID_inst = "mult";
        break;
        case 0x19 : //multu $s $t
            ID_inst = "multu";
        break;
        case 0x10 : //mfhi $d
            ID_inst = "mfhi";
        break;
        case 0x12 : //mflo $d
            ID_inst = "mflo";
        break;
        default :
            cout << "illegal instruction found at 0xaddress" ;
            error_halt = 1;
        break;
        }
    break;
    case 0x08 : //addi $t,$s,C
        ID_inst = "addi";
    break;
    case 0x09 : //addiu $t,$s,C
        ID_inst = "addiu";
    break;
    case 0x23 : //lw $t,C($s)
        ID_inst = "lw";
    break;
    case 0x21 : //lh $t,C($s)
        ID_inst = "lh";
    break;
    case 0x25 : //lhu $t,C($s)
        ID_inst = "lhu";
    break;
    case 0x20 : //lb $t,C($s)
        ID_inst = "lb";
    break;
    case 0x24 : //lbu $t,C($s)
        ID_inst = "lbu";
    break;
    case 0x2b : //sw $t,C($s)
        ID_inst = "sw";
    break;
    case 0x29 : //sh $t,C($s)
        ID_inst = "sh";
    break;
    case 0x28 : //sb $t,C($s)
        ID_inst = "sb";
    break;
    case 0x0f : //lui $t,C
        ID_inst = "lui";
    break;
    case 0x0c : //andi $t,C
        ID_inst = "andi";
    break;
    case 0x0d : //ori $t,C
        ID_inst = "ori";
    break;
    case 0x0e : //nori $t,C
        ID_inst = "nori";
    break;
    case 0x0a : //slti $t,C
        ID_inst = "slti";
    break;
    case 0x04 : //beq $s,$t,C
        ID_inst = "beq";
    break;
    case 0x05 : //bne $s,$t,C
        ID_inst = "bne";
    break;
    case 0x07 : //bgtz $s,$t,C
        ID_inst = "bgtz";
    break;
    case 0x02 : //j C
        ID_inst = "j";
    break;
    case 0x03 : //jal C
        ID_inst = "jal";
    break;
    case 0x3f : //halt
        ID_inst = "halt";
    break;
    default :
        cout << "illegal instruction found at 0xaddress" ;
        error_halt = 1;
    break;
    }
}

void Signed()
{
    if(immediate&0x00008000)
        simmediate = immediate | 0xffff0000;
    else
        simmediate = immediate & 0x0000ffff;
}

void IF_PC_Adder()
{
    PC.IFa = PC.IF + 4;
}
