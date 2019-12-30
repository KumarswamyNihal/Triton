#include <iostream>
#pragma once

#define PACKET_START_BYTE  0xAA

/*Verifies if a packet is a legitimate packet*/
int validatepacket(int packetSize, unsigned char *buffer);

/*Does packet contain pressure readings*/
int containsP(int packetSize, unsigned char *buffer);

/*Does packet contain valve state change from master */
int containsV(int packetSize, unsigned char *buffer);
