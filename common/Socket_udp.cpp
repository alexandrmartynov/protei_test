#include "Socket_udp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define BUFFER_SIZE 1024

Socket_udp::Socket_udp():
    m_socket(0)
{}

Socket_udp::~Socket_udp()
{}

void Socket_udp::create()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "Create socket UDP" << std::endl;
}

void Socket_udp::send(const std::string& message, InternetAddress& addr) const
{
    socklen_t addrlen = addr.getAddrSize();
    sockaddr_in& currentAddr = addr.getAddress();
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = message.copy(buffer, message.size());
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                          m_socket,
                                          reinterpret_cast<void*>(currentBufferPosition),
                                          bytesToWrite,
                                          0,
                                          reinterpret_cast<sockaddr*>(&currentAddr),
                                          addrlen
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
    
    delete[] buffer;

}

std::string Socket_udp::receive(InternetAddress& addr)
{
    socklen_t addrlen = addr.getAddrSize();
    sockaddr_in& currentAddr = addr.getAddress();
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(
                                 m_socket,
                                 static_cast<void*>(buffer),
                                 BUFFER_SIZE,
                                 0,
                                 reinterpret_cast<sockaddr*>(&currentAddr),
                                 &addrlen
                                );
    std::string message = {};
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
    }

    delete[] buffer;

    return message;

}

void Socket_udp::handle_message(InternetAddress& addr)
{
    std::string message = {};
    bool exit = false;
    while(!exit)
    {
        message = getMessage();
        if(message.compare("-exit") == 0)
        {
            exit = true;
        }
        else
        {
            send(message, addr);
            message = receive(addr);
            std::cout << "echo: " << message << std::endl;
        }
    }
}

std::string Socket_udp::echo_message(InternetAddress& addr)
{
    std::string message = {};
    message = receive(addr);
    send(message, addr);

    return message;
}

int Socket_udp::getSocket() const
{
    return m_socket;
}

void Socket_udp::setSocket(int socket)
{
    m_socket = socket;
}

void Socket_udp::closeSocket()
{
    close(m_socket);
}

std::string Socket_udp::getMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}
