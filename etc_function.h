#pragma once
#include "header.h"

u_char* ipv4_to_hex(const char* cAddress);
u_char* mac_to_hex(const char* cAddress);
u_char* get_my_mac_address(const char* Dev);
int hex_str_to_int(char* hex_str);
void print(const u_char* str, int size);
