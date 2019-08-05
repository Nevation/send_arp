#include "arp_packet.h"

arp_packet* arp_spoofing::GetRequestPacket(){
    return RequestPacket;
}
arp_packet* arp_spoofing::GetReplyPacket(){
    return ReplyPacket;
}
arp_packet* arp_spoofing::GetAttackPacket(){
    return AttackPacket;
}

arp_packet::arp_packet(const u_char* packet){
    Ether = new ethernet_header(&packet[0], &packet[6], &packet[12]);
    Arp = new arp_header(&packet[14]);
}

arp_packet::arp_packet() {
    Ether = new ethernet_header();
    Arp = new arp_header();
}
arp_packet::~arp_packet() {
    delete Ether;
    delete Arp;
}

arp_packet::arp_packet(struct ethernet_header* eth, struct arp_header* arp) {
    Ether = new ethernet_header(eth->GetDestionation(), eth->GetSoruce(), eth->GetProtocol());
    Arp = new arp_header(arp->GetHardware(), arp->GetProtocol(),
        arp->GetOpcode(), arp->GetSenderAddress(), arp->GetTargetAddress());
}
struct ethernet_header* arp_packet::GetEthernet() {
    return Ether;
}
struct arp_header* arp_packet::GetArp() {
    return Arp;
}

u_char* arp_packet::ToPacket() {
    u_char* packet = new u_char[42];
    memcpy(&packet[0], Ether->ToPacket(), 14);
    memcpy(&packet[14], Arp->ToPacket(), 28);
    return packet;
}
