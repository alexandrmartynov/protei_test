#include "Server.h"
#include "Socket_tcp.h"
#include "Socket_udp.h"
#include "Epoll.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::~Server()
{}

int Server::exec()
{
    int status = 0;
    Epoll epoll(MAX_EVENTS);

    m_server_addr.setup(PORT);
    Socket_udp m_socket_udp;
    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();
    epoll.setNonBlockingSocket(sock_udp);
    m_server_addr.binded(sock_udp);

    Socket_tcp m_socket_tcp;
    m_socket_tcp.create();
    int listen_sock = m_socket_tcp.getSocket();
    epoll.setNonBlockingSocket(listen_sock);
    m_server_addr.binded(listen_sock);
    m_socket_tcp.listening();

    epoll.createEvents();
    epoll.createEpollfd();

    epoll.addEvent(sock_udp);
    epoll.addEvent(listen_sock);
    
    std::string message = {};
    while(true)
    {
        m_socket_tcp.setSocket(listen_sock);
        
        int countActivefd = epoll.wait();

        for (int activefd = 0; activefd < countActivefd; ++activefd)
        {
            int fd = epoll.getfd(activefd);
            if(fd == listen_sock)
            {
                int sock_tcp = m_socket_tcp.accepted(m_client_addr);
                if(sock_tcp < 0)
                {
                    std::cout << "accept failed with error " << strerror(errno) << std::endl;
                    status = EXIT_FAILURE;
                }
                else
                {
                    epoll.setNonBlockingSocket(sock_tcp);
                    epoll.addEvent(sock_tcp);
                }
            }
            else if(fd == sock_udp)
            {
                std::cout << "UDP connect" << std::endl;
                m_socket_udp.setSocket(fd);
                message.clear();
                message = m_socket_udp.echo_message(m_server_addr);
                if(message.compare("-exit") != 0)
                {
                    m_parser.start(message);
                }
            }
            else
            {
                std::cout << "TCP connect" << std::endl;
                m_socket_tcp.setSocket(fd);
                message.clear();
                message = m_socket_tcp.echo_message();
                if(message.compare("-exit") != 0)
                {
                    m_parser.start(message);
                }
            }
        }
    }

    int epollfd = epoll.getEpollfd();
    close(epollfd);

    return status;

}
