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

static busledContext context;

void busled_init()//초기화
{
	context.fd = open(LED_DRIVER_PATH,O_RDWR);
}

void busled_destroy()//제거
{
	close(context.fd);
}

void severity_display( unsigned short severity )//중증도 분류해서 값 반환 중증도가 1인 경우 0000 0001 반환하고 8인경우 1000 0000반환
{					//1에서는 점등되고 0에서는 소등	
	unsigned short  wdata=1;//busled에 뿌릴 값
	wdata <<=(severity-1);
	write(context.fd,&wdata,4);
}

