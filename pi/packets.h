#include <iostream>
#pragma once

#define PACKET_START_BYTE  0xAA
const unsigned char PACKET_OPEN_VALVE[] = {PACKET_START_BYTE, 5, 'V', 'T',PACKET_START_BYTE^5^'V'^'T' };
const unsigned char PACKET_CLOSE_VALVE[] = {PACKET_START_BYTE, 5, 'V', 'F',PACKET_START_BYTE^5^'V'^'F'};

/*Verifies if a packet is a legitimate packet*/
int validatepacket(int packetSize, unsigned char *buffer);

/*Does packet contain pressure readings*/
int containsP(int packetSize, unsigned char *buffer);

/*Does packet contain valve state change from master */
int containsV(int packetSize, unsigned char *buffer);
