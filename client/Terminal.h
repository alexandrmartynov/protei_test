#ifndef TERMINAL_H
#define TERMINAL_H

#include "Client.h"

class Terminal
{
public:
    explicit Terminal() = default;
    ~Terminal();
    int exec(int protocol);
private:
    Client* m_client;
};

#endif
