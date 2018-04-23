#ifndef APPLICATION_H
#define APPLICATION_H

#include "Server.h"

class Application
{
public:
    explicit Application() = default;
    ~Application();
    int exec();
private:
    int getProtocolType() const;
    Server* m_server;
};

#endif
