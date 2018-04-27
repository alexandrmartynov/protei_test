#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket
{
public:
    Socket() {};
    virtual ~Socket() {};
    virtual void create() = 0;
//    virtual void close();
protected:
};

#endif
