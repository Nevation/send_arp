#pragma once
#include "header.h"

struct arp_spoofing{
public:
    arp_spoofing();
    ~arp_spoofing();
    arp_spoofing(const char* dev, const char* sender, char* target);
    arp_spoofing(const char* dev, u_char* sender, u_char* target,
                 struct arp_packet* request, struct arp_packet* reply, struct arp_packet* attack);
    void ExecuteArpSpoofing();
private:
    char* Dev;
    char* AttackerIp;
    u_char* SenderIp;
    u_char* TargetIp;
    struct arp_packet* RequestPacket;
    struct arp_packet* ReplyPacket;
    struct arp_packet* AttackPacket;

    void MakeRequestPacket(Address* attacker);
    void MakeAttackPacket();
    u_char* GetSenderIp();
    u_char* GetTargetIp();
    struct arp_packet* GetRequestPacket();
    struct arp_packet* GetReplyPacket();
    struct arp_packet* GetAttackPacket();
};

