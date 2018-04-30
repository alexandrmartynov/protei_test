#include "Socket_udp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

Socket_udp::Socket_udp():
    m_socket(0),
    m_message({})
{}

Socket_udp::Socket_udp(const Socket_udp& temp)
{
    this->m_socket = temp.m_socket;
}

Socket_udp::~Socket_udp()
{}

void Socket_udp::create()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "Create socket UDP" << std::endl;
}

void Socket_udp::send(const std::string& message,InternetAddress& addr) const
{
    socklen_t addrlen = addr.getAddrSize();
    sockaddr_in& currentAddr = addr.getAddress();
    std::size_t messageLength = message.size();
    char* buffer = new char[messageLength];
    std::memset(buffer, 0, messageLength);
    std::size_t bytes = message.copy(buffer, messageLength);
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
    m_message.clear();
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        m_message.assign(buffer);
    }

    delete[] buffer;

    return m_message;

}

void Socket_udp::handle_message(InternetAddress& addr)
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
        else
        {
            send(m_message, addr);
            m_message = receive(addr);
            std::cout << "echo: " << m_message << std::endl;
        }
    }
}

std::string Socket_udp::echo_message(InternetAddress& addr)
{
    m_message.clear();
    m_message = receive(addr);
    send(m_message, addr);

    return m_message;
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

std::string Socket_udp::getMessage()
{
    m_message.clear();
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> m_message;

    return m_message;
}
