#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
// to include the var
#include "var.h"

using namespace std;

void Signed();

// Read the data from register
void Read_Reg()
{
    /* Read the register */

    ID_rs_value = reg[rs].cur;
    ID_rt_value = reg[rt].cur;

}
