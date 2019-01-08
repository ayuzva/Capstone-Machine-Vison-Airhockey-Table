#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <stdint.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main()
{
	int serial_port;
	unsigned int xcoord;
	unsigned int ycoord;
	char payload;

	if((serial_port = serialOpen("/dev/ttyAMA0", 1000000)) < 0)
	{
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		return 1;
	}

	if(wiringPiSetup() == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1;
	}

	while(1){
			printf("Sending Coordinate...\n");

			xcoord = 63000;
			ycoord = 62000;

			serialPutchar(serial_port, 'm');

			payload = (xcoord >> (8*0)) & 0xff;
			serialPutchar(serial_port, payload);

			payload = (xcoord >> (8*1)) & 0xff;
			serialPutchar(serial_port, payload);

			payload = (ycoord >> (8*0)) & 0xff;
			serialPutchar(serial_port, payload);

			payload = (ycoord >> (8*1)) & 0xff;
			serialPutchar(serial_port, payload);

			serialPutchar(serial_port, 'n');

			exit(0);
}

exit(0);
}
