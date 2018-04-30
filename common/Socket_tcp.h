#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include "InternetAddress.h"

#include <string>

class Socket_tcp: public Socket
{
public:
    explicit Socket_tcp();
    Socket_tcp(const Socket_tcp& temp);
    ~Socket_tcp();
    virtual void create() override;
    virtual void closeSocket() override;

    void listening() const;
    void connected(InternetAddress& addr) const;
    int accepted(InternetAddress& addr);
    
    void send(const std::string& message) const;
    std::string receive();

    void setSocket(int socket);
    int getSocket() const;
    void handle_message();
    std::string echo_message();
private:
    std::string getMessage();

    static const std::size_t BUFFER_SIZE = 65536;

    int m_socket;
    std::string m_message;
};

#endif
