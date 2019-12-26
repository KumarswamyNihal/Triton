#include<stdio.h>
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main()
{
	int ser_dev;
	struct termios my_serial;
	unsigned char ob[50];
	ser_dev = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

	bzero(&my_serial, sizeof(my_serial));
	my_serial.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tcflush(ser_dev, TCIFLUSH);
		tcsetattr(ser_dev, TCSANOW, &my_serial);
		while(1){
		read(ser_dev, ob, 1);
		printf("%d\n", ob[0]);
		usleep(1000);
		}
}
