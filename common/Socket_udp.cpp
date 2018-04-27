#include "Socket_udp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define PORT 8080
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

void Socket_udp::binded(sockaddr_in& addr)
{
    socklen_t addrlen = sizeof(addr);
    if(bind(m_socket, reinterpret_cast<sockaddr*>(&addr), addrlen) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return;
    }
}

void Socket_udp::send(const std::string& message, sockaddr_in* addr) const
{
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = message.copy(buffer, message.size());
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                          m_socket,
                                          (void*)currentBufferPosition,
                                          bytesToWrite,
                                          0,
                                          reinterpret_cast<sockaddr*>(addr),
                                          sizeof(addr)
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
    
    delete[] buffer;

}

std::string Socket_udp::receive(sockaddr_in* addr, socklen_t* addlen)
{
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(
                                 m_socket,
                                 static_cast<void*>(buffer),
                                 BUFFER_SIZE,
                                 0,
                                 reinterpret_cast<sockaddr*>(addr),
                                 addlen
                                );
    std::string message = {};
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
        std::cout << "receive by udp: " << message << std::endl;
        delete[] buffer;
    }

    return message;

}

void Socket_udp::handle_message(sockaddr_in* addr, socklen_t* addlen)
{
    std::string message = {};
    bool disconnect = false;          
    while(!disconnect)
    {
        message = receive(addr, addlen);
        std::cout << message;
        if(message.compare("-exit") == 0)
        {
            disconnect = true;
        }
        else
        {
            send(message, addr);    
        }
    }  
}

void Socket_udp::disconnect()
{
    close(m_socket);
}

int Socket_udp::getSocket() const
{
    return m_socket;
}

void Socket_udp::setSocket(int socket)
{
    m_socket = socket;
}
