#ifndef PROTOCOL_H
#define PROTOCOL_H

enum Protocol: unsigned short
{
    UDP,
    TCP,

    COUNT,
    FIRST = UDP
};

#endif
