#include <iostream>

#include "main.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << usageMessage << std::endl;
    }
    else
    {
        std::cout << argv[1] << "(2, 3) = ";
        int result = 0;
        std::string oper = argv[1];
        if (oper.compare("add") == 0)
        {
            result = add(2, 3);
        }
        else if (oper.compare("mult") == 0)
        {
            result = mult(2, 3);
        }
        else
        {
            std::cout << "Invalid operation" << std::endl;
            return 1;
        }
        std::cout << result << std::endl;
        return 0;
    }
    return 1;
}
