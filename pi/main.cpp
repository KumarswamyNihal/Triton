#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "chat_client.h"

#define PACKET_START_BYTE  0xAA

int main(int argc, char* argv[])
{
    int ser_dev, count = 0, packetSize = 0;
	size_t res;
	struct termios my_serial;
	unsigned char ob[50];
	unsigned char buffer[255];
	
    /*Opening serial port*/
    ser_dev = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	bzero(&my_serial, sizeof(my_serial));
	my_serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tcflush(ser_dev, TCIFLUSH);
	tcsetattr(ser_dev, TCSANOW, &my_serial);

    /*Starting ASIO*/
    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], "5000");
    chat_client c(io_context, endpoints);
    std::thread t([&io_context](){ io_context.run(); });

    while(1){
			//char buff[10];
							/* buff[0] = 0xAA;
							buff[1] = 8;
							buff[2] = 0x50;
							buff[3] = 0x70;
							buff[4] = 0x00;
							buff[5] = 0x6F;
							buff[6] = 0x00;
							buff[7] = 0xED;
							write(ser_dev, buff, 8); */
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
							//printf("%d\n", buffer[1]);
							 chat_message msg;
      						msg.body_length(packetSize);
      						strncpy(msg.body(), (const char*)buffer, packetSize);
							//printf("buffer is: %c\n", buffer[4]);
      						msg.encode_header();
      						c.write(msg); 
							
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