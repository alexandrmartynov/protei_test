#include <iostream>

#include "Application.h"
#include "Protocol.h"

Application::~Application()
{
    delete m_server;
}

int Application::exec()
{
    m_server = new Server();
    int protocol = getProtocolType();
    m_server->setProtocolType(protocol);
    int status = m_server->run();
    return status;
}

int Application::getProtocolType() const
{
    bool correct = false;
    while(!correct)
    {
        std::cout << UDP << " - UDP\n";
        std::cout << TCP << " - TCP\n";
        std::cout << "Please, select type [" << FIRST << "-" << COUNT-1 << "]:";
        int type = 0;
        std::cin >> type;
        if(type >= FIRST && type < COUNT)
        {
            correct = true;
        }
        
        return type;
    }
}
