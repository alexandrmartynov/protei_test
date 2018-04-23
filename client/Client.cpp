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
                        char* message = getMessage();
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
                        char* message = getMessage();
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

/*void Client::send_udp(char* message)
{
    char* buffer = message;
    size_t bytes = strlen(message);
    size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    socklen_t server_addrlen = sizeof(m_server_addr);
    while(bytesToWrite > 0)
    {
        size_t bytesWritten = sendto(
                                     m_socket,
                                     (void*)currentBufferPosition,
                                     bytesToWrite,
                                     0,
                                     (sockaddr *) &m_server_addr,
                                     server_addrlen
                                    );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }  
}

void Client::send_tcp(char* message)
{
    char* buffer = message;
    size_t bytes = strlen(message);
    size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        size_t bytesWritten = write(
                                   m_socket,
                                   (void*)currentBufferPosition,
                                   bytesToWrite);
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }    
}

char* Client::receive_udp()
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    socklen_t server_addrlen = sizeof(m_server_addr);
    size_t bytes = recvfrom(m_socket, (void*)buffer, BUFFER_SIZE, 0, (sockaddr *) &m_server_addr, &server_addrlen);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
    }
    else
    {
        buffer = nullptr;
    }
    return buffer;
}

char* Client::receive_tcp()
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    size_t bytes = read(m_socket, (void*)buffer, BUFFER_SIZE);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
    }
    else
    {
        buffer = nullptr;
    }
    return buffer;
}*/

char* Client::getMessage() const
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    std::cout << "Write message: ";
    std::cin >> buffer;
    return buffer;
}
