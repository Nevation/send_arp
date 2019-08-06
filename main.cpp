#include "header.h"

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Usage: ./send_arp <interface> <sender ip> <target ip>\n");
        return -1;
    }

    struct arp_spoofing* ArpSpoofing = new arp_spoofing(argv[1], argv[2], argv[3]);
    ArpSpoofing->ExecuteArpSpoofing();
    delete ArpSpoofing;
    return 0;
}
