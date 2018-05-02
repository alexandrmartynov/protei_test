#include "Server.h"
#include "SocketTcp.h"
#include "SocketUdp.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::Server():
    m_epoll(new Epoll(MAX_EVENTS))
{}

Server::~Server()
{
    delete m_epoll;
}

int Server::exec()
{
    SocketTcp socketTcp;
    socketTcp.setup(PORT);
    socketTcp.createSocket();
    socketTcp.bindSocket();
    socketTcp.setNonBlockingSocket();
    socketTcp.listening();
    int listenSocket = socketTcp.getSocket();

    SocketUdp socketUdp;
    socketUdp.setup(PORT);
    socketUdp.createSocket();
    socketUdp.bindSocket();
    socketUdp.setNonBlockingSocket();
    int currentSocketUdp = socketUdp.getSocket();

    m_epoll->addEvent(listenSocket);
    m_epoll->addEvent(currentSocketUdp);
    
    while(true)
    {

        socketTcp.setSocket(listenSocket);
        
        int countActivefd = m_epoll->wait();
        for (int activefd = 0; activefd < countActivefd; ++activefd)
        {
            int fd = m_epoll->getfd(activefd);
            if(fd == listenSocket)
            {
                int newSocketTcp = socketTcp.accepted(m_client_addr);
                socketTcp.setSocket(newSocketTcp);
                socketTcp.setNonBlockingSocket();
                m_epoll->addEvent(newSocketTcp);
            }
            else if(fd == currentSocketUdp)
            {
                std::cout << "UDP connect" << std::endl;
                socketUdp.setSocket(fd);
                std::string message = {};
                message = socketUdp.echo_message();
                if((message.compare("-exit") != 0) && (!message.empty()))
                {
                    m_parser.start(message);
                }
            }
            else
            {
                std::cout << "TCP connect" << std::endl;
                socketTcp.setSocket(fd);
                std::string message = {};
                message = socketTcp.echo_message(); 
                if((message.compare("-exit") != 0) && (!message.empty()))
                {
                    m_parser.start(message);
                }
            }
        }
    }

    return 0;

}
