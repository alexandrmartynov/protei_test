#ifndef SERVER_H
#define SERVER_H

#include "InternetAddress.h"
#include <netinet/in.h>
#include <list>
#include <string>

class Server
{
public:
    Server();
    ~Server();
    int exec();
private:
    void addEvent(int epollfd, int fd) const;
    int setnonblocking(int socket);
    void setup();

    void result(std::string& message) const;
    void displayList(std::list<int> &lst) const;
    void parse(std::string& message, std::list<int> &lst) const;

    static const unsigned int MAX_EVENTS = 10;
    static const unsigned int BUFFER_SIZE = 1024;
    
    sockaddr_in m_client_addr;
    sockaddr_in m_server_addr;
    InternetAddress m_addr;
    char* m_buffer;
};

#endif
