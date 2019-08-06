#include "etc_function.h"
#include <unistd.h>
#include <fcntl.h>

namespace conv{
u_char* ipv4_to_hex(const char* cAddress){
    int index = 0;
    unsigned long StartIndex = 0;
    u_char* uAddress = new u_char[IP_SIZE];
    size_t SenderLen = strlen(cAddress);
    for(size_t i=0; i <= SenderLen; i++){
        if(cAddress[i] == '.' || i == SenderLen){
            char StringNumber[3] = {0, };
            memcpy(StringNumber ,&cAddress[StartIndex], i - StartIndex);
            StartIndex = i + 1;
            uAddress[index++] = static_cast<u_char>(atoi(StringNumber));
        }
    }
    return uAddress;
}

u_char* mac_to_hex(const char* cAddress){
    int index = 0;
    unsigned long StartIndex = 0;
    u_char* uAddress = new u_char[IP_SIZE];
    size_t SenderLen = strlen(cAddress);
    for(size_t i=0; i <= SenderLen; i++){
        if(cAddress[i] == ':' || i == SenderLen){
            char StringNumber[2] = {0, };
            memcpy(StringNumber ,&cAddress[StartIndex], i - StartIndex);
            StartIndex = i + 1;
            uAddress[index++] = static_cast<u_char>(hex_str_to_int(StringNumber));
        }
    }
    return uAddress;
}

int hex_str_to_int(char* hex_str){
    int result = 0;

    int mul[2] = {16, 1};
    for(int i=0;i<2;i++){
        char hex = hex_str[i];
        if(hex >= 48 && hex < 58){
            result += (hex - 48) * mul[i];
        } else if (hex >=65 && hex < 71){
            result += (hex - 55) * mul[i];
        } else if (hex >=97 && hex < 103){
            result += (hex - 87) * mul[i];
        } else{
            return -1;
        }
    }
    return result;
}
}

namespace getinfo {
u_char* get_my_mac_address(const char* Dev){
    u_char* mac = new u_char[MAC_SIZE];
    char buf[18] = {0, };
    char path[64];
    sprintf(path, "/sys/class/net/%s/address", Dev);

    int fd = open(path, O_RDONLY);
    if(read(fd, buf, 17) == -1){
        close(fd);
        return nullptr;
    } else {
        mac = conv::mac_to_hex(buf);
        close(fd);
        return mac;
    }
}

u_char* get_my_ipv4_address(){
    system("hostname -I > get_my_ipv4_address.jsjsjs");
    int fd = open("get_my_ipv4_address.jsjsjs", O_RDONLY);
    char buf[40] = {0, };
    if (read(fd, buf, 40) == -1){
        return nullptr;
    }
    close(fd);
    system("rm get_my_ipv4_address.jsjsjs");
    return conv::ipv4_to_hex(buf);
}
}

namespace arpcd{
bool IsReplyPacket(const u_char* packet, const u_char* mac){
    if (arpcd::IsArp(&packet[12]) && arpcd::UCharCmp(&packet[0], mac, MAC_SIZE)){
        if (arpcd::IsReply(&packet[20])) {
            return true;
        }
    }
    return false;
}
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
}

void print(const u_char* str, int size){
    for(int i=0;i<size;i++){
        printf("%02x ", str[i]);
    }
    printf("\n");
}

