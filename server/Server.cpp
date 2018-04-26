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

#include <sys/poll.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::Server():
    m_buffer(new char[1024])
{}

Server::~Server()
{
    delete[] m_buffer;
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

    int conn_sock = 0;
    int nfds = 0;
    int epollfd = 0;

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setnonblocking(listen_sock);

    std::cout << "listen_sock " << listen_sock << std::endl;
    if(bind(listen_sock, reinterpret_cast<sockaddr*>(&server_addr), server_addrlen) < 0)
    {
        std::cout << "Bind failed with error " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    int connect = listen(listen_sock, 1);
    if(connect < 0)
    {
        std::cout << "Listen failed with error " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    int sock_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setnonblocking(sock_udp);

    epoll_event ev;
    epoll_event events[MAX_EVENTS];
    epollfd = epoll_create(2);
    if (epollfd == -1)
    {
       std::cout << "epoll_create" << std::endl;
       return EXIT_FAILURE;
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return EXIT_FAILURE;
    }
    
    ev.events = EPOLLIN | EPOLLET;
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
        std::string message;
        int client_socket = 0;
        socklen_t client_addrlen = sizeof(client_addr);
        std::cout << "nfds: " << nfds << std::endl;

        for (int n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == listen_sock)
            {
                std::cout << "TCP\n";
                client_socket = accept(listen_sock, reinterpret_cast<sockaddr*>(&client_addr), &client_addrlen);
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
                    bool isOk = receive_udp(events[n].data.fd, message, &client_addr, &client_addrlen);
                    //bool isOk = receive_tcp(events[n].data.fd, message);
                    std::cout << message;
                    if(message.compare("-exit") == 0)
                    {
                        disconnect = true;
                    }
                    else
                    {
                        //send_tcp(events[n].data.fd, message); 
                        send_udp(events[n].data.fd, message, &client_addr, client_addrlen); 
                    }  
                }
                close(events[n].data.fd);
                numFds--;
            }

            if(events[n].data.fd == client_socket)
            {
                bool disconnect = false;          
                while(!disconnect)
                {
                    //bool isOk = receive_udp(events[n].data.fd, message, &client_addr, &client_addrlen);
                    bool isOk = receive_tcp(events[n].data.fd, message);
                    std::cout << message;
                    if(message.compare("-exit") == 0)
                    {
                        disconnect = true;
                    }
                    else
                    {
                        send_tcp(events[n].data.fd, message); 
                        //send_udp(events[n].data.fd, message, &client_addr, client_addrlen);   
                    }
                    close(events[n].data.fd);
                }
            }
        }
    }
    close(epollfd);
   
    return 0;

}

int Server::setnonblocking(int sock)
{
    int ret = -1;
    int flag = fcntl(sock, F_GETFL);
    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(sock, F_SETFL, flag) >= 0)
        {
            ret = 0;
        }
    }

    return ret;

}

void Server::send_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t addrlen)
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = message.copy(m_buffer, message.size());
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = m_buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = sendto(
                                          socket,
                                          (void*)currentBufferPosition,
                                          bytesToWrite,
                                          0,
                                          reinterpret_cast<sockaddr*>(addr),
                                          addrlen
                                         );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    } 
}

bool Server::receive_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t* addrlen)
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = recvfrom(
                                 socket,
                                 static_cast<void*>(m_buffer),
                                 BUFFER_SIZE,
                                 0,
                                 reinterpret_cast<sockaddr*>(addr),
                                 addrlen
                                );
    bool status_readed = false;
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
        status_readed = true;
    }

    return status_readed;

}

bool Server::receive_tcp(int socket, std::string& message)
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = read(socket, static_cast<void*>(m_buffer), BUFFER_SIZE);
    bool status_readed = false;
    if(bytes > 0)
    {
        m_buffer[bytes] = '\0';
        message.assign(m_buffer);
        status_readed = true;
    }

    return status_readed;    
}

void Server::send_tcp(int socket, std::string& message)
{
    std::memset(m_buffer, 0, BUFFER_SIZE);
    std::size_t bytes = message.copy(m_buffer, message.size());
    std::size_t bytesToWrite = bytes;
    char* currentBufferPosition = m_buffer;
    while(bytesToWrite > 0)
    {
        std::size_t bytesWritten = write(
                                         socket,
                                         reinterpret_cast<void*>(currentBufferPosition),
                                         bytesToWrite
                                        );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
}

