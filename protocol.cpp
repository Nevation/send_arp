#include "protocol.h"

Protocol::Protocol() {
    ProtocolType = new u_char[U_CHAR_SIZE_2];
}
Protocol::~Protocol() {
    delete[] ProtocolType;
}
Protocol::Protocol(u_char* type, u_char size) {
    ProtocolType = new u_char[U_CHAR_SIZE_2];
    memcpy(ProtocolType, type, U_CHAR_SIZE_2);
    ProtocolSize = size;
}
Protocol::Protocol(const u_char* packet){
    ProtocolType = new u_char[U_CHAR_SIZE_2];
    memcpy(ProtocolType, &packet[0], 2);
    ProtocolSize = packet[3];
}
u_char* Protocol::GetProtocolType() {
    return ProtocolType;
}
u_char Protocol::GetProtocolSize() {
    return ProtocolSize;
}
