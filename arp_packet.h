#pragma once
#include "header.h"

struct arp_packet
{
public:
    arp_packet();
    ~arp_packet();
    arp_packet(const u_char* packet);
    arp_packet(struct ethernet_header* eth, struct arp_header* arp);
    struct ethernet_header* GetEthernet();
    struct arp_header* GetArp();
    u_char* ToPacket();
private:
    struct ethernet_header* Ether;
    struct arp_header* Arp;
};
