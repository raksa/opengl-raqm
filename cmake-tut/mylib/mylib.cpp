#include <iostream>

void say(std::string str)
{
#ifndef SHOULD_NOT_SAY
    std::cout << str << std::endl;
#endif
}