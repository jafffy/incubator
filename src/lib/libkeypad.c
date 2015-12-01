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
#include "libkeypad.h"
#define DRIVER_NAME "/dev/cnkey"

static struct keypad_context keypad_context;

void init_keypad()
{
	keypad_context.fd = open(DRIVER_NAME, O_RDWR);
	read(keypad_context.fd, &(keypad_context.rdata), 4);
	return;	
}

int keypad_isPressed(int num)
{
	return num == keypad_context.rdata;
}

void destroy_keypad()
{
	close(keypad_context.fd);
	return;
}

