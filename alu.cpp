#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"
// to detect the error
#include "error_detect.h"

using namespace std;

/* R-Type Instructions */ //17
//No EX_shamt
void R_add();
void R_addu();
void R_sub();
void R_and();
void R_or();
void R_xor();
void R_nor();
void R_nand();
void R_slt();
//No rs
void R_sll();
void R_srl();
void R_sra();
//Only rs
void R_jr();
//rs and rt
void R_mult();
void R_multu();
//Only rd
void R_mfhi();
void R_mflo();
/* R-Type Instructions */

/* I-Type Instructions */ //18
void I_addi();
void I_addiu();
void I_lw();
void I_lh();
void I_lhu();
void I_lb();
void I_lbu();
void I_sw();
void I_sh();
void I_sb();
void I_lui();
void I_andi();
void I_ori();
void I_nori();
void I_slti();
void I_beq();
void I_bne();
void I_bgtz();
/* I-Type Instructions */

/* J-Type Instructions */
void J_j();
void J_jal();
/* J-Type Instructions */

/* S-Type Instructions */
void halt();
/* S-Type Instructions */

//implement the meaning of the instruction
void ALU()
{
    switch(EX_opcode)
    {
    case 0x00 :
        switch(EX_funct)
        {
        case 0x20 : //add $d,$s,$t
            R_add();
        break;
        case 0x21 : //addu $d,$s,$t
            R_addu();
        break;
        case 0x22 : //sub $d,$s,$t
            R_sub();
        break;
        case 0x24 : //and $d,$s,$t
            R_and();
        break;
        case 0x25 : //or $d,$s,$t
            R_or();
        break;
        case 0x26 : //xor $d,$s,$t
            R_xor();
        break;
        case 0x27 : //nor $d,$s,$t
            R_nor();
        break;
        case 0x28 : //nand $d,$s,$t
            R_nand();
        break;
        case 0x2a : //slt $d,$s,$t
            R_slt();
        break;
        case 0x00 : //sll $d,$t,C
            R_sll();
        break;
        case 0x02 : //srl $d,$t,C
            R_srl();
        break;
        case 0x03 : //sra $d,$t,C
            R_sra();
        break;
        case 0x08 : //jr $s
            R_jr();
        break;
        case 0x18 : //mult $s $t
            R_mult();
        break;
        case 0x19 : //multu $s $t
            R_multu();
        break;
        case 0x10 : //mfhi $d
            R_mfhi();
        break;
        case 0x12 : //mflo $d
            R_mflo();
        break;
        default :
            cout << "illegal instruction found at 0xaddress" ;
            error_halt = 1;
        break;
        }
    break;
    case 0x08 : //addi $t,$s,C
        I_addi();
    break;
    case 0x09 : //addiu $t,$s,C
        I_addiu();
    break;
    case 0x23 : //lw $t,C($s)
        I_lw();
    break;
    case 0x21 : //lh $t,C($s)
        I_lh();
    break;
    case 0x25 : //lhu $t,C($s)
        I_lhu();
    break;
    case 0x20 : //lb $t,C($s)
        I_lb();
    break;
    case 0x24 : //lbu $t,C($s)
        I_lbu();
    break;
    case 0x2b : //sw $t,C($s)
        I_sw();
    break;
    case 0x29 : //sh $t,C($s)
        I_sh();
    break;
    case 0x28 : //sb $t,C($s)
        I_sb();
    break;
    case 0x0f : //lui $t,C
        I_lui();
    break;
    case 0x0c : //andi $t,C
        I_andi();
    break;
    case 0x0d : //ori $t,C
        I_ori();
    break;
    case 0x0e : //nori $t,C
        I_nori();
    break;
    case 0x0a : //slti $t,C
        I_slti();
    break;
    case 0x04 : //beq $s,$t,C
        I_beq();
    break;
    case 0x05 : //bne $s,$t,C
        I_bne();
    break;
    case 0x07 : //bgtz $s,$t,C
        I_bgtz();
    break;
    case 0x02 : //j C
        J_j();
    break;
    case 0x03 : //jal C
        J_jal();
    break;
    case 0x3f : //halt
        halt();
    break;
    default :
        cout << "illegal instruction found at 0xaddress" ;
        error_halt = 1;
    break;
    }
}

