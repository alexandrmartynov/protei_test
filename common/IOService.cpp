#include "IOService.h"
#include "Protocol.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>

IOService::IOService():
    m_buffer(new char[BUFFER_SIZE])
{}

IOService::~IOService()
{
    delete[] m_buffer;
}

unsigned short IOService::setProtocolType() const
{
    int type = 0;
    bool correct = false;
    while(!correct)
    {
        std::cout << UDP << " - UDP" << std::endl;
        std::cout << TCP << " - TCP" << std::endl;
        std::cout << "Please, select type [" << FIRST << "-" << (COUNT - 1) << "]:";
        std::cin >> type;
        if(type >= FIRST && type < COUNT)
        {
            correct = true;
        }      
    }

    return type;
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

bool IOService::receive_tcp(int socket, std::string& message) const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = read(socket, static_cast<void*>(m_buffer), BUFFER_SIZE);
    bool status_readed = false;
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
        status_readed = true;
    }

    return status_readed;    
}

bool IOService::receive_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t* addrlen) const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(socket, static_cast<void*>(m_buffer), BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(addr), addrlen);
    bool status_readed = false;
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
        status_readed = true;
    }
    return status_readed;
}

std::string IOService::getMessage() const
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> m_buffer;
    std::string message(m_buffer);
    return message;
}

bool IOService::exit(std::string& message) const
{
    return message.compare("-exit") == 0;
}
