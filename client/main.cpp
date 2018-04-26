#include "Client.h"

#include <getopt.h>
#include <iostream>

int main(int argc, char** argv)
{

    const option options[] {
        { "proto", required_argument, NULL, 't' },
        { "port", required_argument, NULL, 'p' },
        { NULL, no_argument, NULL, 0 }        
    };
    
    std::string protocol = {};
    int port = 0;
    int opt = getopt_long(argc, argv, "t:p:", options, nullptr);
    while(opt != -1)
    {
        switch(opt)
        {
            case 't':
            {
                protocol =  optarg;
                break;
            }
            case 'p':
            {
                port = std::atoi(optarg);
                std::cout << "Port: " << port << std::endl;
                break;
            }
            default: /* ? */
            {
                std::cout << "Usage " << argv[0] << " [-t TCP/UDP] [-p port]" << std::endl;
                break;
            }
        }

        opt = getopt_long(argc, argv, "t:p:", options, nullptr);
    }

    if(protocol.empty() || port == 0)
    {
        std::cerr << "Usage " << argv[0] << " [-t TCP/UDP] [-p port]" << std::endl;
        return 1;
    }

    std::cout << "Protocol: " << protocol << " port: " << port << std::endl;

  
    Client client;
    int result = client.exec();
    return result;
}
