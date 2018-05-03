#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"

class SocketTcp: public Socket
{
public:
    explicit SocketTcp();
    void listening() const;
    void connected() const;
    int accepted(InternetAddress& addr);
private:
    virtual void send(const std::string& message) override;
    virtual std::string receive() override;

    static const unsigned int MAX_CLIENT = 1;
};

#endif
