#ifndef LIBTLCD_H_
#define LIBTLCD_H_

typedef struct{
	int fd;
}tlcdtext;

void tlcd_init();
void tlcd_destory();
int ISBusy(void);
int writeCmd(unsigned short cmd);
int setDDRAMAddr(int x , int y);
int writeCh(unsigned short ch);
int setCursorMode(int bMove , int bRightDir);
int setDDRAMAddr(int x , int y);
int writeStr(char* str);
int clearScreen(int nline);

#endif
