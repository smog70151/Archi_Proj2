#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

void Stage_IF();
void Stage_ID();
void Stage_EX();
void Stage_DM();
void Stage_WB();

/* IF section */
void PC_decision();

/* ID section */
void ID_Flush();
void ID_PC_Adder();
void Control();
void Hazard();

/* EX section */
void EX_Get_inst();
void EX_Mux_Reg_rs();
void EX_Mux_Reg_rt();
void EX_Mux_rd(); // to determine which register save the value
