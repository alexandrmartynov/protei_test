#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <typeinfo>

#include "IOService.h"

#define BUFFER_SIZE 1024
#define ERROR -1

void IOService::send_tcp(int socket, char* message) const
{
    char* buffer = message;
    size_t bytes = strlen(message);
    size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        size_t bytesWritten = write(
                                   socket,
                                   (void*)currentBufferPosition,
                                   bytesToWrite
                                   );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    }
}

void IOService::send_udp(int socket, char* message, sockaddr_in* addr, socklen_t addrlen) const
{
    char* buffer = message;
    size_t bytes = strlen(message);
    size_t bytesToWrite = bytes;
    char* currentBufferPosition = buffer;
    while(bytesToWrite > 0)
    {
        size_t bytesWritten = sendto(
                                     socket,
                                     (void*)currentBufferPosition,
                                     bytesToWrite,
                                     0,
                                     (sockaddr *) addr,
                                     addrlen
                                    );
        if(bytesWritten <= bytesToWrite)
        {
            bytesToWrite -= bytesWritten;
            currentBufferPosition += bytesWritten;
        }
    } 
}

char* IOService::receive_tcp(int socket) const
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    size_t bytes = read(socket, (void*)buffer, BUFFER_SIZE);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
    }
    else
    {
        buffer = nullptr;
    }
    return buffer;    
}

char* IOService::receive_udp(int socket, sockaddr_in* addr, socklen_t* addrlen) const
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    size_t bytes = recvfrom(socket, (void*)buffer, BUFFER_SIZE, 0, (sockaddr *) addr, addrlen);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
    }
    else
    {
        buffer = nullptr;
    }
    return buffer;
}

char* IOService::getMessage() const
{
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> buffer;
    return buffer;
}

bool IOService::exit(char* message) const
{
    return (strcmp(message, "-exit") == 0);
  
}

