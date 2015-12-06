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



#define LED_ON	1
#define LED_OFF	0
#define MAX_LED_NO		8
#define LED_DRIVER_PATH		"/dev/cnled"

typedef struct{
	int fd;

}busledContext;

busledContext context;

void busled_init()//초기화
{
	context.fd = open(LED_DRIVER_PATH,O_RDWR);
}

void busled_destroy()//제거
{
	close(context.fd);
}

void severity_classified(unsigned char severity )
{					//1에서는 점등되고 0에서는 소등	
	unsigned char  temp = 1;
	//	read(context.fd,&rdata,4);	
		temp = (temp << severity)-1;
		write(context.fd,&temp,4);
}


void doHelp(void)
{
	printf("Usage:\n");
	printf("ledtest <ledNo> <1(on)|0(off)>\n");
	printf("ex) ledtest 3 1 ;3th led on\n");
	printf("    ledtest 4 0 ;4th led off\n");
	printf("    ledtest 0 0 ;all led off\n");
	printf("    ledtest 0 1 ;all led on\n");
	printf("Max LedNo =>8 \n");  
}

int main(int argc, char **argv)
{
	
	busled_init();
	severity_classified(4);
	busled_destroy();
	
	return 0;
}
