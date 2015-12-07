#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "libtlcd.h"

#define TRUE		1
#define FALSE		0

#define SUCCESS		0
#define FAIL		1

#define DRIVER_NAME		"/dev/cntlcd"
/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
#define CLEAR_DISPLAY		0x0001
#define CURSOR_AT_HOME		0x0002

// Entry Mode set 
#define MODE_SET_DEF		0x0004
#define MODE_SET_DIR_RIGHT	0x0002
#define MODE_SET_SHIFT		0x0001

// Display on off
#define DIS_DEF				0x0008
#define DIS_LCD				0x0004
#define DIS_CURSOR			0x0002
#define DIS_CUR_BLINK		0x0001

// shift
#define CUR_DIS_DEF			0x0010
#define CUR_DIS_SHIFT		0x0008
#define CUR_DIS_DIR			0x0004

// set DDRAM  address 
#define SET_DDRAM_ADD_DEF	0x0080

// read bit
#define BUSY_BIT			0x0080
#define DDRAM_ADD_MASK		0x007F


#define DDRAM_ADDR_LINE_1	0x0000
#define DDRAM_ADDR_LINE_2	0x0040


#define SIG_BIT_E			0x0400
#define SIG_BIT_RW			0x0200
#define SIG_BIT_RS			0x0100

/***************************************************
read /write  sequence
write cycle
RS,(R/W) => E (rise) => Data => E (fall)

***************************************************/

#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2
static struct tlcdtext context;

static int bCursorOn;
static int bBlink;
static int nline = 2;
static int nColumn;

void tlcd_init() //초기화
{
	// open driver
	context.fd = open(DRIVER_NAME, O_RDWR);
	if (context.fd < 0)
	{
		perror("driver open error.\n");
	}
}

void tlcd_destory() //제거
{
	close(context.fd);
}

static int IsBusy() // 이전의 명령이 수행중인지 확인
{
	unsigned short wdata, rdata;

	wdata = SIG_BIT_RW;
	write(context.fd, &wdata, 2);

	wdata = SIG_BIT_RW | SIG_BIT_E;
	write(context.fd, &wdata, 2);

	read(context.fd, &rdata, 2);

	wdata = SIG_BIT_RW;
	write(context.fd, &wdata, 2);

	if (rdata &  BUSY_BIT)
		return TRUE;

	return FALSE;
}
static int writeCmd(unsigned short cmd) //TLCD에 명령을 내림
{
	unsigned short wdata;

	if (IsBusy())
		return FALSE;

	wdata = cmd;
	write(context.fd, &wdata, 2);

	wdata = cmd | SIG_BIT_E;
	write(context.fd, &wdata, 2);

	wdata = cmd;
	write(context.fd, &wdata, 2);

	return TRUE;
}

static int setDDRAMAddr(int x, int y){ //커서와 index을 설정
	unsigned short cmd = 0;
	//	printf("x :%d , y:%d \n",x,y);
	if (IsBusy())
	{
		perror("setDDRAMAddr busy error.\n");
		return FALSE;

	}

	if (y == 1)
	{
		cmd = DDRAM_ADDR_LINE_1 + x;
	}
	else if (y == 2)
	{
		cmd = DDRAM_ADDR_LINE_2 + x;
	}
	else
		return FALSE;

	if (cmd >= 0x80)
		return FALSE;


	//	printf("setDDRAMAddr w1 :0x%X\n",cmd);

	if (!writeCmd(cmd | SET_DDRAM_ADD_DEF))
	{
		perror("setDDRAMAddr error\n");
		return FALSE;
	}
	//	printf("setDDRAMAddr w :0x%X\n",cmd|SET_DDRAM_ADD_DEF);
	usleep(1000);
	return TRUE;
}

static int displayMode(int bCursor, int bCursorblink, int blcd)
{
	unsigned short cmd = 0;

	if (bCursor)
	{
		cmd = DIS_CURSOR;
	}

	if (bCursorblink)
	{
		cmd |= DIS_CUR_BLINK;
	}

	if (blcd)
	{
		cmd |= DIS_LCD;
	}

	if (!writeCmd(cmd | DIS_DEF))
		return FALSE;

	return TRUE;
}

static int writeCh(unsigned short ch) // 현재 address index에 한 문자를 씀
{
	unsigned short wdata = 0;

	if (IsBusy())
		return FALSE;

	wdata = SIG_BIT_RS | ch;
	write(context.fd, &wdata, 2);

	wdata = SIG_BIT_RS | ch | SIG_BIT_E;
	write(context.fd, &wdata, 2);

	wdata = SIG_BIT_RS | ch;
	write(context.fd, &wdata, 2);
	usleep(1000);
	return TRUE;

}


static int setCursorMode(int bMove, int bRightDir)// 현제 커서를 앞 뒤로 움직임
{
	unsigned short cmd = MODE_SET_DEF;
	if (bMove)
		cmd |= MODE_SET_SHIFT;

	if (bRightDir) // 커서 깜빡거림 enable
		cmd |= MODE_SET_DIR_RIGHT;

	if (!writeCmd(cmd))
		return FALSE;
	return TRUE;
}

int functionSet() // TLCD 초기화
{
	unsigned short cmd = 0x0038; // 5*8 dot charater , 8bit interface , 2 line

	if (!writeCmd(cmd))
		return FALSE;
	return TRUE;
}

void setWrite(char* str) //현재 address index에 문자 쓰기
{
	unsigned char wdata;
	int i;

	for (i = 0; i < strlen(str); i++)
	{
		if (str[i] == '_')
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		writeCh(wdata);
	}
}

#define LINE_NUM			2
#define COLUMN_NUM			16			
int clearScreen() // line clear 1 or 2
{
	int nline = 2;
	int i;
	if (nline == 0)
	{
		if (IsBusy())
		{
			perror("clearScreen error\n");
			return FALSE;
		}
		if (!writeCmd(CLEAR_DISPLAY))
			return FALSE;
		return TRUE;
	}
	else if (nline == 1)
	{
		setDDRAMAddr(0, 1);
		for (i = 0; i <= COLUMN_NUM; i++)
		{
			writeCh((unsigned char)' ');
		}
		setDDRAMAddr(0, 1);

	}
	else if (nline == 2)
	{
		setDDRAMAddr(0, 2);
		for (i = 0; i <= COLUMN_NUM; i++)
		{
			writeCh((unsigned char)' ');
		}
		setDDRAMAddr(0, 2);
	}
	return TRUE;
}

void setting()
{
	char starting[] = "Name : ";
	
	functionSet();

	bBlink = 1;
	nColumn = 12;

	displayMode(1, bBlink, 1);
	setDDRAMAddr(1, nColumn);

	usleep(2000);

	setWrite(starting);

	setDDRAMAddr(0, 2);
}

int main(int argc, char** argv)
{
	char str[COLUMN_NUM + 1] = {"I_love_jjangman"};
	tlcd_init();

	setting();
    	setWrite(str);
	//clearScreen();
	//setWrite("me too");
	tlcd_destory();
	return 0;
}
