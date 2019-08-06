#pragma once
#include "header.h"

void print(const u_char* str, int size);

namespace getinfo{
u_char* get_my_ipv4_address();
u_char* get_my_mac_address(const char* Dev);
}

namespace arpcd{
bool IsReplyPacket(const u_char* packet, const u_char* mac);
bool IsArp(const u_char* packet);
bool IsReply(const u_char* packet);
bool UCharCmp(const u_char* dest, const u_char* src, const int size);
}

namespace conv{
u_char* ipv4_to_hex(const char* cAddress);
u_char* mac_to_hex(const char* cAddress);
int hex_str_to_int(char* hex_str);
}
