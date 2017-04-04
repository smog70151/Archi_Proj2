#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

//output file
fstream snapshot;    // snapshot.rpt
fstream error_dump;  // error_dump.rpt

//input file
fstream iimage;      // read iimage
fstream dimage;      // read dimage

//reg part
Register reg[32];
Register HI, LO;

PCounter PC;

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

//instruction addr
unsigned int addr;

/* Init var */
bool reset;

/* IF Stage */
unsigned int IF_inst;

/* ID Stage */
string ID_inst;
unsigned int ID_instruction;
bool ID_Flag_PC;
int ID_Reg_rs, ID_Reg_rt;
bool isReg_Eq;
/* Control Signal */
bool IF_Flush;
bool ID_Flush;
bool EX_Flush;

/* EX Stage */
string EX_inst;
int EX_Reg_rs, EX_Reg_rt, EX_Reg_rd;
int EX_rs, EX_rt, EX_rd, EX_shamt;
unsigned int EX_opcode, EX_funct;
unsigned int EX_immediate;
int EX_simmediate;

/* DM Stage */
string DM_inst;
int DM_rd;
unsigned int DM_Reg_rd;
bool isDMforward;

/* WB Stage */
string WB_inst;
int WB_rd;
unsigned int WB_Reg_rd;
bool isWBforward;

/* Hazard detection Unit*/