/* R-Type instructions */
/* No EX_shamt */
void R_add()
{
    //$d = $s + $t
    EX_Reg_rd = EX_Reg_rs + EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
}
void R_addu()
{
    //$d = $s + $t(unsigned, no overflow exception)
    EX_Reg_rd = EX_Reg_rs + EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
}
void R_sub()
{
    //$d = $s - $t
    EX_Reg_rd = EX_Reg_rs - EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
}
void R_and()
{
    //$d = $s & $t
    EX_Reg_rd = EX_Reg_rs & EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
}
void R_or()
{
    //$d = $s | $t
    EX_Reg_rd = EX_Reg_rs | EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
}
void R_xor()
{
    //$d = $s ^ $t
    EX_Reg_rd = EX_Reg_rs ^ EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
}
void R_nor()
{
    //$d = ~ ($s | $t)
    EX_Reg_rd = ~(EX_Reg_rs | EX_Reg_rt);
    Error_R0(); //detect Write Register[0]
}
void R_nand()
{
    //$d = ~($s & $t)
    EX_Reg_rd = ~(EX_Reg_rs & EX_Reg_rt);
    Error_R0(); //detect Write Register[0]
}
void R_slt()
{
    //$d = ($s < $t), signed comparison
    EX_Reg_rd = EX_Reg_rs < EX_Reg_rt;
    Error_R0(); //detect Write Register[0]
}

/* No rs */
void R_sll()
{
    //$d = $t << C
    EX_Reg_rd = EX_Reg_rt << EX_shamt;
    if( EX_rt != 0 || EX_rd !=0 || EX_shamt !=0 )
        Error_R0(); //detect Write Register[0]
}
void R_srl()
{
    //$d = $t >> C
    EX_Reg_rd = (unsigned int)EX_Reg_rt >> EX_shamt;

    Error_R0(); //detect Write Register[0]
}
void R_sra()
{
    //$d = $t >> C, with sign bit shifted in
    EX_Reg_rd = EX_Reg_rt >> EX_shamt;
    Error_R0(); //detect Write Register[0]
}

/* Only rs */
void R_jr()
{
    //PC=$s
    //PC.cur = EX_Reg_rs;
}

/* rs and rt */
void R_mult()
{
    //{Hi || Lo} = $s * $t
    unsigned long long temp_rs, temp_rt;
    /* Extension signed rs, rt */
    if(EX_Reg_rs & 0x80000000)
        temp_rs = EX_Reg_rs | 0xffffffff00000000;
    else
        temp_rs = EX_Reg_rs & 0x00000000ffffffff;
    if(EX_Reg_rt & 0x80000000)
        temp_rt = EX_Reg_rt | 0xffffffff00000000;
    else
        temp_rt = EX_Reg_rt & 0x00000000ffffffff;
    HI.cur = (temp_rs*temp_rt) >> 32;
    LO.cur = (temp_rs*temp_rt) & 0x00000000ffffffff;
    Error_OVW(); //detect OVW HI, LO
}
void R_multu()
{
    //{Hi || Lo} = $s * $t (unsigned, no overflow exception)
    unsigned long long temp_rs, temp_rt;
    /* Extension rs, rt */
    temp_rs = EX_Reg_rs & 0x00000000ffffffff;
    temp_rt = EX_Reg_rt & 0x00000000ffffffff;

    HI.cur = (temp_rs*temp_rt) >> 32;
    LO.cur = (temp_rs*temp_rt) & 0x00000000ffffffff;
    Error_OVW(); //detect OVW HI, LO
}

/* Only rd */
void R_mfhi()
{
    //$d = Hi
    EX_Reg_rd = HI.cur;
    Error_R0(); //detect Write Register[0]
    Flag_OVW(); //Flag to detect OVW
}
void R_mflo()
{
    //$d = Lo
    EX_Reg_rd = LO.cur;
    Error_R0(); //detect Write Register[0]
    Flag_OVW(); //Flag to detect OVW
}

/* R-Type instructions */

