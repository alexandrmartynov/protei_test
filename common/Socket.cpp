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

std::string Socket::getMessage()
{
    m_message.clear();
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> m_message;

    return m_message;
}

void Socket::closeSocket()
{
    close(m_socket);
}
