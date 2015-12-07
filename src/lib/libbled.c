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
#include "libbled.h"


#define LED_DRIVER_PATH		"/dev/cnled"

struct busledContext context;

void busled_init()//초기화
{
	context.fd = open(LED_DRIVER_PATH,O_RDWR);
}

void busled_destroy()//제거
{
	close(context.fd);
}

void severity_classified(unsigned char severity )//severity가 3이면 왼쪽부터 3개 led 점등된다.
{					//1에서는 점등되고 0에서는 소등	
	unsigned char  temp = 1;
	//	read(context.fd,&rdata,4);	
		temp = (temp << severity)-1;
		write(context.fd,&temp,4);
}



int main(int argc, char **argv)
{
	
	busled_init();
	severity_classified(4);
	busled_destroy();
	
	return 0;
}
