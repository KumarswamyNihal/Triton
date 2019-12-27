#include<stdio.h>
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define PACKET_START_BYTE  0xAA

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
int main()
{
	//const unsigned char PACKET_START_BYTE = 0xAA;
	int ser_dev, count = 0, packetSize = 0;
	size_t res;
	struct termios my_serial;
	unsigned char ob[50];
	unsigned char buffer[255];
	ser_dev = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

	bzero(&my_serial, sizeof(my_serial));
	my_serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tcflush(ser_dev, TCIFLUSH);
	tcsetattr(ser_dev, TCSANOW, &my_serial);
		
		while(1){
			if(ser_dev != -1){
				res = read(ser_dev, ob, 1);

				if(res == 0)
				usleep(1000);
				
				else{//TODO: VALIDATE
					if(count == 0 && ob[0] == PACKET_START_BYTE)
					{
						buffer[count] = ob[0];
						count++;
						continue;
					}
					else if(count == 0)
					continue;

					else if(count == 1){
						buffer[count] = ob[0];
						packetSize = ob[0];
						count++;
						continue;
					}
					else if(count<packetSize){
						buffer[count] = ob[0];
						count++;
					}
					if(count >= packetSize){
						if(validatepacket(packetSize, buffer) == 1){
							//Received a packet
							printf("%d\n", buffer[6]<<8);
						}
						else
						{
							printf("VALIDATE FAILED\n");
						}
						
						count = 0;
						packetSize = 0;
					}
				}
			}
			else
			{
				usleep(100000);	//Sleep between threads. Need to decrease time
			}
		}

	return 0;
}
