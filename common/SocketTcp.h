#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"

class SocketTcp: public Socket
{
public:
    explicit SocketTcp();
    virtual ~SocketTcp();
    virtual void send(const std::string& message) const override;
    virtual std::string receive() override;
    void listening() const;
    void connected() const;
    int accepted(InternetAddress& addr);
private:
    static const unsigned int MAX_CLIENT = 1;
    static const std::size_t BUFFER_SIZE = 65536;
};

#endif
