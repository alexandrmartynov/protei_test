#include "SocketUDP.h"

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

SocketUDP::SocketUDP():
    m_socket(0)
{}

SocketUDP::~SocketUDP()
{}

int SocketUDP::create()
{
    socklen_t m_addrlen = sizeof(m_addr);
    std::memset(reinterpret_cast<char*>(&m_addr), 0 , m_addrlen);
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(PORT);

    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    std::cout << "Create socket UDP" << std::endl;
    if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_addr), m_addrlen) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    return m_socket;
}

void SocketUDP::send(const std::string& message) const
{
    sockaddr_in addr = m_addr;
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
                                          reinterpret_cast<sockaddr*>(&addr),
                                          sizeof(m_addr)
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
    
    delete[] buffer;

}

std::string SocketUDP::receive()
{
    sockaddr_in addr = m_addr;
    socklen_t addlen = sizeof(addr);
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(
                                 m_socket,
                                 static_cast<void*>(buffer),
                                 BUFFER_SIZE,
                                 0,
                                 reinterpret_cast<sockaddr*>(&addr),
                                 &addlen
                                );
    std::string message;
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
        delete[] buffer;
    }

    return message;

}
