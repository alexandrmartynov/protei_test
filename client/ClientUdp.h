#ifndef CLIENTUDP_H
#define CLIENTUDP_H

class ClientUdp
{
public:
    explicit ClientUdp() = default;
    int exec(int port);
};

#endif
