
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
#include <termios.h>

#include "libmled.h"

#define DRIVER_NAME             "/dev/cnmled"
#define MAX_COLUMN_NUM  5
// 0 ~ 9
const unsigned short NumData[10][MAX_COLUMN_NUM]= // 숫자 제어하는 5*9 제어가 필
{
        {0xfe00,0xfd7F,0xfb41,0xf77F,0xef00}, // 0
        {0xfe00,0xfd42,0xfb7F,0xf740,0xef00}, // 1
        {0xfe00,0xfd79,0xfb49,0xf74F,0xef00}, // 2
        {0xfe00,0xfd49,0xfb49,0xf77F,0xef00}, // 3
        {0xfe00,0xfd0F,0xfb08,0xf77F,0xef00}, // 4
        {0xfe00,0xfd4F,0xfb49,0xf779,0xef00}, // 5
        {0xfe00,0xfd7F,0xfb49,0xf779,0xef00}, // 6
        {0xfe00,0xfd07,0xfb01,0xf77F,0xef00}, // 7
        {0xfe00,0xfd7F,0xfb49,0xf77F,0xef00}, // 8
        {0xfe00,0xfd4F,0xfb49,0xf77F,0xef00}  // 9
};

static mledtext context;

void mled_init() // 초기화
{
        context.fd = open(MLED_DRIVER_PATH,0_RDWR);
};

void mled_destory() // 제거
{
        close(context.fd);
};

int kbhit(void) //현제 key 값이 들어왔는지 확인
{
        struct timeval tv;
        fd_set rdfs;

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO , &rdfs);

        select(STDIN_FILENO + 1 , &rdfs , NULL, NULL, &tv);

        return FD_ISSET(STDIN_FILENO , &rdfs);
}

#define ONE_LINE_TIME_U         1000
void num_of_mled(meldtext* context ,int num) //숫자를 1초동안 출력함
{
	int timeS = 1;
        int cSelCounter,loopCounter;
        int highChar , lowChar;
        int temp , totalCount ;
        unsigned short wdata[2];

        temp = num % 100;

        highChar = temp / 10; // 10의 자리
        lowChar = temp % 10;// 1의 자리


        totalCount = timeS*(1000000 / ONE_LINE_TIME_U); //totalCount = 1000
        printf("totalcounter: %d\n",totalCount); //totalCount = 1000
        cSelCounter = 0;
        loopCounter = 0;
        while(1)// 두개의 5*8 dot에 순차적으로 해당 숫자를 만들기 제어
        {
                // high byte display 
                wdata[0] = NumData[highChar][cSelCounter];

                // low byte display 
                wdata[1] = NumData[lowChar][cSelCounter];

                write(context->fd,(unsigned char*)wdata,4);

                cSelCounter++;
                if ( cSelCounter >= (MAX_COLUMN_NUM-1)) // 5번 돌면 다시 첫번째 부터 다시 시작
                        cSelCounter = 1;

                usleep(ONE_LINE_TIME_U);
                loopCounter++;
                if ( loopCounter > totalCount )
                        break;

               if (kbhit()) // 키가 인식되었는지 확인
               {
                        if ( getchar() == (int)'q') //'q'면 바로 종
                        {

                                wdata[0]= 0;
                                wdata[1]= 0;
                                write(context->fd,(unsigned char*)wdata,4);
                                printf("Exit mledtest\n");
                        }

                }

        }
        wdata[0]= 0;
        wdata[1]= 0;
        write(context->fd,(unsigned char*)wdata,4);
}

