#include "IOService.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>

#define BUFFER_SIZE 1024

IOService::IOService():
    m_buffer(new char[BUFFER_SIZE])
{}

IOService::~IOService()
{
    delete[] m_buffer;
}

void IOService::send_tcp(int socket, const char* message) const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    strcpy(m_buffer, message);
    std::size_t bytes = strlen(message);
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = m_buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = write(
                                   socket,
                                   (void*)currentBufferPosition,
                                   bytesToWrite
                                   );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
}

void IOService::send_udp(int socket, const char* message, sockaddr_in* addr, socklen_t addrlen) const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    strcpy(m_buffer, message);
    std::size_t bytes = strlen(message);
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = m_buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                     socket,
                                     (void*)currentBufferPosition,
                                     bytesToWrite,
                                     0,
                                     (sockaddr *) addr,
                                     addrlen
                                    );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    } 
}

std::string IOService::receive_tcp(int socket) const
{
    std::string message;
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = read(socket, static_cast<void*>(m_buffer), BUFFER_SIZE);
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
    }
    return message;    
}

std::string IOService::receive_udp(int socket, sockaddr_in* addr, socklen_t* addrlen) const
{
    std::string message;
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(socket, static_cast<void*>(m_buffer), BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(addr), addrlen);
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
    }
    return message;
}

std::string IOService::getMessage() const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> m_buffer;
    std::cout << m_buffer;
    std::string message(m_buffer);
    return message;
}

bool IOService::exit(const char* message) const
{
    return (strcmp(message, "-exit") == 0);
}
