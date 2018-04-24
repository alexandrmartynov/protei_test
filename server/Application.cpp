#include "Application.h"
#include "../common/Protocol.h"

#include <iostream>


Application::~Application()
{
    delete m_server;
}

int Application::exec()
{
    m_server = new Server();
    unsigned short protocol = getProtocolType();
    m_server->setProtocolType(protocol);
    int status = m_server->run();
    return status;
}

unsigned short Application::getProtocolType() const
{
    int type = 0;
    bool correct = false;
    while(!correct)
    {
        std::cout << UDP << " - UDP\n";
        std::cout << TCP << " - TCP\n";
        std::cout << "Please, select type [" << FIRST << "-" << (COUNT - 1) << "]:";
        std::cin >> type;
        if(type >= FIRST && type < COUNT)
        {
            correct = true;
        }      
    }

    return type;

}
