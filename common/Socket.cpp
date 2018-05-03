#include "Socket.h"
#include "IOService.h"

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

void Socket::dialog()
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

std::string Socket::echo()
{
    std::string message = {};
    message = receive();
    send(message);

    return message;   
}

int Socket::getSocket() const
{
    return m_socket;
}

void Socket::setSocket(int socket)
{
    m_socket = socket;
}

void Socket::setupAddress(int port)
{
    m_addr.setup(port);
    m_addrlen = sizeof(m_addr);
}

void Socket::bindSocket()
{
    m_addr.binded(m_socket);
}

void Socket::setNonBlockingSocket()
{
    int flag = fcntl(m_socket, F_GETFL);    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(m_socket, F_SETFL, flag) < 0)
        {
            std::cout << "Set nonblocking socket failed with error "
                      << strerror(errno) << std::endl;
        }
    }
}
