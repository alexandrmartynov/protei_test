#include "Terminal.h"

Terminal::~Terminal()
{
    delete m_client;
}

int Terminal::exec(int protocol)
{
    m_client = new Client();
    m_client->setProtocolType(protocol);
    int status = m_client->run();
    return status;
}
