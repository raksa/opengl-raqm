#include <iostream>

#include "Table.h"

void say(std::string str)
{
#ifndef SHOULD_NOT_SAY
    std::cout << sqrtTable[0] << sqrtTable[1] << sqrtTable[2] << sqrtTable[3] << std::endl;
    std::cout << str << std::endl;
#endif
}