#include <iostream>
#pragma once

#define PACKET_START_BYTE  0xAA

/*Verifies if a packet is a legitimate packet*/
int validatepacket(int packetSize, unsigned char *buffer);

/*Does packet contain pressure readings*/
int containsP(int packetSize, unsigned char *buffer);

/*Package pressure values in ASIO format*/
char* encode_asio(int packetSize, unsigned char *buffer);
