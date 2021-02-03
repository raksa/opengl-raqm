#include <iostream>

#include "main.h"
#include "mylib.h"

int main(int argc, char const *argv[])
{
    say("Hello World");
#ifdef MAIN_SHOULD_NOT_SAY
    std::cout << "MAIN_SHOULD_NOT_SAY" << std::endl;
#endif
    if (argc < 2)
    {
        std::cout << usageMessage << std::endl;
    }
    else
    {
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
        std::cout << argv[1] << "(2, 3) = ";
        std::cout << result << std::endl;
        return 0;
    }
    return 1;
}
