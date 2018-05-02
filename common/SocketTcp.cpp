#include "SocketTcp.h"

#include <netinet/in.h>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <exception>

SocketTcp::~SocketTcp()
{
    close(m_socket);
}

void SocketTcp::createSocket()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::cout << "Create socket TCP" << std::endl;
}

void SocketTcp::listening() const
{
    int connect = listen(m_socket, MAX_CLIENT);
    if(connect < 0)
    {
        std::cout << "Listen failed" << std::endl;
        throw(strerror(errno));
    }
}

void SocketTcp::connected() const
{
    int connected = connect(m_socket, reinterpret_cast<const sockaddr*>(&m_addr), m_addrlen);
    if(connected < 0)
    {
        std::cout << "Failed connection with error" << std::endl;
        throw(strerror(errno));
    }
}

int SocketTcp::accepted(InternetAddress& addr)
{
    sockaddr_in& currentAddr = addr.getAddress();
    socklen_t addrlen = sizeof(currentAddr);
    int newSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&currentAddr), &addrlen);
    if(newSocket < 0)
    {
        std::cout << "Accept failed" << std::endl;
        throw(strerror(errno));
    }
    else
    {
        std::cout << "Accepted TCP socket" << std::endl;
    }

    return newSocket;

}

void SocketTcp::send(const std::string& message) const
{
    std::size_t bytesToWrite = message.size();
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

std::string SocketTcp::receive()
{
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = read(m_socket, static_cast<void*>(buffer), BUFFER_SIZE);
    
    std::string message = {};
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
    }

    return message;
}

void SocketTcp::handle_message()
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

        send(message);
        message = receive();
        std::cout << "echo: " << message << std::endl;

    } 
}

std::string SocketTcp::echo_message()
{
    std::string message = {};
    message = receive();
    send(message);

    return message;   
}
