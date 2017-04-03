#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

// Read the data from register
void Read_Reg()
{
    /* Read the register */
    ID_Reg_rs = reg[rs].cur;
    ID_Reg_rt = reg[rt].cur;
    /* determine the rs, rt register value equality */
    isReg_Eq = ~(ID_Reg_rs^ID_Reg_rt);
}
