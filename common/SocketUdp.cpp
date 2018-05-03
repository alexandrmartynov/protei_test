#include "SocketUdp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

SocketUdp::SocketUdp()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "Create socket UDP" << std::endl;    
}

void SocketUdp::send(const std::string& message)
{
    std::size_t bytesToWrite = message.size();
    const char* currentPosition = message.c_str();
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                          m_socket,
                                          reinterpret_cast<const void*>(currentPosition),
                                          bytesToWrite,
                                          0,
                                          reinterpret_cast<const sockaddr*>(&m_addr),
                                          m_addrlen
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentPosition += bytesWritten;
        }
    }
}

std::string SocketUdp::receive()
{
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(
                                 m_socket,
                                 static_cast<void*>(buffer),
                                 BUFFER_SIZE,
                                 0,
                                 reinterpret_cast<sockaddr*>(&m_addr),
                                 &m_addrlen
                                );
    std::string message = {};
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
    }

    return message;

}
