#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#ifndef VAR_H
#define VAR_H

using namespace std;

//memory addr
extern unsigned int addr;

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
    int pts;
} Register;

extern Register reg[32];
extern Register HI, LO, PC;

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

/* Pipeline addition */
// Each stage Instruction
extern unsigned int Inst_IF;
extern string Inst_ID;
extern string Inst_EX;
extern string Inst_DM;
extern string Inst_WB;

// Stage WB
extern int WB_rd;
extern int WB_ALU_outcome;
extern int WB_Memory_data;
extern int WB_Forward_data;

// Stage DM
extern int DM_ALU_outcome;
extern int DM_Memory_data;
extern int DM_Forward_data;
extern int DM_PC; //to overwrite reg[31] ( Instruction = "jal")
extern int DM_HI, DM_LO;
extern int DM_rd;

// Stage EX
extern int EX_ALU_outcome;
extern int EX_PC;
extern int EX_rd;
extern int EX_rt;
extern int ALU_rs_value, ALU_rt_value;
extern bool isRSForwarded, isRTForwarded;

// Stage ID
extern int ID_rd;
extern int ID_rs_value, ID_rt_value;
extern bool isIDStalled;
extern bool isFlushed;
extern int Branch;
extern int Branch_q;
extern bool isBranch;
extern int ID_PC;
extern unsigned int Inst_ID_value;

// Stage IF
extern bool isIFStalled;
extern bool isIFStoped;
extern int IF_PC;

// String to detect stall / flush / foward
extern string IF_info;
extern string ID_info;
extern string EX_sinfo;
extern string EX_tinfo;

#endif // VAR_H
