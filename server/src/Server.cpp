#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Server.h"
#include "Protocol.h"

#define MAX_CLIENT 1
#define BUFFER_SIZE 1024
#define PORT 8080


int Server::run()
{
    int status = 0;
    
    setSocket();
    if(m_socket < 0)
    {
        std::cout << "socket failed with error " << strerror(errno) << "\n";
        status = EXIT_FAILURE;
    }
    else
    {
        writeServerAddress();
        socklen_t server_addrlen = sizeof(m_server_addr);

        if(bind(m_socket, (sockaddr *) &m_server_addr, server_addrlen) < 0)
        {
            std::cout << "Bind failed with error " << strerror(errno) << "\n";
            status = EXIT_FAILURE;
        }
        else
        {
            if(m_protocol == TCP)
            {
                int connect = listen(m_socket, MAX_CLIENT);
                if(connect < 0)
                {
                    std::cout << "Listen failed with error " << strerror(errno) << "\n";
                    status = EXIT_FAILURE;
                }
            }

            int client_socket = 0;
            socklen_t client_addrlen = sizeof(m_client_addr);
            char* buffer = new char[BUFFER_SIZE];
            while(true)
            {
                switch(m_protocol)
                {
                    case UDP:
                    {
                        client_socket = m_socket;
                        
                        while(true)
                        {
                            int readed_bytes = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (sockaddr *) &m_client_addr, &client_addrlen);
                            if(readed_bytes < 0)
                            {
                                std::cout << "Readed failed with error " << strerror(errno) << "\n";
                                status = EXIT_FAILURE;
                                break;
                            }
                            buffer[readed_bytes] = '\0';
                            size_t bytesToWrite = strlen(buffer);
                            int writed_bytes = sendto(client_socket, buffer, bytesToWrite, 0, (sockaddr *) &m_client_addr, client_addrlen);
                            if(writed_bytes < 0)
                            {
                                std::cout << "Writed failed with error " << strerror(errno) << "\n";
                                status = EXIT_FAILURE;
                                break;
                            }
                        }
                        break;
                    }
                    case TCP:
                    {
                        while(true)
                        {
                            int readed_bytes = read(client_socket, (void *)buffer, BUFFER_SIZE);
                            if(readed_bytes < 0)
                            {
                                std::cout << "Readed failed with error " << strerror(errno) << "\n";
                                status = EXIT_FAILURE;
                                break;
                            }

                            buffer[readed_bytes] = '\0';
                            std::cout << buffer;
                            char* message = buffer;
                            size_t bytes = strlen(message);
                            size_t bytesToWrite = bytes;
                            char* currentMessagePosition = message;
                            while(bytesToWrite > 0)
                            {
                                size_t bytesWritten = write(
                                                           m_socket,
                                                           (void*)currentMessagePosition,
                                                           bytesToWrite);
                                if(bytesWritten <= bytesToWrite)
                                {
                                    bytesToWrite -= bytesWritten;
                                    currentMessagePosition += bytesWritten;
                                }
                            }
                        }

                        break;

                    }
                }
            }
        }

    }   
}

void Server::setProtocolType(int protocol)
{
    m_protocol = protocol;
}

void Server::setSocket()
{
    switch(m_protocol)
    {
        case UDP:
        {
            m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            std::cout << "Create UDP socket\n";
            break;
        }
        case TCP:
        {
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            std::cout << "Create TCP socket\n";
            break;
        }
    }
}

void Server::writeServerAddress()
{
    memset((char *) &m_server_addr, 0 , sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(PORT);
}
