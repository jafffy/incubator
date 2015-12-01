#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>
#include "libarduino.h"

#define FALSE 0
#define TRUE 1
#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyACM0"
#define _POSIX_SOURCE 1

static struct arduino_context arduino_context;

void *arduino_loop()
{
}
void init_arduino()
{
	arduino_context.temp = 32.5;
	arduino_context.humid = 65.4;
	arduino_context.photo = 39;
	arduino_context.CDS = 340;
	arduino_context.BPM = 72;
}

void destroy_arduino()
{
}

float read_temp()
{
	return arduino_context.temp;
}

float read_humid()
{
	return arduino_context.humid;
}

int read_photo()
{
	return arduino_context.photo;
}

int read_cds()
{
	return arduino_context.CDS;
}

int read_bpm()
{
	return arduino_context.BPM;
}

int is_sound()
{
	return 0;
}
