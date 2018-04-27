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

Client_tcp::Client_tcp():
    m_port(0)
{}

int Client_tcp::exec(int port)
{
    m_port = port;
    m_socket.create();
    writeInternetAddress();
    m_socket.connected(m_server_addr);
    int sockfd = m_socket.accepted(m_client_addr);
    m_socket.setSocket(sockfd);

    bool disconnect = false;
    while(!disconnect)
    {
        std::string message = getMessage();
        if(message.compare("-exit") == 0)
        {
            disconnect = true;
            m_socket.send(message);
            std::cout << "You are disconnected!" << std::endl;
            m_socket.disconnect();
        }
        else
        {
            m_socket.send(message);
            std::string outputMessage = m_socket.receive();
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

std::string Client_tcp::getMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}

void Client_tcp::writeInternetAddress()
{
    socklen_t server_addrlen = sizeof(m_server_addr);
    std::memset(reinterpret_cast<char*>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(m_port);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&m_server_addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    }   
}
