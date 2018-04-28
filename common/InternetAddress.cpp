#include "InternetAddress.h"

#include <cerrno>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <typeinfo>

#define LOCALHOST "127.0.0.1"

void InternetAddress::setup(int port)
{
    socklen_t addrlen = getAddrSize();
    std::memset(reinterpret_cast<char*>(&m_addr), 0 , addrlen);
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(port);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&m_addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    }
}

void InternetAddress::binded(const int socket)
{
    if(bind(socket, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr)) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return;
    }
}

socklen_t InternetAddress::getAddrSize() const
{
    return sizeof(m_addr);
}

sockaddr_in& InternetAddress::getAddress()
{
    return m_addr;
}
