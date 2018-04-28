#include "Client_tcp.h"
#include "Socket_tcp.h"

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Client_tcp::Client_tcp():
    m_port(0)
{}

int Client_tcp::exec(int port)
{ 
    m_port = port;
    sockaddr_in server_addr;
    setup(server_addr);
    Socket_tcp socket;
    socket.create();
    socket.connected(server_addr);
    socket.handle_message();

    int currentsock = socket.getSocket();
    close(currentsock);

    return 0;

}

void Client_tcp::setup(sockaddr_in& addr) const
{
    socklen_t addrlen = sizeof(addr);
    std::memset(reinterpret_cast<char*>(&addr), 0 , addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    } 
}
