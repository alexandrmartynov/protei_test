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
    void disconnect();

    void binded(sockaddr_in& m_addr);
    void listening() const;
    int connect(sockaddr_in& client_addr);
    void setSocket(int socket);
    int getSocket() const;
private:

    int m_socket;
    sockaddr_in m_addr;
    sockaddr_in m_client_addr;
};

#endif
