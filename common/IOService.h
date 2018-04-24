#ifndef IOSERVICE_H
#define IOSERVICE_H

#include <netinet/in.h>
#include <string>

class IOService
{
public:
    explicit IOService();
    ~IOService();
    unsigned short getProtocolType() const;
    int getSocket(int protocol) const;
    void send_tcp(int socket, std::string& message);
    void send_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t addrlen);
    bool receive_tcp(int socket, std::string& message);
    bool receive_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t* addrlen);
    std::string getMessage();
    bool exit(std::string& message) const;
private:
    static const std::size_t BUFFER_SIZE = 65536; 
    char* m_buffer;
};

#endif
