#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include <string>

class SocketTcp: public Socket
{
public:
    explicit SocketTcp();
    void listening() const;
    void connected() const;
    int accepted(InternetAddress& addr);
    void dialog();
    std::string echo();
private:
    void send(const std::string& message) const;
    std::string receive();

    static const std::size_t BUFFER_SIZE = 65536;
    static const unsigned int MAX_CLIENT = 1;
};

#endif
