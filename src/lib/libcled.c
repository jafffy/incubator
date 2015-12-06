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

#include "libcled.h"

#define CLED_DRIVER_PATH		"/dev/cncled"

struct cledContext context;

//choice one of 3 colors then, put them to cled_write 
unsigned short colorRed[4]={0,50,0,0};//255 0 0 is red, 255 255 0 is yellow, 0 0 255 is blue
unsigned short colorYellow[4]={0,50,50,0};
unsigned short colorBlue[4]={0,0,0,50};

void cled_init()
{
	context.fd = open(CLED_DRIVER_PATH,O_RDWR);	
};

void cled_destroy()
{
	close(context.fd);
};

void cled_write(unsigned short colorArray[4])
{
	write(context.fd,colorArray,6);	
};

int main(int argc , char **argv)
{
	cled_init();
	
	cled_write(colorYellow);
	cled_destroy();
	
	return 0;
}
