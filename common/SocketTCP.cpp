#include "SocketTCP.h"

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define BUFFER_SIZE 1024

SocketTCP::SocketTCP():
    m_socket(0)
{}

SocketTCP::~SocketTCP()
{}

int SocketTCP::create()
{
    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::cout << "Create socket TCP" << std::endl;
    if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr)) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    int connect = listen(listen_sock, 1);
    if(connect < 0)
    {
        std::cout << "Listen failed with error " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    bool accepted = false;
    while(!accepted)
    {
        socklen_t client_addrlen = sizeof(m_client_addr);
        m_socket = accept(listen_sock, reinterpret_cast<sockaddr*>(&m_client_addr), &client_addrlen);
        if(m_socket < 0)
        {
            std::cout << "Accept failed with error" << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            accepted = true;
            std::cout << "Accepted TCP socket" << std::endl;
        }
    }
    
    return m_socket;
}

void SocketTCP::send(const std::string& message) const
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

    delete[] buffer;

}

std::string SocketTCP::receive()
{
    char* buffer = new char[BUFFER_SIZE];
    size_t size = strlen(buffer);
    std::memset(buffer, 0, BUFFER_SIZE);
    
    std::size_t bytes = read(m_socket, static_cast<void*>(buffer), size);
    std::string message;
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        message.assign(buffer);
        delete[] buffer;
    }
    
    return message;
}

void SocketTCP::setSocket(int socket)
{
    m_socket = socket;
}

void SocketTCP::writeAddress(sockaddr_in addr)
{
   m_addr = addr;
}
