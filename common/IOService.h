#ifndef IOSERVICE_H
#define IOSERVICE_H

#include "Socket.h"
#include <string>

class IOService
{
public:
    IOService();
    void dialog(Socket* socket);
    std::string echo(Socket* socket);
private:
    std::string getNewMessage() const;

    std::string m_message;
};

#endif
