#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#define TRUE 1
#define FALSE 0

struct keypad_context
{
	int fd;
	int rdata;
};

void init_keypad();

int keypad_isPressed(int num);

void destroy_keypad();

#endif
