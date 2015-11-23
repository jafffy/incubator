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



#define LED_ON	1
#define LED_OFF	0
#define MAX_LED_NO		8
#define LED_DRIVER_PATH		"/dev/cnled"

typedef struct{
	unsigned char int fd;

}busledContext;

void busled_init(busledContext* context)//초기화
{
	context->fd = open(LED_DRIVER_PATH,O_RDWR);
}

void busled_destroy(busledContext* context)//제거
{
	close(context->fd);
}

int severity_classified(busledContext* context, unsigned char* severity )//중증도 분류해서 값 반환 중증도가 1인 경우 1000 0000 반환하고 8인경우 0000 0001반환
{					//1에서는 점등되고 0에서는 소등	
	unsigned char  wdata;//busled에 뿌릴 값
	unsigned char rdata;//버퍼에서 읽어드린 값
	unsigned char  temp = 1;
		read(context->fd,&rdata,4);	
		temp <<=(severity-1);
		wdata = rdata | temp;
	return wdata;
}

void busled_on(busledContext* context, unsigned char* ledNo)
{
	write(context->fd,ledNo,4);
}


void doHelp(void)
{
	printf("Usage:\n");
	printf("ledtest <ledNo> <1(on)|0(off)>\n");
	printf("ex) ledtest 3 1 ;3th led on\n");
	printf("    ledtest 4 0 ;4th led off\n");
	printf("    ledtest 0 0 ;all led off\n");
	printf("    ledtest 0 1 ;all led on\n");
	printf("Max LedNo =>8 \n");  
}



int main(int argc , char **argv)
{
	int ledNo = 0;
	int ledControl = 0;
	int wdata ,rdata,temp ;
	
	int fd;
	
	if (argc < 3 )
	{
		perror(" Args number is less than 3\n");
		doHelp();
		return 1;
	}
	
	if ( strlen(argv[1]) > 1 )
	{
		perror("<ledNo> length > 1 \n");
		doHelp();
		return 1;
	}
	
	if (isalnum((int)argv[1][0])) // number
	{
		ledNo = (int)(argv[1][0] - '0');
		if ( ledNo > MAX_LED_NO )
		{
			perror(" <ledNo> over range \n");
			doHelp();
			return 1;
		}
	}
	else 
	{
		perror(" <ledNo> is not number\n");
		doHelp();
		return 1;
	}
	
	if ( (argv[2][0] == '0' ) || ( argv[2][0] == '1' ))
	{
		ledControl = (int)(argv[2][0] - '0');
	}
	else
	{
		perror(" 3th arg is not '1' and '0'. \n");
		doHelp();
		return 1;
	}
	
	// open  driver 
	fd = open(LED_DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver (//dev//cnled) open error.\n");
		return 1;
	}
	// control led 
	
	if ( ledNo == 0 )
	{
		if ( ledControl ==  1 ) wdata = 0xff;
		else wdata = 0;
	}
	else
	{
		read(fd,&rdata,4);
		temp = 1;

		if ( ledControl ==1 )
		{
			temp <<=(ledNo-1);
			wdata = rdata | temp;
		}
		else
		{
			temp =  ~(temp<<(ledNo-1));
			wdata = rdata & temp;
		}
	}
	printf("wdata:0x%X\n",wdata);
	write(fd,&wdata,4);

	close(fd);
	
	return 0;
}
