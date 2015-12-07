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

#include "liboled.h"

#define OLED_DRIVER_PATH	"/dev/cnoled"

#ifndef BOOL
#define BOOL int
#endif // BOOL

#ifndef TRUE
#define TRUE 1
#endif // TRUe

#ifndef FALSE
#define FALSE 0
#endif // FALSE


#define RST_BIT_MASK		0xEFFF		
#define CS_BIT_MASK		0xF7FF
#define DC_BIT_MASK		0xFBFF
#define WD_BIT_MASK		0xFDFF
#define RD_BIT_MASK		0xFEFF
#define DEFAULT_MASK		0xFFFF


#define CMD_SET_COLUMN_ADDR	0x15
#define CMD_SET_ROW_ADDR	0x75
#define CMD_WRITE_RAM		0x5C
#define CMD_READ_RAM		0x5D
#define CMD_LOCK		0xFD

struct oledContext context;

static unsigned short gamma[64]= 
{
0xB8,
0x02, 0x03, 0x04, 0x05,
0x06, 0x07, 0x08, 0x09,
0x0A, 0x0B, 0x0C, 0x0D,
0x0E, 0x0F, 0x10, 0x11,
0x12, 0x13, 0x15, 0x17,
0x19, 0x1B, 0x1D, 0x1F,
0x21, 0x23, 0x25, 0x27,
0x2A, 0x2D, 0x30, 0x33,
0x36, 0x39, 0x3C, 0x3F,
0x42, 0x45, 0x48, 0x4C,
0x50, 0x54, 0x58, 0x5C,
0x60, 0x64, 0x68, 0x6C,
0x70, 0x74, 0x78, 0x7D,
0x82, 0x87, 0x8C, 0x91,
0x96, 0x9B, 0xA0, 0xA5,
0xAA, 0xAF, 0xB4

};


static int writeCmd(int size , unsigned short* cmdArr)//명령 배열?을 받아서 fd에 2byte씩 쓴다.
{
	int i ;
	unsigned short wdata;

	//printf("wCmd : [0x%02X]",cmdArr[0]);
	//wdata = CS_BIT_MASK;
	//write(fd,&wdata,2);

	wdata = CS_BIT_MASK & DC_BIT_MASK;// 1111 0011 1111 1111
	write(context.fd,&wdata,2);

	wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK ; // 1111 0001 1111 1111
	write(context.fd,&wdata,2);

	wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK & (cmdArr[0]|0xFF00) ; // 1111 0001 1111 1111 & (cmdArr[0]|0xff00) 
	write(context.fd,&wdata,2);
	
	wdata = CS_BIT_MASK & DC_BIT_MASK & (cmdArr[0] | 0xFF00) ;
	write(context.fd,&wdata,2);

	wdata = CS_BIT_MASK & ( cmdArr[0] | 0xFF00);
	write(context.fd,&wdata,2);

	for (i = 1; i < size ; i++ )
	{
	//	wdata = CS_BIT_MASK ;
	//	write(fd,&wdata,2);

	//	wdata = CS_BIT_MASK ;
	//	write(fd,&wdata,2);

		wdata = CS_BIT_MASK & WD_BIT_MASK ;
		write(context.fd,&wdata,2);

		wdata = CS_BIT_MASK & WD_BIT_MASK & (cmdArr[i] | 0xFF00) ;
		write(context.fd,&wdata,2);

		wdata = CS_BIT_MASK & (cmdArr[i] | 0xFF00);
		write(context.fd,&wdata,2);

	//	wdata = CS_BIT_MASK & (cmdArr[i] | 0xFF00);
	//	write(fd,&wdata,2);
	//	printf("[0x%02X]",cmdArr[i]);

	}
	wdata= DEFAULT_MASK;
	write(context.fd,&wdata,2);
	//printf("\n");
	return TRUE;
}
static int writeData(int size , unsigned char* dataArr)
{
	int i ;
	unsigned short wdata;
	
	wdata = CS_BIT_MASK & DC_BIT_MASK;
	write(context.fd,&wdata,2);

	write(context.fd,&wdata,2);

	wdata = CS_BIT_MASK & DC_BIT_MASK & (CMD_WRITE_RAM | 0xFF00);
	write(context.fd,&wdata,2);

	wdata = CS_BIT_MASK &  (CMD_WRITE_RAM | 0xFF00);
	write(context.fd,&wdata,2);

	for (i = 0; i < size ; i++ )
	{
		wdata = CS_BIT_MASK & WD_BIT_MASK ;
		write(context.fd,&wdata,2);

		wdata = CS_BIT_MASK & WD_BIT_MASK & ((unsigned char)dataArr[i] | 0xFF00 );
		write(context.fd,&wdata,2);

		wdata = CS_BIT_MASK & ( (unsigned char)dataArr[i] | 0xFF00);
		write(context.fd,&wdata,2);


	}
	wdata = DEFAULT_MASK;
	write(context.fd,&wdata,2);

	return TRUE;

}

