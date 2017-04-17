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
void Error_R0();
//detect addition OVF
void Error_OVF(string inst);
//detect HI, LO OVW
void Error_OVW();
//detect D-memory boundary
void Error_Dmem(string inst, int addr);
//detect D-memory misaligned
void Error_Misaligned(string inst, int addr);
//detect mfhi and mflo
void Flag_OVW();
