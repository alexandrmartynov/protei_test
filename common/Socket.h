#ifndef SOCKET_H
#define SOCKET_H

#include<string>

class Socket
{
public:
    virtual void create() = 0;
    void setSocket(int socket);
    int getSocket() const;
    void closeSocket();
protected:
    std::string getMessage();

    int m_socket;
    std::string m_message;
};

#endif
