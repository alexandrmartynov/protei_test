#include "Client_tcp.h"
#include "Client_udp.h"

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
                port = std::stoi(optarg);
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

    int result = 0;
    if(protocol.empty() || port == 0)
    {
        std::cerr << "Usage " << argv[0] << " [-t tcp/udp] [-p port]" << std::endl;
        result = 1;
    }
    else
    {
        std::cout << "Protocol: " << protocol << " port: " << port << std::endl;

        if(protocol == "tcp")
        {
            Client_tcp client;
            result = client.exec(port);
        }
        else if(protocol == "udp")
        {
            Client_udp client;
            result = client.exec(port);
        }
        else
        {
            std::cout << "wrong protocol type!" << std::endl;
        }
    }

    return result;
}
