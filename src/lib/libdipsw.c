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

#include "libdipsw.h"

#define DIPSWITCH_PATH		"/dev/cndipsw"

#ifndef BOOL
#define BOOL int
#endif // BOOL

#ifndef TRUE
#define TRUE 1
#endif // TRUe

#ifndef FALSE
#define FALSE 0
#endif // FALSE

static dipswContext context;

void dipsw_init(){
	context.fd = open(DIPSWITCH_PATH, O_RDWR);
};

void dipsw_destroy(){
	close(context.fd);   
};


BOOL dipsw_test()
{
	int e;
	read(context.fd, &e, 4);

	return e == context.curstate;
}

int dipsw_curstate()
{
	int e;
	read(context.fd, &e, 4);
	return e;
}


int main(int argc , char **argv)
{
	int retvalue;

	

	// open  driver 
	dipsw_init();
	retvalue = dipsw_curstate();
	printf("retvalue:0x%X\n", retvalue);
	dipsw_destroy();
	
	return 0;
}
