#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "chat_client.h"
#include "logger.h"
#include "packets.h"
#include "global.h"

int main(int argc, char* argv[])
{
    int count = 0, packetSize = 0;
	size_t res;
	struct termios my_serial;
    logger *serial_rec;
	unsigned char ob[50];
	unsigned char buffer[255];
	
    /*Initializing logger objects with filenames*/
    serial_rec = new logger("serial_rec.dat");

    /*Opening serial port*/
    ser_dev = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
	bzero(&my_serial, sizeof(my_serial));
	my_serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tcflush(ser_dev, TCIFLUSH);
	tcsetattr(ser_dev, TCSANOW, &my_serial);

    /*Starting ASIO*/
    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], "5000");
    chat_client c(io_context, endpoints, "serial_trans.dat");
    std::thread t([&io_context](){ io_context.run(); });

    while(1){
		if(ser_dev != -1){
			res = read(ser_dev, ob, 1);
				
			if(res == 0){
                //Sleep between characters
				usleep(1000);
            }
				
			else{//TODO: VALIDATE
				if(count == 0 && ob[0] == PACKET_START_BYTE){
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

                        serial_rec->log((char*)buffer, packetSize);
						
                        if(containsP(packetSize, buffer) == 1){
                            /*If packet from serial port contains pressure readings, send via ASIO*/
                            chat_message msg;
                            msg.body_length(packetSize);
                            strncpy(msg.body(), (const char*)buffer, packetSize);
                            msg.encode_header();
                            c.write_asio(msg);
                        }
							
					}
					else{
						printf("VALIDATE FAILED\n");
					}
						
						count = 0;
						packetSize = 0;
				}
			}
		}
		else{
			std::cerr<<"cannot find serial"<<std::endl;
			usleep(100000);	//Sleep between threads. Need to decrease time
			}
	}

    free(serial_rec);

    return 0;
}
