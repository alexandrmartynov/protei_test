#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include "InternetAddress.h"

class Socket_tcp: public Socket
{
public:
    Socket_tcp();
    virtual ~Socket_tcp();
    virtual void create() override;
    void send(const std::string& message) const;
    std::string receive();

    void binded(InternetAddress& addr);
    void listening() const;
    void connected(InternetAddress& addr) const;
    int accepted(InternetAddress& addr);
    void setSocket(int socket);
    int getSocket() const;
    void closeSocket();
    void handle_message();
    std::string echo_message();
private:
    std::string getMessage() const;

    int m_socket;
};

#endif
