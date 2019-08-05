#include "hardware.h"

Hardware::Hardware() {
    HardwareType = new u_char[U_CHAR_SIZE_2];
}
Hardware::~Hardware() {
    delete[] HardwareType;
}
Hardware::Hardware(const u_char* type, const u_char size){
    HardwareType = new u_char[U_CHAR_SIZE_2];
    memcpy(HardwareType, type, U_CHAR_SIZE_2);
    HardwareSize = size;
}
Hardware::Hardware(const u_char* packet){
    HardwareType = new u_char[U_CHAR_SIZE_2];
    memcpy(HardwareType, &packet[0], 2);
    HardwareSize = packet[4];
}
u_char* Hardware::GetHardwareType() {
    return HardwareType;
}
u_char Hardware::GetHardwareSize() {
    return HardwareSize;
}
