#include "Client_udp.h"

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>

Client_udp::Client_udp():
    m_port(0)
{}

int Client_udp::exec(int port)
{
    m_port = port;
    m_socket.create();
    writeInternetAddress();
    socklen_t server_addrlen = sizeof(m_server_addr);
    m_socket.binded(m_server_addr, server_addrlen);
    
    bool disconnect = false;
    while(!disconnect)
    {
        std::string message = getMessage();
        if(message.compare("-exit") == 0)
        {
            disconnect = true;
            m_socket.send(message, m_server_addr);
            std::cout << "You are disconnected!" << std::endl;
            m_socket.disconnect();
        }
        else
        {
            m_socket.send(message, m_server_addr);
            std::string outputMessage = m_socket.receive(m_server_addr, server_addrlen);
            if(outputMessage.empty())
            {
                std::cout << "Output message: " << outputMessage << std::endl;
            }
            else
            {
                std::cout << "Message not readed!" << std::endl;
            }
        }
    }

    return 0;

}

void Client_udp::writeInternetAddress()
{
    socklen_t server_addrlen = sizeof(m_server_addr);
    std::memset(reinterpret_cast<char*>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(m_port);    
}

std::string Client_udp::getMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}
