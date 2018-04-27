#include "Client_tcp.h"

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
    setup();
    m_socket.create();

    int client_socket = m_socket.getSocket();
    
    socklen_t server_addrlen = sizeof(m_server_addr);
    m_socket.connected(m_server_addr, server_addrlen);

    bool close = false;
    while(!close)
    {
        close = m_socket.echo_message();
    }

    return 0;

}

std::string Client_tcp::getMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}

void Client_tcp::setup()
{
    socklen_t server_addrlen = sizeof(m_server_addr);
    std::memset(reinterpret_cast<char*>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(m_port);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&m_server_addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    } 
}
