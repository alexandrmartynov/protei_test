#include "Client.h"
#include "../common/Protocol.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client():
    m_protocol(0),
    m_client_socket(0),
    m_socket(0)
{}

int Client::exec()
{
    m_protocol = m_service.setProtocolType();

    int status = 0;
    m_socket = setSocket();
    if(m_socket < 0)
    {
        std::cout << "socket failed with error " << strerror(errno) << std::endl;
        status = EXIT_FAILURE;
    }
    else
    {
        writeServerAddress();
        socklen_t server_addrlen = sizeof(m_server_addr);
        if(m_protocol == TCP)
        {
            int connected = connect(m_socket, reinterpret_cast<sockaddr*>(&m_server_addr), server_addrlen);
            if(connected < 0)
            {
                std::cout << "Failed connection with error " << strerror(errno) << std::endl;
                status = EXIT_FAILURE;
            }
        }

        switch(m_protocol)
        {
            case UDP:
            {
                bool disconnect = false;
                while(!disconnect)
                {
                    std::string message = m_service.getMessage();
                    if(m_service.exit(message))
                    {
                        disconnect = true;
                        m_service.send_udp(m_socket, message.c_str(), &m_server_addr, server_addrlen);
                        std::cout << "You are disconnected!" << std::endl;
                        close(m_socket);
                    }
                    else
                    {
                        m_service.send_udp(m_socket, message.c_str(), &m_server_addr, server_addrlen);
                        std::string outputMessage;
                        bool readed_status = m_service.receive_udp(m_socket, outputMessage, &m_server_addr, &server_addrlen);
                        if(readed_status)
                        {
                            std::cout << "Output message: " << outputMessage << std::endl;
                        }
                        else
                        {
                            std::cout << "Message not readed!" << std::endl;
                        }
                    }
                }
                break;
            }
            case TCP:
            {
                bool disconnect = false;
                while(!disconnect)
                {
                    std::string message = m_service.getMessage();
                    if(m_service.exit(message))
                    {
                        disconnect = true;
                        m_service.send_tcp(m_socket, message.c_str());
                        std::cout << "You are disconnected!" << std::endl;
                        close(m_socket);
                    }
                    else
                    {
                        m_service.send_tcp(m_socket, message.c_str());
                        std::string outputMessage;
                        bool readed_status = m_service.receive_tcp(m_socket, outputMessage);
                        if(readed_status)
                        {
                            std::cout << "Output message: " << outputMessage << std::endl;
                        }
                        else
                        {
                            std::cout << "Message not readed!" << std::endl;
                        }
                    }
                }
                break;
            }
        }  
    }

    return status;

}

int Client::setSocket()
{
    int sockfd = 0;
    switch(m_protocol)
    {
        case UDP:
        {
            sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            std::cout << "Create UDP socket" << std::endl;
            break;
        }
        case TCP:
        {
            sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            std::cout << "Create TCP socket" << std::endl;
            break;
        }
    }
    return sockfd;
}

void Client::writeServerAddress()
{
    socklen_t server_addrlen = sizeof(m_server_addr); 
    std::memset(reinterpret_cast<char *>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(PORT);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&m_server_addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    }
}
