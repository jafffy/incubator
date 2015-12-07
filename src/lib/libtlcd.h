#ifndef LIBTLCD_H_
#define LIBTLCD_H_

struct tlcdtext{
	int fd;
};

void tlcd_init();
void tlcd_destory();

static int ISBusy();
static int writeCmd(unsigned short cmd);
static int setDDRAMAddr(int x , int y);
static int displayMode(int bCursor, int bCursorblink, int blcd);
static int writeCh(unsigned short ch);
static int setCursorMode(int bMove , int bRightDir);
static int functionSet();

void setWrite(char* str);
int clearScreen();
void setting();
#endif
