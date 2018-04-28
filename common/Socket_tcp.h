#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include <netinet/in.h>

class Socket_tcp: public Socket
{
public:
    Socket_tcp();
    virtual ~Socket_tcp();
    virtual void create() override;
    void send(const std::string& message) const;
    std::string receive();

    void binded(sockaddr_in& addr);
    void listening() const;
    void connected(sockaddr_in& addr) const;
    int accepted(sockaddr_in* addr);
    void setSocket(int socket);
    int getSocket() const;
    void handle_message();
    std::string echo_message();
private:
    std::string getMessage() const;

    int m_socket;
};

#endif
