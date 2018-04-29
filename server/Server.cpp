#include "Server.h"
#include "Socket_tcp.h"
#include "Socket_udp.h"
#include "Epoll.h"

#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>
#include <algorithm>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::~Server()
{}

int Server::exec()
{
    Epoll epoll(MAX_EVENTS);

    m_server_addr.setup(PORT);
    Socket_udp m_socket_udp;
    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();
    setnonblocking(sock_udp);
    m_server_addr.binded(sock_udp);

    Socket_tcp m_socket_tcp;
    m_socket_tcp.create();
    int listen_sock = m_socket_tcp.getSocket();
    setnonblocking(listen_sock);
    m_server_addr.binded(listen_sock);
    m_socket_tcp.listening();

    epoll.createEvents();
    epoll.createEpollfd();

    epoll.addEvent(sock_udp);
    epoll.addEvent(listen_sock);
    
    int nfds = 0;
    while(true)
    {
        m_socket_tcp.setSocket(listen_sock);
        
        nfds = epoll.wait();

        int sock_tcp = 0;
        std::string message = {};
        for (int n = 0; n < nfds; ++n)
        {
            int fd = epoll.getfd(n);
            if(fd == listen_sock)
            {
                sock_tcp = m_socket_tcp.accepted(m_client_addr);
                if(sock_tcp < 0)
                {
                    std::cout << "accept failed with error " << strerror(errno) << std::endl;
                    printf("accept failed %d\n", errno);
                    return EXIT_FAILURE;
                }

                setnonblocking(sock_tcp);
                epoll.addEvent(sock_tcp);
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

    return 0;

}

int Server::setnonblocking(int socket)
{
    int result = -1;
    int flag = fcntl(socket, F_GETFL);    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(socket, F_SETFL, flag) >= 0)
        {
            result = 0;
        }
    }

    return result;

}
