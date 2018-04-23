#include <iostream>
#include <string.h>
#include "Terminal.h"
#include "Protocol.h"

#define PROTOCOL_UDP "UDP"
#define PROTOCOL_TCP "TCP"
#define MINIMUM_ARGUMENTS 2


int main(int argc, char** argv)
{
    if(argc < MINIMUM_ARGUMENTS)
    {
        std::cout << "ERROR, no input protocol type\nPlease use following mask: ./client TCP or UDP\n";
        return 0;
    }

    int protocol = 0;
    if(strcmp(argv[1], PROTOCOL_UDP) == 0)
    {
        std::cout << "Connect by UDP" << "\n";
        protocol = UDP;
    }
    else
    {
        std::cout << "Connect by TCP" << "\n";
        protocol = TCP;  
    }

    Terminal terminal;
    int result = terminal.exec(protocol);
    return result;
}
