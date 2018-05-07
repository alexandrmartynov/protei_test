#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"

class Server
{
public:
    explicit Server() = default;
    ~Server() {};
    int exec();
private:
    void handleMessage(Socket* socket, int fd) const;

    static const unsigned int MAX_EVENTS = 10;
    static const unsigned int PORT = 8080;
};

#endif
