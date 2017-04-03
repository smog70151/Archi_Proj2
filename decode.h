#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

//Decode instruction
void Decode(unsigned int inst);
void Get_Inst_name();
// Signed immediate(16 Bits)
void Signed();

void IF_PC_Adder();
