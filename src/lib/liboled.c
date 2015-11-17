#include "libcommon.h"
#include "liboled.h"

#define OLED_DRIVER_PATH		"/dev/cnoled"

static oledContext context;

static int writeData(int size , unsigned char* dataArr)
{
	int i ;
	unsigned short wdata;
	
	wdata = CS_BIT_MASK & DC_BIT_MASK;
	write(oledCont.fd,&wdata,2);

	write(oledCont.fd,&wdata,2);

	wdata = CS_BIT_MASK & DC_BIT_MASK & (CMD_WRITE_RAM | 0xFF00);
	write(oledCont.fd,&wdata,2);

	wdata = CS_BIT_MASK &  (CMD_WRITE_RAM | 0xFF00);
	write(oledCont.fd,&wdata,2);

	for (i = 0; i < size ; i++ )
	{
		wdata = CS_BIT_MASK & WD_BIT_MASK ;
		write(oledCont.fd,&wdata,2);

		wdata = CS_BIT_MASK & WD_BIT_MASK & ((unsigned char)dataArr[i] | 0xFF00 );
		write(oledCont.fd,&wdata,2);

		wdata = CS_BIT_MASK & ( (unsigned char)dataArr[i] | 0xFF00);
		write(oledCont.fd,&wdata,2);


	}
	wdata = DEFAULT_MASK;
	write(oledCont.fd,&wdata,2);

	return TRUE;

}

void oled_open()//드라이버 오픈
{
	context.fd = open(OLED_DRIVER_PATH,O_RDWR);
}

void oled_destroy()//드라이버 클로즈
{
	close(oledCont.fd);
}

int oled_imageLoading(const char* fileName)
{

}


