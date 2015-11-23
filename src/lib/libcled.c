#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <libcled.h>

#define CLED_DRIVER_PATH		"/dev/cncled"

static cledContext context;

//choice one of 3 colors then, put them to cled_write 
unsigned short colorRed[3]={255,0,0};//255 0 0 is red, 255 255 0 is yellow, 0 0 255 is blue
unsigned short colorYellow[3]={255,255,0};
unsigned short colorBlue[3]={0,0,255};

void cled_open()
{
	context.fd = open(CLED_DRIVER_PATH,O_RDWR);	
};

void cled_destory()
{
	close(context.fd);
};

void cled_write(unsigned short* colorArray)
{
	write(context.fd,&colorArray,6);	
};


