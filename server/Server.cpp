#include "Server.h"
#include "../common/Protocol.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <algorithm>


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
        std::cout << "socket failed with error " << strerror(errno) << std::endl;
        status = EXIT_FAILURE;
    }
    else
    {
        writeServerAddress();

        socklen_t server_addrlen = sizeof(m_server_addr);
        if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_server_addr), server_addrlen) < 0)
        {
            std::cout << "Bind failed with error " << strerror(errno) << std::endl;
            status = EXIT_FAILURE;
        }
        else
        {
            if(m_protocol == TCP)
            {
                int connect = listen(m_socket, MAX_CLIENT);
                if(connect < 0)
                {
                    std::cout << "Listen failed with error " << strerror(errno) << std::endl;
                    status = EXIT_FAILURE;
                }
            }

            socklen_t client_addrlen = sizeof(m_client_addr);
            switch(m_protocol)
            {
                case UDP:
                {
                    m_client_socket = m_socket;
                    
                    bool disconnect = false;
                    while(!disconnect)
                    {
                        std::string message;
                        bool readed_status = m_service.receive_udp(m_client_socket, message,  &m_client_addr, &client_addrlen);
                        if(readed_status)
                        {
                            if(m_service.exit(message.c_str()))
                            {
                                disconnect = true;
                                std::cout << "Client disconnected" << std::endl;
                                close(m_client_socket);
                            }
                            else
                            {
                                calculate(message.c_str());
                                m_service.send_udp(m_client_socket, message.c_str(), &m_client_addr, client_addrlen);
                            }
                        }
                        else
                        {
                            std::cout << "Message not readed!" << std::endl;
                        }
                    }
                    break;
                }
                case TCP:
                {
                    while(true)
                    {
                        socklen_t client_addrlen = sizeof(m_client_addr);
                        m_client_socket = accept(m_socket, reinterpret_cast<sockaddr*>(&m_client_addr), &client_addrlen);
                        if(m_client_socket < 0)
                        {
                            std::cout << "Accept failed with error" << strerror(errno) << std::endl;
                            status = EXIT_FAILURE;
                        }
                        else
                        {
                            std::cout << "Client connected with address " << m_client_addr.sin_addr.s_addr << std::endl;

                            bool disconnect = false;
                            while(!disconnect)
                            {
                                std::string message;
                                bool readed_status = m_service.receive_tcp(m_client_socket, message);
                                if(readed_status)
                                {
                                    if(m_service.exit(message.c_str()))
                                    {
                                        disconnect = true;
                                        std::cout << "Client disconnected" << std::endl;
                                        close(m_client_socket);
                                        
                                    }
                                    else
                                    {
                                        calculate(message.c_str());                                   
                                        m_service.send_tcp(m_client_socket, message.c_str());
                                    }
                                }
                                else
                                {
                                    std::cout << "Message not readed!" << std::endl;
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

void Server::setProtocolType(unsigned short protocol)
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
            std::cout << "Create UDP socket" << std::endl;
            break;
        }
        case TCP:
        {
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            std::cout << "Create TCP socket" << std::endl;
            break;
        }
    }
}

void Server::writeServerAddress()
{
    socklen_t server_addrlen = sizeof(m_server_addr);
    std::memset(reinterpret_cast<char*>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(PORT);
}

int Server::connectClient()
{
    int status = 0;
    socklen_t client_addrlen = sizeof(m_client_addr);
    m_client_socket = accept(m_socket, reinterpret_cast<sockaddr*>(&m_client_addr), &client_addrlen);
    if(m_client_socket < 0)
    {
        std::cout << "Accept failed with error" << strerror(errno) << std::endl;
        status = EXIT_FAILURE;
    }
    
    return status;

}

void Server::calculate(const char* buffer) const
{
    std::list<int> numbers;
    numbers.clear();
    std::string input(buffer);
    std::string::iterator parser = input.begin();

    while(parser != input.end())
    {
        if(*parser >= '0' && *parser <= '9')
        {
            char temp = static_cast<char> (*parser);
            int number = std::atoi(&temp);
            numbers.push_back(number);
        }

        parser++;
    }

    displayList(numbers);

    int sum_numbers = std::accumulate(numbers.begin(), numbers.end(), 0);

    std::cout << "sum of numbers: " << sum_numbers << std::endl;

    numbers.sort(std::greater<int>());

    displayList(numbers);

    auto minmax = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "min: " << *minmax.first << " max: " << *minmax.second << std::endl;
}

void Server::displayList(std::list<int> &lst) const
{
    for(int &item: lst)
    {
        std::cout << item << " ";    
    }
    std::cout << std::endl;
}


