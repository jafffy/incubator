
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

#define MLED_DRIVER_PATH            "/dev/cnmled"
#define MAX_COLUMN_NUM  2

#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2

// 0 ~ 7
const unsigned short NumData_first[8][MAX_COLUMN_NUM]=
{
        {0xfe00,0xfd00}, // 0
        {0xfe30,0xfd30}, // 1
        {0xfe0C,0xfd0C}, // 2
        {0xfe3C,0xfd3C}, // 3
        {0xfe03,0xfd03}, // 4
        {0xfe33,0xfd33}, // 5
        {0xfe0F,0xfd0F}, // 6
        {0xfe3F,0xfd3F}  // 7
};

const unsigned short NumData_second[8][MAX_COLUMN_NUM]=
{
        {0xfb00,0xf700}, // 0
        {0xfb30,0xf730}, // 1
        {0xfb0C,0xf70C}, // 2
        {0xfb3C,0xf73C}, // 3
        {0xfb03,0xf703}, // 4
        {0xfb33,0xf733}, // 5
        {0xfb0F,0xf70F}, // 6
        {0xfb3F,0xf73F}  // 7
};

const unsigned short NumData_third[8][MAX_COLUMN_NUM]=
{
        {0xef00,0xfe00}, // 0
        {0xef30,0xfe30}, // 1
        {0xef0C,0xfe0C}, // 2
        {0xef3C,0xfe3C}, // 3
        {0xef03,0xfe03}, // 4
        {0xef33,0xfe33}, // 5
        {0xef0F,0xfe0F}, // 6
        {0xef3F,0xfe3F}  // 7
};

const unsigned short NumData_four[8][MAX_COLUMN_NUM]=
{
        {0xfd00,0xfb00}, // 0
        {0xfd30,0xfb30}, // 1
        {0xfd0C,0xfb0C}, // 2
        {0xfd3C,0xfb3C}, // 3
        {0xfd03,0xfb03}, // 4
        {0xfd33,0xfb33}, // 5
        {0xfd0F,0xfb0F}, // 6
        {0xfd3F,0xfb3F}  // 7
};

const unsigned short NumData_five[8][MAX_COLUMN_NUM]=
{
        {0xf700,0xef00}, // 0
        {0xf730,0xef30}, // 1
        {0xf70C,0xef0C}, // 2
        {0xf73C,0xef3C}, // 3
        {0xf703,0xef03}, // 4
        {0xf733,0xef33}, // 5
        {0xf70F,0xef0F}, // 6
        {0xf73F,0xef3F}  // 7
};

static struct mledtext context;

void mled_init() // initialization
{
        context.fd = open(MLED_DRIVER_PATH,O_RDWR);
	
	if(context.fd < 0)
	{
		perror("driver open error.\n");
	}
}

void mled_destory() // destory
{
        close(context.fd);
}


#define ONE_LINE_TIME_U         1000
void mled_display(unsigned int num)// 10 sec print
{
	int i, j;
	int timeS, totalCount;
        int final_position,energization, cnt, mul;
        int cSelCounter,loopCounter;
	unsigned int temp;
	int sumBuf[5] = {0, 0, 0, 0, 0};
	unsigned short wdata[2];

	timeS = 3;
	totalCount = timeS*(1000000 / ONE_LINE_TIME_U);
	
        loopCounter = 0;

        temp = num;
	mul = 4;

        for(i = 0; i < 3; i++)  // Virtual Array
        {
                for(j = 0; j < 5; j++)
                {
                        final_position = temp & 0x01; // final bit extract 
                        temp = temp >> 1; //right bit operation of temp
			sumBuf[j] = sumBuf[j] + (final_position * mul);  // final bit sum
		}
	       	mul = mul / 2; //mul => 4 => 2 => 1
	 }
	while(1){
        	for(i = 0; i < 5; i++)
        	{
			cSelCounter = 0;
                	wdata[0] = 0; // wdata[0] initialization
                	wdata[1] = 0; // wdata[1] initialization
                	while(1) // write process
                	{
                        	if(i == 0) wdata[0] = NumData_first[sumBuf[i]][cSelCounter]; // i = 0 : first column
                        	else if(i == 1) wdata[0] = NumData_second[sumBuf[i]][cSelCounter]; // i = 1 : second column
                        	else if(i == 2) // i = 2 : third column
                        	{
                                	if(cSelCounter == 0) // if cSelCounter = 0, left dot matrix
                                        	wdata[0] = NumData_third[sumBuf[i]][cSelCounter];
                                	else // if cSelCounter = 1, right dot matrix
                                	        wdata[1] = NumData_third[sumBuf[i]][cSelCounter];
                       	 	}
                       		else if(i == 3) wdata[1] = NumData_four[sumBuf[i]][cSelCounter]; // i = 3 : four column
                       		else wdata[1] = NumData_five[sumBuf[i]][cSelCounter]; // i = 4 : five column
                        
				write(context.fd, (unsigned char*)wdata, 4); // write : wdata array

        	                cSelCounter++; // column ++
				usleep(ONE_LINE_TIME_U);

                        	if ( cSelCounter == 2) break;
                	}
		}
	loopCounter++;
        if(loopCounter > totalCount) break;
	}
}

int main(int argc, char **argv)
{
        unsigned int num = 146;
	//open driver
	
	mled_init();

	mled_display(num);

	mled_destory();
	return 1;
}
   
