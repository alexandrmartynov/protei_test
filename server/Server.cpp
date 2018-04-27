#include "Server.h"

#include <iostream>
#include <fcntl.h>
#include <cstdio>
#include <sys/epoll.h>


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

Server::Server():
    m_buffer(new char[1024])
{}

Server::~Server()
{
    delete[] m_buffer;
}

void Server::addEvent(int epollfd, int fd) const
{
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return;
    }
}

int Server::setnonblocking(int socket)
{
    int ret = -1;
    int flag = fcntl(socket, F_GETFL);
    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(socket, F_SETFL, flag) >= 0)
        {
            ret = 0;
        }
    }

    return ret;

}

int Server::exec()
{
    sockaddr_in client_addr;
    sockaddr_in server_addr;
    socklen_t server_addrlen = sizeof(server_addr);
    std::memset(reinterpret_cast<char*>(&server_addr), 0 , server_addrlen);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    m_socket_tcp.create();
    int listen_sock = m_socket_tcp.getSocket();

    setnonblocking(m_socket_tcp.getSocket());

    m_socket_tcp.binded(server_addr);

    m_socket_tcp.listening();


    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();

    setnonblocking(sock_udp);

    int nfds = 0;
    int epollfd = 0;
    epoll_event ev;
    epoll_event events[MAX_EVENTS];

    epollfd = epoll_create(2);

    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return EXIT_FAILURE;
    }

    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = sock_udp;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_udp, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return EXIT_FAILURE;
    }

    int numFds = 2;
    while(numFds)
    {
        std::cout << "Waiting connection...\n";
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds <= -1)
        {
            std::cout << "epoll_wait failed" << std::endl;
            return EXIT_FAILURE;
        }    


        memset(m_buffer, 0, BUFFER_SIZE);
        std::string message = {};
        int client_socket = 0;
        socklen_t client_addrlen = sizeof(client_addr);
        std::cout << "nfds: " << nfds << std::endl;

        for (int n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == listen_sock)
            {
                std::cout << "TCP\n";
                client_socket = m_socket_tcp.connect(client_addr);
                if(client_socket < 0)
                {
                    std::cout << "Accept failed with error" << strerror(errno) << std::endl;
                    return EXIT_FAILURE;
                }

                setnonblocking(client_socket);
        
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_socket;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &ev) == -1)
                {
                    std::cout << "failed epoll_ctl" << std::endl;
                    return EXIT_FAILURE;
                }
            }

            if(events[n].data.fd == sock_udp)
            {

                std::cout << "UDP\n";
                bool disconnect = false;          
                while(!disconnect)
                {
                    message = m_socket_udp.receive(client_addr, client_addrlen);
                    //bool isOk = receive_tcp(events[n].data.fd, message);
                    std::cout << message;
                    if(message.compare("-exit") == 0)
                    {
                        disconnect = true;
                    }
                    else
                    {
                        //send_tcp(events[n].data.fd, message); 
                        m_socket_udp.send(message, client_addr); 
                    }  
                }
                close(events[n].data.fd);
                numFds--;
            }

            if(events[n].data.fd == client_socket)
            {
                bool disconnect = false;          
                while(!disconnect)
                {;
                    message = m_socket_tcp.receive();
                    std::cout << message;
                    if(message.compare("-exit") == 0)
                    {
                        disconnect = true;
                    }
                    else
                    {
                        m_socket_tcp.send(message);    
                    }
                    close(events[n].data.fd);
                }
            }
        }
    }
    close(epollfd);
   
    return 0;

}
