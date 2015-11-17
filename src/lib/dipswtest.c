

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

#define DIPSWITCH_PATH		"/dev/cndipsw"


void doHelp(void)
{
	printf("Usage:\n");
	printf("dipsw \n");
	printf("return  int  (0~ 15 bit are valid). \n");
};

typedef struct {
	short int fd;// dipswitch 파일 디스크립터
	//차후에 전역변수로 쓰일 것들 여기에..
} dipswContext;

void dipsw_init(dipswContext* context){
	context->fd = open(DIPSWITCH_PATH, O_RDWR);
};

void dipsw_destroy(dipswContext* context){
	close(context->fd);   
};

int num_of_dipsw(dipswContext* context){//dipsw가 몇 번이 on되엇는지 
	int retvalue;
 
	read(context->fd,&retvalue,4);
	retvalue &= 0xF;
	printf("retvalue:0x%X\n",retvalue);

	return retvalue;
};

int main(int argc , char **argv)
{
	dipswContext context;
	int retvalue;
	
	if (argc >1 )
	{
		perror(" Args number is more than 1\n");
		doHelp();
		return 1;
	}
	

	// open  driver 
	dipsw_init(&context);
	if ( context.fd < 0 )
	{
		perror("driver  open error.\n");
		return 1;
	}
	retvalue = num_of_dipsw(&context);
	dipsw_destroy(&context);
	
	return retvalue;
}


