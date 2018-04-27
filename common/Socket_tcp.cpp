#include "Socket_tcp.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define BUFFER_SIZE 1024

Socket_tcp::Socket_tcp():
    m_socket(0)
{}

Socket_tcp::~Socket_tcp()
{
}

void Socket_tcp::create()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::cout << "Create socket TCP" << std::endl;
}

void Socket_tcp::binded(sockaddr_in& addr)
{
    socklen_t addrlen = sizeof(addr);
    if(bind(m_socket, reinterpret_cast<sockaddr*>(&addr), addrlen) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return;
    }
}

void Socket_tcp::listening() const
{
    int connect = listen(m_socket, 2);
    if(connect < 0)
    {
        std::cout << "Listen failed with error " << strerror(errno) << std::endl;
        return;
    }
}

void Socket_tcp::connected(sockaddr_in& addr, socklen_t addrlen) const
{
    int connected = connect(m_socket, reinterpret_cast<const sockaddr*>(&addr), addrlen);
    if(connected < 0)
    {
        std::cout << "Failed connection with error " << strerror(errno) << std::endl;
        return;
    }
}

int Socket_tcp::accepted(sockaddr_in* addr, socklen_t* addrlen)
{
    socklen_t client_addrlen = sizeof(addr);
    int newSocket = accept(m_socket, reinterpret_cast<sockaddr*>(addr), addrlen);
    if(newSocket < 0)
    {
        std::cout << "Accept failed with error" << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Accepted TCP socket" << std::endl;
    }

    return newSocket;

}

void Socket_tcp::send(const std::string& message) const
{
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);
    std::size_t bytes = message.copy(buffer, message.size());
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = write(
                                         m_socket,
                                         reinterpret_cast<void*>(currentBufferPosition),
                                         bytesToWrite
                                        );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
}

std::string Socket_tcp::receive()
{
    char* buffer = new char[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);  
    std::size_t bytes = read(m_socket, static_cast<void*>(buffer), BUFFER_SIZE);
    std::string message = {};
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
        delete[] buffer;
    }

    return message;
}

void Socket_tcp::handle_message()
{
    std::string message = {};
    bool disconnect = false;          
    while(!disconnect)
    {
        message = receive();
        std::cout << message;
        if(message.compare("-exit") == 0)
        {
            disconnect = false;
        }
        else
        {
            send(message);    
        }
    }  
}

void Socket_tcp::setSocket(int socket)
{
    m_socket = socket;
}

int Socket_tcp::getSocket() const
{
    return m_socket;
}

void Socket_tcp::disconnect()
{
    close(m_socket);
}
