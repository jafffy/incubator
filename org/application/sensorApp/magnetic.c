#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  INPUT_DEVICE_LIST	"/proc/bus/input/devices"

#define  SYS_PATH	"S: Sysfs="
#define  ACCEL_NAME	"bmm050"

#define  LINE_FEED	0x0A

#define  MAX_BUFF	200
int main(int argc, char *argv[])
{
	FILE    *fp ,*fpProperty;

	char 	cbuff[MAX_BUFF+1];
	char	clastchar;
	char	*pChar;
	char	*pTemp;
	int	nfindNameFlag;
	int	findInputPath;
	int	i,readSize,displayNum;
	char	strSysPath[100];
	char	strshellcmd[150];
	int	x,y,z;

	printf("Parcing BMM050 inputX...\n");

	if (argc != 2)
	{
		printf("Usage : magentic  <read Number>\n");
		return 1;
	}
	displayNum = atoi(argv[1]);	
	printf("read input list file(%s)\n",INPUT_DEVICE_LIST);
	fp = fopen(INPUT_DEVICE_LIST, "r");
	
	if ( NULL == fp )
	{
		printf("%s file read error.\n",INPUT_DEVICE_LIST);
		return 1;
	}

	nfindNameFlag = 0;
	findInputPath = 0;

	for(i = 0; i < MAX_BUFF ;i++)
	{
		readSize = fread(cbuff+i, 1, 1, fp);

		if ( readSize == 0)
			break;
		if ( LINE_FEED == *(cbuff+i))
		{
			*(cbuff+i) = '\0';
			if ( !nfindNameFlag )
			{
				pChar = strstr(cbuff,ACCEL_NAME);
				if ( pChar != 0)
				{
					nfindNameFlag = 1;
				}
			}
			else 
			{
				pChar = strstr(cbuff,SYS_PATH);
				if ( pChar != 0)
				{
					sprintf(strSysPath,"/sys%s/",cbuff+10);
					printf("sys path:%s\n",strSysPath);
					findInputPath = 1;
					break;
				}
			}
			i = 0;// reset
		}
	}
	
	if (!findInputPath)
	{
		printf("No bmm050 input device\n");
		fclose(fp);
		return 1;
	}
	sprintf(strshellcmd,"echo '1'> %senable\n",strSysPath);
	printf(strshellcmd);
	system(strshellcmd);
	
	sprintf(strshellcmd,"echo '0'> %sop_mode\n",strSysPath);
	printf(strshellcmd);
	system(strshellcmd);

        sprintf(strshellcmd,"%svalue",strSysPath);	
	for(i = 0; i < displayNum  ;i++ )
	{
		fpProperty = fopen( strshellcmd, "r");
		if (NULL == fpProperty)
		{
			printf("%s open fail\n",strshellcmd);
			fclose(fp);
			return 1;
		}
		readSize = fread(cbuff , 1 ,MAX_BUFF,fpProperty);
		cbuff[readSize] = '\0';
		pTemp = cbuff;
		pChar = index(pTemp,' ');
		*pChar = '\0';
		x = atoi(pTemp);
		pTemp = pChar+1;
		pChar = index(pTemp,' ');
		*pChar = '\0';
		y = atoi(pTemp);
	 	pTemp = pChar+1;
		pChar = index(pTemp,' ');
		z = atoi(pTemp);
		printf("x = %d, y = %d, z = %d\n",x,y,z);
		fclose(fpProperty);
		sleep(2); 
	}
	fclose(fp);		
}
