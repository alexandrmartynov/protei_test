#ifndef SERVER_H
#define SERVER_H

#include "InternetAddress.h"

#include <list>
#include <string>

class Server
{
public:
    explicit Server() = default;
    ~Server();
    int exec();
private:
    void addEvent(int epollfd, int fd) const;
    int setnonblocking(int socket);

    void result(std::string& message) const;
    void displayList(std::list<int> &lst) const;
    void parse(std::string& message, std::list<int> &lst) const;

    static const unsigned int MAX_EVENTS = 10;
    static const unsigned int BUFFER_SIZE = 1024;
    
    InternetAddress m_client_addr;
    InternetAddress m_server_addr;
};

#endif
