#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include <string>

class SocketTcp: public Socket
{
public:
    explicit SocketTcp() = default;
    ~SocketTcp();
    virtual void createSocket() override;

    void listening() const;
    void connected() const;
    int accepted(InternetAddress& addr);
    
    void send(const std::string& message) const;
    std::string receive();

    void handle_message();
    std::string echo_message();
private:
    static const std::size_t BUFFER_SIZE = 65536;
    static const unsigned int MAX_CLIENT = 1;
};

#endif
