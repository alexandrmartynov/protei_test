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
{}

int Server::exec()
{
    int status = 0;

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

    Epoll m_epoll(MAX_EVENTS);
    m_epoll.addEvent(listenSocket);
    m_epoll.addEvent(currentSocketUdp);
    
    std::string message = {};
    while(true)
    {
        socketTcp.setSocket(listenSocket);
        
        int countActivefd = m_epoll.wait();

        for (int activefd = 0; activefd < countActivefd; ++activefd)
        {
            int fd = m_epoll.getfd(activefd);
            if(fd == listenSocket)
            {
                int newSocketTcp = socketTcp.accepted(m_client_addr);
                socketTcp.setSocket(newSocketTcp);
                if(newSocketTcp < 0)
                {
                    std::cout << "accept failed with error " << strerror(errno) << std::endl;
                    status = EXIT_FAILURE;
                }
                else
                {
                    socketTcp.setNonBlockingSocket();
                    m_epoll.addEvent(newSocketTcp);
                }
            }
            else if(fd == currentSocketUdp)
            {
                std::cout << "UDP connect" << std::endl;
                socketUdp.setSocket(fd);
                message.clear();
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
                message.clear();
                message = socketTcp.echo_message(); 
                if((message.compare("-exit") != 0) && (!message.empty()))
                {
                    m_parser.start(message);
                }
            }
        }
    }

    return status;

}
