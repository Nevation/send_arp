TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        address.cpp \
        arp_header.cpp \
        arp_packet.cpp \
        arp_spoofing.cpp \
        etc_function.cpp \
        ethernet_header.cpp \
        hardware.cpp \
        main.cpp \
        protocol.cpp

HEADERS += \
    address.h \
    arp_header.h \
    arp_packet.h \
    arp_spoofing.h \
    etc_function.h \
    ethernet_header.h \
    hardware.h \
    header.h \
    protocol.h
