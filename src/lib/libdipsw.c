i#include <stdio.h>
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

static dipswContext context;

void dipsw_init(){
	context.fd = open(DIPSWITCH_PATH, O_RDWR);
};

void dipsw_destroy(){
	close(context.fd);   
};

int num_of_dipsw(){//return what number is on 

	int shiftvalue;
 	unsigned short temp = 1;

	read(context.fd,&shiftvalue,4);
	temp = temp << shiftvalue;

	return temp;
};


