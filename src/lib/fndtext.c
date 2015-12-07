#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "libfnd.h"
/*
                a               
        f               b
                g               
        e               c       
                d               dp


*/
#define MLED_DRIVER_PATH            "/dev/cnfnd"

#define MAX_FND_NUM             6

#define  DOT_OR_DATA    0x80 // bit data value of dot

const unsigned short segNum[10] = //Segment bit combination for the numbers
{
        0x3F, // 0
        0x06,
        0x5B,
        0x4F,
        0x66,
        0x6D,
        0x7D,
        0x27,
        0x7F,
        0x6F  // 9
};
const unsigned short segSelMask[MAX_FND_NUM] = //6개의 세그먼트의 active 값
{
        0xFE00,
        0xFD00,
        0xFB00,
        0xF700,
        0xEF00,
        0xDF00
};

static struct termios oldt, newt;
static struct fndtext context;

void fnd_init() // 초기화
{
        context.fd = open(MLED_DRIVER_PATH,O_RDWR);

	if(context.fd < 0)
	{
		perror("driver open error.\n");
	}
}

void fnd_destory() // 제거
{
        close(context.fd);
}

void changemode(int dir) // 사용자 terminal 입력 방향 변경
{
        if( dir == 1) //프로그램으로 키 입력 변경
        {
                tcgetattr(STDIN_FILENO , &oldt); //터미널의 속성 얻어옴
                newt = oldt;
                newt.c_lflag &= ~(ICANON | ECHO ); //터미널 속성 변경
                tcsetattr(STDIN_FILENO, TCSANOW, &newt); //변경된 속성 즉시(TCSANOW) 적용
        }
        else // 예전 방향으로 키 입력을 돌려 놓음
        {
                tcsetattr(STDIN_FILENO , TCSANOW, &oldt);

        }
}

int changeNum(int num) // sec -> time
{
	int wdata, hour, min, sec;
	int setNum;
	
	setNum = num;

	hour = setNum / 3600;
	setNum = setNum % 3600;
	min = setNum / 60;
	sec = setNum / 60;

	wdata = hour * 10000 + min * 100 + sec * 1;
	return wdata;
}

#define ONE_SEG_DISPLAY_TIME_USEC       1000

void fndDisp(int num) //remain drug time
{
	int durationtime = 3;
	int setNumber, i;
	int dotflag = 0b1010;
        int cSelCounter,loopCounter;
        int temp , totalCount;
        unsigned short wdata;
        int dotEnable[MAX_FND_NUM];
        int fndChar[MAX_FND_NUM];

	setNumber = num;
	
	for(i = 0 ; i< MAX_FND_NUM ; i++)
	{
		dotEnable[i] = dotflag & (0x1 << i);
	}	

	//if 6 fnd
	temp = setNumber % 1000000;
        fndChar[0]= temp /100000;

        temp = num % 100000;
        fndChar[1]= temp /10000;

        temp = num % 10000;
        fndChar[2] = temp /1000;

        temp = num %1000;
        fndChar[3] = temp /100;

        temp = num %100;
        fndChar[4] = temp /10;

        fndChar[5] = setNumber %10;

        totalCount = durationtime*(1000000 / ONE_SEG_DISPLAY_TIME_USEC);
        
	cSelCounter = 0;
        loopCounter = 0;
	
	changemode(1);
	while(1)
        {
                wdata = segNum[fndChar[cSelCounter]]  | segSelMask[cSelCounter] ;
                if(dotEnable[cSelCounter])
			wdata |= DOT_OR_DATA;

		write(context.fd,&wdata,2);

                cSelCounter++;
                if ( cSelCounter >= MAX_FND_NUM )
                        cSelCounter = 0;

                usleep(ONE_SEG_DISPLAY_TIME_USEC);

                loopCounter++;
                if ( loopCounter > totalCount )
                        break;
        }

        wdata = 0;
	write(context.fd, &wdata, 2);
	changemode(0);
}

int main(int argc, char **argv)
{
	int num = 654321;

	//open driver
	
	fnd_init();
	
	fndDisp(num);

	fnd_destory();
}
