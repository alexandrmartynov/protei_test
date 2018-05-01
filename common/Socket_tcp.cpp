#include "Socket_tcp.h"

#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

Socket_tcp::Socket_tcp(const Socket_tcp& temp)
{
    this->m_socket = temp.m_socket;
}

void Socket_tcp::create()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::cout << "Create socket TCP" << std::endl;
}

void Socket_tcp::listening() const
{
    int connect = listen(m_socket, MAX_CLIENT);
    if(connect < 0)
    {
        std::cout << "Listen failed with error " << strerror(errno) << std::endl;
        return;
    }
}

void Socket_tcp::connected(InternetAddress& addr) const
{
    socklen_t addrlen = addr.getAddrSize();
    sockaddr_in& currentAddr = addr.getAddress();
    int connected = connect(m_socket, reinterpret_cast<const sockaddr*>(&currentAddr), addrlen);
    if(connected < 0)
    {
        std::cout << "Failed connection with error " << strerror(errno) << std::endl;
        return;
    }
}

int Socket_tcp::accepted(InternetAddress& addr)
{
    socklen_t addrlen = addr.getAddrSize();
    sockaddr_in& currentAddr = addr.getAddress();
    int newSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&currentAddr), &addrlen);
    if(newSocket < 0)
    {
        std::cout << "Accept failed with error" << strerror(errno) << std::endl;
        newSocket = EXIT_FAILURE;
    }
    else
    {
        std::cout << "Accepted TCP socket" << std::endl;
    }

    return newSocket;

}

void Socket_tcp::send(const std::string& message) const
{
    std::size_t bytes = message.size();
    std::size_t bytesToWrite = bytes;
    const char* currentPosition = message.c_str();
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = write(
                                         m_socket,
                                         reinterpret_cast<const void*>(currentPosition),
                                         bytesToWrite
                                        );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentPosition += bytesWritten;
        }
    }
}

std::string Socket_tcp::receive()
{
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = read(m_socket, static_cast<void*>(buffer), BUFFER_SIZE);
    
    m_message.clear();
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        m_message.assign(buffer);
    }

    return m_message;
}

void Socket_tcp::handle_message()
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

std::string Socket_tcp::echo_message()
{
    m_message.clear();
    m_message = receive();
    send(m_message);

    return m_message;   
}
