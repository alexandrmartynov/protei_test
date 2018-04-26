#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket
{
public:
    Socket() {};
    virtual ~Socket() {};
    virtual int create() = 0;
    virtual void send(const std::string& message) const = 0;
    virtual std::string receive() = 0;
//    virtual void close();
protected:
};

#endif
