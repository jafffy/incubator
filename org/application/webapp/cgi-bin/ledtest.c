//////////////////////////////////////////////////////////
//                  LED CGI Program                   ////
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "peripheral.h"

#define LED_ON	1
#define LED_OFF	0
#define MAX_LED_NO	8
#define LED_DRIVER_NAME "/dev/cnled"
#define LED1_ON 	1
#define LED1_OFF	9

typedef struct{
	char name[128];
	char val[128];
}entry;

void getword(char *word, char *line, char stop)
{
	int x = 0, y = 0 ;
	
	for(x=0; ((line[x]) && (line[x] != stop)); x++)
	{
		word[x] =line[x];
	}

	word[x] = '\0';
	
	if(line[x])
	{
		++x;
	}

	while(line[y++] = line[x++]);
}

void ImageDisplay(int data)
{
	int tmp,LedInfo,i;
	tmp = 1;
	
	printf("&nbsp;\n");

	for(i = 0; i < 8; i++ )
	{
		LedInfo = tmp & data;
		data>>=1;
		if(LedInfo == 1)
		{
			printf("<img src=\"../image/LedOn.png\" style=\"width: 94px; height: 85px\" /> \n");
			printf("&nbsp; &nbsp; \n");

		}
		
		else
		{
			printf("<img src=\"../image/LedOff.png\" style=\"width: 94px; height: 85px\" /> \n");
			printf("&nbsp; &nbsp; \n");
		}
	}	
}

int main()
{

	char *BtnVal;

	entry entries; 
	
	int num,fd,Val;
	int LedNo = 0;
	int LedControl = 0;
	int wdata ,rdata, temp;
	
	printf("Contnet-type :  text/html\n\n");

	printf("<html>\n<head><title>LED Web Control</title></head>\n");
	printf("<body>\n");
	printf("<form method=get>\n");
	printf("&lt; LED Web Control &gt;<br />\n");
	printf("<br />\n");
	printf("&nbsp;\n");
	printf("<input name= \"1\" style=\"width: 94px\" type=\"submit\" value=\"LED1 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"2\" style=\"width: 94px\" type=\"submit\" value=\"LED2 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"3\" style=\"width: 94px\" type=\"submit\" value=\"LED3 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"4\" style=\"width: 94px\" type=\"submit\" value=\"LED4 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"5\" style=\"width: 94px\" type=\"submit\" value=\"LED5 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"6\" style=\"width: 94px\" type=\"submit\" value=\"LED6 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"7\" style=\"width: 94px\" type=\"submit\" value=\"LED7 ON\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"8\" style=\"width: 94px\" type=\"submit\" value=\"LED8 ON\" />\n");
	printf("<br /><br /> \n");
	printf("&nbsp;\n");
	printf("<input name= \"9\" style=\"width: 94px\" type=\"submit\" value=\"LED1 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"10\" style=\"width: 94px\" type=\"submit\" value=\"LED2 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"11\" style=\"width: 94px\" type=\"submit\" value=\"LED3 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"12\" style=\"width: 94px\" type=\"submit\" value=\"LED4 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"13\" style=\"width: 94px\" type=\"submit\" value=\"LED5 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"14\" style=\"width: 94px\" type=\"submit\" value=\"LED6 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"15\" style=\"width: 94px\" type=\"submit\" value=\"LED7 OFF\" />\n");
	printf("&nbsp; &nbsp; \n");
	printf("<input name= \"16\" style=\"width: 94px\" type=\"submit\" value=\"LED8 OFF\" />\n");
	printf("<br /><br /> \n");

	BtnVal = (char *)getenv("QUERY_STRING");
	getword(entries.name,BtnVal,'=');
	getword(entries.val,BtnVal,'&');
	
	num = atoi(entries.name);
	if ( num < LED1_OFF)
	{
		LedNo = num;
		LedControl = LED_ON;
	}
	else
	{
		LedNo = num - LED1_OFF +1;
		LedControl = LED_OFF;
	}
	fd = open(LED_DRIVER_NAME,O_RDWR); //open driver

	if(fd < 0)
	{
		printf("LED Driver (//dev//cnled) open fail.\n");
		return 1;
	}

	read(fd,&rdata,4);

	temp = 1;

	
	if(LedControl == 1)
	{
		temp <<=(LedNo-1);
		wdata = rdata | temp;
	}
	else
	{
		temp = ~(temp<<(LedNo-1));
		wdata = rdata & temp;
	}
	write(fd,&wdata,4);

	read(fd,&rdata,4);

	ImageDisplay(rdata);
	
	return 0;
}
