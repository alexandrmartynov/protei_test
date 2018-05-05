#include "IOService.h"

#include <iostream>

IOService::IOService():
    m_message({})
{}

void IOService::dialog(Socket* socket)
{
    bool exit = false;
    while(!exit)
    {
        m_message.clear();
        m_message = getNewMessage();
        if(m_message.compare("-exit") == 0)
        {
            exit = true;
        }

        socket->send(m_message);
        m_message = socket->receive();
        std::cout << "echo: " << m_message << std::endl;

    } 
}

std::string IOService::echo(Socket* socket)
{
    m_message.clear();
    m_message = socket->receive();
    socket->send(m_message);

    return m_message;   
}

std::string IOService::getNewMessage() const
{
    std::string message = {};
    std::cout << "For disconnect, please write -exit\n";
    std::cout << "Write message: ";
    std::cin >> message;

    return message;
}

