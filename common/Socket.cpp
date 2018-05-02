#include "Socket.h"

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int Socket::getSocket() const
{
    return m_socket;
}

void Socket::setSocket(int socket)
{
    m_socket = socket;
}

void Socket::closeSocket()
{
    close(m_socket);
}

void Socket::setup(int port)
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
            std::cout << "Set nonblocking socket failed" << std::endl;
        }
    }
}

std::string Socket::getMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}


