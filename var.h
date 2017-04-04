#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#ifndef VAR_H
#define VAR_H


using namespace std;

//output file
extern fstream snapshot;    // snapshot.rpt
extern fstream error_dump;  // error_dump.rpt

//input file
extern fstream iimage;      // read iimage
extern fstream dimage;      // read dimage

//reg part
typedef struct Register {
    int pre;
    int cur;
} Register;

extern Register reg[32];
extern Register HI, LO;

typedef struct PCounter {
    int IF;
    int ID;
    int EX;
    int IFa;
    int IDa;
    int cur;
} PCounter;

extern PCounter PC;

//Instruction and Data memory storage
extern unsigned int inst_mem[256]; //iimage max 1KB
extern unsigned int data_mem[1024]; //dimage max 1KB

//cyc
extern int cyc;

//Decode var
extern unsigned int address; //26 bits
extern unsigned int immediate; //16 bits
extern unsigned int opcode, funct; //6 bits
extern unsigned int rt, rs, rd, shamt; //5 bits

//Reg var
//data1 = rs, data2 = rt
extern int read_data1, read_data2; //to calculate
extern int simmediate; //signed immediate 16 -> 32

//Error
extern int error_halt; //D memory OVF or Misaligned occur
extern bool flag_OVW; //to detect HI, LO OVW

//instruction addr
extern unsigned int addr;

/* Init var */
extern bool reset;

/* IF Stage */
extern unsigned int IF_inst;

/* ID Stage */
extern string ID_inst;
extern unsigned int ID_instruction;
extern bool ID_Flag_PC;
extern int ID_Reg_rs, ID_Reg_rt;
extern bool isReg_Eq;
//Control
extern bool IF_Flush;
extern bool ID_Flush;
extern bool EX_Flush;

/* EX Stage */
extern string EX_inst;
extern int EX_Reg_rs, EX_Reg_rt, EX_Reg_rd;
extern int EX_rs, EX_rt, EX_rd, EX_shamt;
extern unsigned int EX_opcode, EX_funct;
extern unsigned int EX_immediate;
extern int EX_simmediate;

/* DM Stage */
extern string DM_inst;
extern int DM_rd;
extern unsigned int DM_Reg_rd;
extern bool isDMforward;
/* WB Stage */
extern string WB_inst;
extern int WB_rd;
extern unsigned int WB_Reg_rd;
extern bool isWBforward;

/* Hazard detection Unit*/


#endif // VAR_H
