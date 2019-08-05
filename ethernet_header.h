#pragma once
#include "header.h"


struct ethernet_header
{
public:
    ethernet_header();
    ~ethernet_header();
    ethernet_header(const u_char* dest, const u_char* source, const u_char* protocol);
    ethernet_header(const u_char* packet);
    u_char* GetDestionation();
    u_char* GetSoruce();
    u_char* GetProtocol();
    u_char* ToPacket();
private:
    u_char* Destination;
    u_char* Source;
    u_char* Protocol;
};
