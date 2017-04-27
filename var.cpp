#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

//memory addr
unsigned int addr;

//output file
fstream snapshot;    // snapshot.rpt
fstream error_dump;  // error_dump.rpt

//input file
fstream iimage;      // read iimage
fstream dimage;      // read dimage

//reg part
Register reg[32];
Register HI, LO, PC;

//Instruction and Data memory storage
unsigned int inst_mem[256]; //iimage max 1KB
unsigned int data_mem[1024]; //dimage max 1KB

//cyc
int cyc;

//Decode var
unsigned int address; //26 bits
unsigned int immediate; //16 bits
unsigned int opcode, funct; //6 bits
unsigned int rt, rs, rd, shamt; //5 bits

//Reg var
//data1 = rs, data2 = rt
int read_data1, read_data2; //to calculate
int simmediate; //signed immediate 16 -> 32

//Error
int error_halt; //D memory OVF or Misaligned occur
bool flag_OVW; //to detect HI, LO OVW

/* Pipeline addition */
// Each stage Instruction
unsigned int Inst_IF;
string Inst_ID;
string Inst_EX;
string Inst_DM;
string Inst_WB;

// Stage WB
int WB_rd;
int WB_ALU_outcome;
int WB_Memory_data;
int WB_Forward_data;

// Stage DM
int DM_ALU_outcome;
int DM_Memory_data;
int DM_Forward_data;
int DM_PC; //to overwrite reg[31] ( Instruction = "jal")
int DM_HI, DM_LO;
int DM_rd;

// Stage EX
int EX_ALU_outcome;
int EX_PC;
int EX_rd;
int EX_rt;
int ALU_rs_value, ALU_rt_value;
bool isRSForwarded, isRTForwarded;

// Stage ID
int ID_rd;
int ID_rs_value, ID_rt_value;
int ID_PC;
int Branch;
int Branch_q;
bool isBranch;
bool isIDStalled;
bool isFlushed;
unsigned int Inst_ID_value;

// Stage IF
bool isIFStalled;
bool isIFStoped;
int IF_PC;

// String to detect stall / flush / foward
string IF_info;
string ID_info;
string EX_sinfo;
string EX_tinfo;