// to send cmd  , must unlock
static int setCmdLock(int bLock)
{
	unsigned short  cmd[3];
	
	cmd[0] = CMD_LOCK;            //
	if (bLock)
	{
		cmd[1] = 0x16; // lock
		writeCmd(2,cmd);

	}
	else
	{
		cmd[1] = 0x12; // lock
		writeCmd(2,cmd);

		// A2,B1,B3,BB,BE accessible
		cmd[1] = 0xB1;
		writeCmd(2,cmd);
	}
	return TRUE;
}

void oled_init()//open the device driver
{
	context.fd = open(OLED_DRIVER_PATH,O_RDWR);
}
void oled_destroy()//close the device driver
{
	close(context.fd);
}

void reset()//oled chip reset
{
	unsigned short wdata ;

	wdata = RST_BIT_MASK;// 1110 1111 1111 1111 
	write(context.fd,&wdata , 2 );//wdata의 값을 fd에 2byte를 쓴다.
	usleep(2000);
	wdata = DEFAULT_MASK;// 1111 1111 1111 1111
	write(context.fd,&wdata , 2 );
}

void  setting_init_oled()//set the initial device driver
{
	unsigned short wdata[10];
	unsigned char  wcdata[10];
	int i,j;
	wdata[0]= 0xFD;
	wdata[1] = 0x12;
	writeCmd(2,wdata);
	
	wdata[0] = 0xFD;
	wdata[1] = 0xB1;
	writeCmd(2,wdata);

	wdata[0] = 0xAE;
	writeCmd(1,wdata);

	wdata[0] = 0xB3;
	wdata[1] = 0xF1;
	writeCmd(2,wdata);

	wdata[0] = 0xCA;
	wdata[1] = 0x7F;
	writeCmd(2,wdata);

	wdata[0] = 0xA2;
	wdata[1] = 0x00;
	writeCmd(2,wdata);

	wdata[0]= 0xA1;
	wdata[1]=0x00;
	writeCmd(2,wdata);

	wdata[0]= 0xA0;
	wdata[1] = 0xB4;
	writeCmd(2,wdata);

	wdata[0] = 0xAB;
	wdata[1] = 0x01;
	writeCmd(2,wdata);

	wdata[0] = 0xB4;
	wdata[1] = 0xA0;
	wdata[2] = 0xB5;
	wdata[3] = 0x55;
	writeCmd(4,wdata);

	wdata[0] = 0xC1;
	wdata[1] = 0xC8;
	wdata[2] = 0x80;
	wdata[3] = 0xC8;
	writeCmd(4,wdata);

	wdata[0] = 0xC7;
	wdata[1] = 0x0F;
	writeCmd(2,wdata);

	// gamma setting 
	writeCmd(64,gamma);


	wdata[0] = 0xB1;
	wdata[1] = 0x32;
	writeCmd(2,wdata);

	wdata[0] = 0xB2;
	wdata[1] = 0xA4;
	wdata[2] = 0x00;
	wdata[3] = 0x00;
	writeCmd(4,wdata);

	wdata[0] = 0xBB;
	wdata[1] = 0x17;
	writeCmd(2,wdata);

	wdata[0] = 0xB6;
	wdata[1] = 0x01;
	writeCmd(2, wdata);

	wdata[0]= 0xBE;
	wdata[1] = 0x05;
	writeCmd(2, wdata);

	wdata[0] = 0xA6;
	writeCmd(1,wdata);
	

	for (i = 0; i < 128;i++ )
	{
		for(j = 0; j < 128; j++ )
		{
			wcdata[0]= 0x3F;
			wcdata[1]= 0;
			wcdata[2] = 0;
			writeData(3,wcdata);
		}
	
	}

	wdata[0] = 0xAF;
	writeCmd(1,wdata);
}

void oled_imageLoading(const char* fileName)//load the image and put fileName 
{
	int imgfile;
	unsigned char* temp =NULL;//temporary space
	int  width , height;

	setting_init_oled();

	imgfile = open(fileName , O_RDONLY );
	setCmdLock(FALSE);//release command lock


	read(imgfile ,&width , sizeof(unsigned char));
	read(imgfile ,&height , sizeof(unsigned char));

	temp = malloc( 128 * 128 * 3 );

	read(imgfile, temp , 128 * 128 *3 );//copy to temp from imgfile
	close(imgfile);//close the using imagefile

	writeData(128 * 128 *3 ,temp);//load image to oled
	setCmdLock(TRUE);//command lock
	
	free(temp);//free using memory
}

int main(int argc, char** argv)
{
	oled_init();
	oled_imageLoading(argv[1]);//이미지가 있는 곳의 메모리 주소 
	oled_destroy();
	return 0;
}
