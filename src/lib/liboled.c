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

void oled_open()//open the device driver
{
	context.fd = open(OLED_DRIVER_PATH,O_RDWR);
}

void oled_destroy()//close the device driver
{
	close(oledCont.fd);
}

int oled_imageLoading(const char* fileName)//load the image and put fileName 
{
	int imgfile;
	unsigned char* temp =NULL;//임시 보관
	int  width , height;

	imgfile = open(fileName , O_RDONLY );
	if ( imgfile < 0 ) 
	{
		printf ("imageloading(%s)  file is not exist . err.\n",fileName);
		return FALSE;
	}
	setCmdLock(FALSE);//명령어 입력 잠금 해제


	read(imgfile ,&width , sizeof(unsigned char));
	read(imgfile ,&height , sizeof(unsigned char));

	temp = malloc( 128 * 128 * 3 );//메모리 확보

	read(imgfile, temp , 128 * 128 *3 );//이미지 데이터를 템프로 복사

	close(imgfile);//사용한 이미지 파일 클로즈

	writeData(128 * 128 *3 ,temp );//oled에 이미지 로딩

	setCmdLock(TRUE);//명령어 입력 잠금
	return TRUE;
}


