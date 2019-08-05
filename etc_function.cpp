#include "etc_function.h"
#include <unistd.h>
#include <fcntl.h>

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

void print(const u_char* str, int size){
    for(int i=0;i<size;i++){
        printf("%02x ", str[i]);
    }
    printf("\n");
}


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
        mac = mac_to_hex(buf);
        close(fd);
        return mac;
    }
}
