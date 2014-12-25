#include "functions.h"
#include <iostream>
#include <vector>

int main(int argc , char* argv[])
{
    std::vector<std::string> line;
    functions::getdata( argc , argv[1] , line );
    return 0;
}
