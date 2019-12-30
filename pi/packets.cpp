#include "packets.h"

int validatepacket(int packetSize, unsigned char *buffer)
{
	if(buffer[0] != PACKET_START_BYTE)
	{
		printf("VALIDATE BIG%c\n", buffer[0]);
		return 0;

	}
		
	if(buffer[1] != packetSize)
	{
		
		return 0;
	}

	if(buffer == NULL)
	{
		
		return 0;
	}
	unsigned char checksum = 0x00;
	for(unsigned int i=0; i < packetSize-1; i++)
		checksum = checksum ^ buffer[i];
	
	if(buffer[packetSize-1] != checksum){
		printf("%c", checksum);
		return 0; 
	}

	return 1;
}

int containsP(int packetSize, unsigned char *buffer)
{
	if(packetSize != 8)
		return -1;

	if(buffer[2] != 'P')
		return -1;
	
	return 1;
}
