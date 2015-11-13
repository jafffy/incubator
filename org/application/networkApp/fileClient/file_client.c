#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE        30
#define NUM_OF_ARGC     3
#define INIT_VAL_OF_MEM 0
#define PROTOCOL_TYPE   0
#define DATA_SIZE       1

void error_handling(char *message);


int main(int argc, char *argv[])
{
	int     clientfd;
	FILE    *fp;

    	char    buf[BUF_SIZE];
	char    file_name[BUF_SIZE];
	int     read_cnt;
	struct  sockaddr_in serv_adr;
	int		fileSize;
	int		totalReadCounter;
	int     i;
	int	flag;

	printf("=========================================\n");
	printf("TCP/IP File Transmission Program - Client\n");
	printf("=========================================\n\n");

	if(argc != NUM_OF_ARGC) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	printf("Input file name: ");
	scanf("%s", file_name);
	fp = fopen(file_name, "wb");

	clientfd = socket(PF_INET, SOCK_STREAM, PROTOCOL_TYPE);
	if (clientfd == -1)
        	error_handling("socket() error");

	memset(&serv_adr, INIT_VAL_OF_MEM, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(clientfd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	        error_handling("connect() error");

	write(clientfd, file_name, strlen(file_name)+1);

	// first read file length
	flag = 1;
	while(flag)
	{
		read_cnt = read(clientfd, buf, BUF_SIZE);
		if (read_cnt < 0 )
		{
			printf("ERROR!");
			return;
		}
		else if (read_cnt > 0)
		{
			for (i = 0; i < read_cnt ; i++ )
			{
				if (buf[i] == 0 )
				{
					fileSize = atoi(buf);
					printf("fileSize:%d\n",fileSize);
					flag = 0;
					break;
				}
			}
		}
	}
	if ( fileSize < 0 )
	{
		printf("no file in server.\n");
		return 0;
	}

	totalReadCounter =0;

	while(1)
	{
		read_cnt = read(clientfd, buf, BUF_SIZE);
		totalReadCounter += read_cnt;
		fwrite((void*)buf, DATA_SIZE, read_cnt, fp);
		if(read_cnt)
			printf(".");
		if (totalReadCounter >= fileSize )
		{
			printf("\n%s file rx success!\n",file_name);
			break;
		}
	}

	fclose(fp);
	close(clientfd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
