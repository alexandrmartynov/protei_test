#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <list>

#include "Server.h"
#include "Protocol.h"

#define MAX_CLIENT 1
#define BUFFER_SIZE 1024
#define PORT 8080

Server::Server():
    m_socket(0),
    m_protocol(0),
    m_client_socket(0)
{}

int Server::run()
{
    int status = 0;
    
    setSocket();
    if(m_socket < 0)
    {
        std::cout << "socket failed with error " << strerror(errno) << "\n";
        status = EXIT_FAILURE;
    }
    else
    {
        writeServerAddress();
        socklen_t server_addrlen = sizeof(m_server_addr);

        if(bind(m_socket, (sockaddr *) &m_server_addr, server_addrlen) < 0)
        {
            std::cout << "Bind failed with error " << strerror(errno) << "\n";
            status = EXIT_FAILURE;
        }
        else
        {
            if(m_protocol == TCP)
            {
                int connect = listen(m_socket, MAX_CLIENT);
                if(connect < 0)
                {
                    std::cout << "Listen failed with error " << strerror(errno) << "\n";
                    status = EXIT_FAILURE;
                }
            }

            socklen_t client_addrlen = sizeof(m_client_addr);
            char* buffer = new char[BUFFER_SIZE];

            switch(m_protocol)
            {
                case UDP:
                {
                    m_client_socket = m_socket;
                    
                    bool disconnect = false;
                    while(!disconnect)
                    {
                        char* message = m_service.receive_udp(m_client_socket, &m_client_addr, &client_addrlen);
                        if(m_service.exit(message))
                        {
                            disconnect = true;
                            std::cout << "Client disconnected\n";
                            close(m_client_socket);
                        }
                        else
                        {
                            calculate(message);
                            if(message == nullptr)
                            {
                                std::cout << "Readed failed with error " << strerror(errno) << "\n";
                                status = EXIT_FAILURE;
                                break;
                            }

                            m_service.send_udp(m_client_socket, message, &m_client_addr, client_addrlen);

                            memset(message, 0, sizeof(message));
                            delete[] message;
                            message = nullptr;
                        }
                    }
                    break;
                }
                case TCP:
                {
                    while(true)
                    {
                        socklen_t client_addrlen = sizeof(m_client_addr);
                        m_client_socket = accept(m_socket, (sockaddr *) &m_client_addr, &client_addrlen);
                        if(m_client_socket < 0)
                        {
                            std::cout << "Accept failed with error" << strerror(errno) << "\n";
                            status = EXIT_FAILURE;
                        }
                        else
                        {
                            std::cout << "Client connected with address " << m_client_addr.sin_addr.s_addr << "\n";

                            bool disconnect = false;
                            while(!disconnect)
                            {
                                char* message = m_service.receive_tcp(m_client_socket);
                                if(m_service.exit(message))
                                {
                                    disconnect = true;
                                    std::cout << "Client disconnected\n";
                                    close(m_client_socket);
                                    
                                }
                                else
                                {
                                    calculate(message);
                                    if(message == nullptr)
                                    {
                                        std::cout << "Readed failed with error " << strerror(errno) << "\n";
                                        status = EXIT_FAILURE;
                                        break;
                                    }
                                    
                                    m_service.send_tcp(m_client_socket, message);

                                    memset(message, 0, sizeof(message));
                                    delete[] message;
                                    message = nullptr;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    return status;
 
}

void Server::setProtocolType(int protocol)
{
    m_protocol = protocol;
}

void Server::setSocket()
{
    switch(m_protocol)
    {
        case UDP:
        {
            m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            std::cout << "Create UDP socket\n";
            break;
        }
        case TCP:
        {
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            std::cout << "Create TCP socket\n";
            break;
        }
    }
}

void Server::writeServerAddress()
{
    memset((char *) &m_server_addr, 0 , sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(PORT);
}

int Server::connectClient()
{
    int status = 0;
    socklen_t client_addrlen = sizeof(m_client_addr);
    m_client_socket = accept(m_socket, (sockaddr *) &m_client_addr, &client_addrlen);
    if(m_client_socket < 0)
    {
        std::cout << "Accept failed with error" << strerror(errno) << "\n";
        status = EXIT_FAILURE;
    }
    
    return status;

}

void Server::calculate(char* buffer) const
{
    std::list<int> numbers;
    numbers.clear();
    std::string input(buffer);
    std::string::iterator parser = input.begin();
    while(parser != input.end())
    {
        if(*parser >= '0' && *parser <= '9')
        {
            char temp = *parser;
            int number = atoi(&temp);
            numbers.push_back(number);
        }

        parser++;
    }

    std::list<int>::iterator current = numbers.begin();
    while(current != numbers.end())
    {
        std::cout << *current << " ";
        current++;
    }
    std::cout << "\n";

    int sum_numbers = std::accumulate(numbers.begin(), numbers.end(), 0);

    std::cout << "sum of numbers: " << sum_numbers << "\n";

    numbers.sort();

    std::list<int>::reverse_iterator current_reverse = numbers.rbegin();
    while(current_reverse != numbers.rend())
    {
        std::cout << *current_reverse << " ";
        current_reverse++;
    }
    std::cout << "\n";

    auto minmax = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "max: " << *minmax.first << " min: " << *minmax.second << "\n";

}


