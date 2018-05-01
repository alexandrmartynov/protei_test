#include "Socket.h"

#include <unistd.h>
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

std::string Socket::getMessage()
{
    m_message.clear();
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> m_message;

    return m_message;
}