/* I-Type Instructions */ //18
void I_addi()
{
    //$t = $s + C(signed)
    EX_Reg_rd = EX_Reg_rs + EX_simmediate;
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
}
void I_addiu()
{
    //$t = $s + C(unsigned, no overflow exception)
    EX_Reg_rd = EX_Reg_rs + EX_simmediate;
    Error_R0(); //detect Write Register[0]
}
void I_lw()
{
    //$t = 4 bytes from Memory[$s + C(signed)]
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1020)
        EX_Reg_rd = (data_mem[addr  ] << 24)
                  + (data_mem[addr+1] << 16)
                  + (data_mem[addr+2] <<  8)
                  + (data_mem[addr+3]      );
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory

}
void I_lh()
{
    //$t = 2 bytes from Memory[$s + C(signed)], signed
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1022)
    {
        EX_Reg_rd = (data_mem[addr  ] << 8)
                    + (data_mem[addr+1]     );
        if( EX_Reg_rd & 0x00008000 )
            EX_Reg_rd = EX_Reg_rd | 0xffff0000;
    }
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_lhu()
{
    //$t = 2 bytes from Memory[$s + C(signed)], unsigned
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1022)
        EX_Reg_rd = (data_mem[addr  ] << 8)
                    + (data_mem[addr+1]     );
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_lb()
{
    //$t = Memory[$s + C(signed)], signed
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1023)
    {
        EX_Reg_rd = data_mem[addr];
        if( EX_Reg_rd & 0x00000080 )
            EX_Reg_rd = EX_Reg_rd | 0xffffff00;
    }
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_lbu()
{
    //$t = Memory[$s + C(signed)], unsigned
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1023)
        EX_Reg_rd = data_mem[addr];
    Error_R0(); //detect Write Register[0]
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_sw()
{
    //4 bytes from Memory[$s + C(signed)] = $t
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1020)
    {
        data_mem[addr  ] = (EX_Reg_rd >> 24) & 0x000000ff;
        data_mem[addr+1] = (EX_Reg_rd >> 16) & 0x000000ff;
        data_mem[addr+2] = (EX_Reg_rd >>  8) & 0x000000ff;
        data_mem[addr+3] = (EX_Reg_rd      ) & 0x000000ff;
    }
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_sh()
{
    //2 bytes from Memory[$s + C(signed)] = $t & 0x0000FFFF
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1022)
    {
        data_mem[addr  ] = (EX_Reg_rd >>  8) & 0x000000ff;
        data_mem[addr+1] = (EX_Reg_rd      ) & 0x000000ff;
    }
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_sb()
{
    //Memory[$s + C(signed)] = $t & 0x000000FF
    int addr;
    addr = EX_Reg_rs + EX_simmediate;
    if(0<=addr&&addr<=1023)
        data_mem[addr  ] = (EX_Reg_rd      ) & 0x000000ff;
    Error_OVF(); //detect Adder OVF
    Error_Dmem(addr); //detect D Memory OVF
    Error_Misaligned(addr); //detect S/L Wrong Memory
}
void I_lui()
{
    //$t = C << 16
    EX_Reg_rd = EX_immediate << 16;
    Error_R0(); //detect Write Register[0]
}
void I_andi()
{
    //$t = $s & C(unsigned)
    EX_Reg_rd = EX_Reg_rs & EX_immediate;
    Error_R0(); //detect Write Register[0]
}
void I_ori()
{
    //$t = $s | C(unsigned)
    EX_Reg_rd = EX_Reg_rs | EX_immediate;
    Error_R0(); //detect Write Register[0]
}
void I_nori()
{
    //$t = ~($s | C(unsigned))
    EX_Reg_rd = ~(EX_Reg_rs | EX_immediate);
    Error_R0(); //detect Write Register[0]
}
void I_slti()
{
    //$t = ($s < C(signed) ), signed comparison
    EX_Reg_rd = EX_Reg_rs < EX_simmediate;
    Error_R0(); //detect Write Register[0]
}
void I_beq()
{
    //if ($s == $t) go to PC+4+4*C(signed)
    if( EX_Reg_rs == EX_Reg_rt )
    {
        PC.cur = PC.cur + ( EX_simmediate << 2 );
    }
    Error_OVF(); //detect Adder OVF
}
void I_bne()
{
    //if ($s != $t) go to PC+4+4*C(signed)
    if( EX_Reg_rs != EX_Reg_rt )
    {
        PC.cur = PC.cur + ( EX_simmediate << 2 );
    }
    Error_OVF(); //detect Adder OVF
}
void I_bgtz()
{
    //if ($s > 0) go to PC+4+4*C(signed)
    //if( EX_Reg_rs > 0)
    //    PC.cur = PC.cur + ( EX_simmediate << 2 );
    Error_OVF(); //detect Adder OVF
}
/* I-Type Instructions */

/* J-Type Instructions */
void J_j()
{
    //PC.cur = ( PC.cur >> 27 ) | ( EX_immediate << 2 );
}
void J_jal()
{
    reg[31].cur = PC.cur;
    //PC.cur = ( PC.cur >> 27 ) | ( EX_immediate << 2 );
}
/* J-Type Instructions */

/* S-Type Instructions */
void halt()
{
    error_halt = 1;
}
/* S-Type Instructions */
