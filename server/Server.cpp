#include "Server.h"
#include "Parser.h"
#include "Epoll.h"
#include "InternetAddress.h"
#include "SocketTcp.h"
#include "SocketUdp.h"
#include "IOService.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"

int Server::exec()
{
    Epoll epoll(MAX_EVENTS);

    SocketTcp socketTcp;
    socketTcp.setupAddress(PORT);
    socketTcp.bindSocket();
    socketTcp.setNonBlockingSocket();
    socketTcp.listening();
    int listenSocket = socketTcp.getSocket();

    SocketUdp socketUdp;
    socketUdp.setupAddress(PORT);
    socketUdp.bindSocket();
    socketUdp.setNonBlockingSocket();
    int currentSocketUdp = socketUdp.getSocket();

    epoll.addEvent(listenSocket);
    epoll.addEvent(currentSocketUdp);
    
    InternetAddress client_addr;
    while(true)
    {

        socketTcp.setSocket(listenSocket);
        
        int countActivefd = epoll.wait();
        for (int activefd = 0; activefd < countActivefd; ++activefd)
        {
            int currentfd = epoll.getfd(activefd);
            if(currentfd == listenSocket)
            {
                int newSocketTcp = socketTcp.accepted(client_addr);
                socketTcp.setSocket(newSocketTcp);
                socketTcp.setNonBlockingSocket();
                epoll.addEvent(newSocketTcp);
            }
            else if(currentfd == currentSocketUdp)
            {
                std::cout << "UDP connect" << std::endl;
                handleMessage(&socketUdp, currentfd);
            }
            else
            {
                std::cout << "TCP connect" << std::endl;
                handleMessage(&socketTcp, currentfd);
            }
        }
    }

    return 0;

}

void Server::handleMessage(Socket* socket, int fd) const
{
    Parser parser;
    IOService service;
    socket->setSocket(fd);
    std::string message = {};
    message = service.echo(socket); 
    if((message.compare("-exit") != 0) && (!message.empty()))
    {
        parser.start(message);
    }    
}
