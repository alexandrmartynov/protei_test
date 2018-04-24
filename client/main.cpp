#include "Terminal.h"
#include "../common/Protocol.h"

#include <iostream>
#include <cstring>

#define PROTOCOL_UDP "UDP"
#define PROTOCOL_TCP "TCP"
#define MINIMUM_ARGUMENTS 2


int main(int argc, char** argv)
{
    if(argc < MINIMUM_ARGUMENTS)
    {
        std::cout << "ERROR, no input protocol type" << std::endl
                  << "Please use following mask: ./client TCP or UDP" << std::endl;
        return 0;
    }

    int protocol = 0;
    if(strcmp(argv[1], PROTOCOL_UDP) == 0)
    {
        std::cout << "Connect by UDP" << std::endl;
        protocol = UDP;
    }
    else
    {
        std::cout << "Connect by TCP" << std::endl;
        protocol = TCP;  
    }

    Terminal terminal;
    int result = terminal.exec(protocol);
    return result;
}
