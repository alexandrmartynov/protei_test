#ifndef SOCKET_H
#define SOCKET_H

class Socket
{
public:
    virtual void create() = 0;
    virtual void closeSocket() = 0;
protected:
};

#endif
