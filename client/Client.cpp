#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

#include "Client.h"

enum Protocol
{
    UDP,
    TCP,

    COUNT,
    FIRST = TCP
};

Client::Client():
    m_protocol(0),
    m_client_socket(0),
    m_socket(0)
{}

int Client::run()
{
    int status = 0;
    m_socket = setSocket();
    if(m_socket < 0)
    {
        std::cout << "socket failed with error " << strerror(errno) << "\n";
        status = EXIT_FAILURE;
    }
    else
    {
        writeServerAddress();
        socklen_t server_addrlen = sizeof(m_server_addr);
        if(m_protocol == TCP)
        {
            int connected = connect(m_socket, (sockaddr *) &m_server_addr, server_addrlen);
            if(connected < 0)
            {
                std::cout << "Failed connection with error " << strerror(errno) << "\n";
                status = EXIT_FAILURE;
            }
        }
        char* buffer = new char[BUFFER_SIZE];
        bool exit = false;
        while(!exit)
        {
            switch(m_protocol)
            {
                case UDP:
                {
                    while(true)
                    {
                        char* message = m_service.getMessage();
                        m_service.send_udp(m_socket, message, &m_server_addr, server_addrlen);
                        char* outputMessage = m_service.receive_udp(m_socket, &m_server_addr, &server_addrlen);
                        std::cout << "Output message: " << outputMessage << "\n";
                        delete[] message;
                        delete[] outputMessage;
                    }
                    break;
                }
                case TCP:
                {
                    while(true)
                    {
                        char* message = m_service.getMessage();
                        m_service.send_tcp(m_socket, message);
                        char* outputMessage = m_service.receive_tcp(m_socket);
                        std::cout << "Output message: " << outputMessage << "\n";
                        delete[] message;
                        delete[] outputMessage;
                        message = nullptr;
                        outputMessage = nullptr;
                    }
                    break;
                }
            }

        close(m_socket);
        delete[] buffer;
        }  
    }

    return status;

}

void Client::setProtocolType(int protocol)
{
    m_protocol = protocol;
}

int Client::setSocket()
{
    int sockfd = 0;
    switch(m_protocol)
    {
        case UDP:
        {
            sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            std::cout << "Create UDP socket\n";
            break;
        }
        case TCP:
        {
            sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            std::cout << "Create TCP socket\n";
            break;
        }
    }
    return sockfd;
}

void Client::writeServerAddress()
{
    memset((char *) &m_server_addr, 0 , sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(PORT);
    int convert = inet_aton(LOCALHOST, (in_addr *)&m_server_addr.sin_addr.s_addr);
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << "\n";
        return;
    }
}
