#include "Socket_udp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

Socket_udp::Socket_udp(const Socket_udp& temp)
{
    this->m_socket = temp.m_socket;
}

void Socket_udp::create()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "Create socket UDP" << std::endl;
}

void Socket_udp::send(const std::string& message)
{
    std::size_t bytes = message.size();
    std::size_t bytesToWrite = bytes;
    const char* currentPosition = message.c_str();
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                          m_socket,
                                          reinterpret_cast<const void*>(currentPosition),
                                          bytesToWrite,
                                          0,
                                          reinterpret_cast<sockaddr*>(&m_addr),
                                          m_addrlen
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentPosition += bytesWritten;
        }
    }
}

std::string Socket_udp::receive()
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
    m_message.clear();
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        m_message.assign(buffer);
    }

    return m_message;

}

void Socket_udp::handle_message()
{
    m_message.clear();
    bool exit = false;
    while(!exit)
    {
        m_message = getMessage();
        if(m_message.compare("-exit") == 0)
        {
            exit = true;
        }

        send(m_message);
        m_message = receive();
        std::cout << "echo: " << m_message << std::endl;

    }
}

std::string Socket_udp::echo_message()
{
    m_message.clear();
    m_message = receive();
    send(m_message);

    return m_message;
}
