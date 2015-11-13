#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 	30
#define INIT_VAL_OF_MEM	0
#define NUM_OF_ARGC	2
#define PROTOCOL_TYPE	0
#define NUM_OF_QUEUE	5
#define DATA_SIZE	1

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serverfd, clientfd;
	FILE * fp;
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	int read_cnt;
	int fileSize ;
	int sendCounter;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	printf("=========================================\n");
	printf("TCP/IP File Transmission Program - Server\n");
	printf("=========================================\n\n");

	if(argc != NUM_OF_ARGC) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	serverfd =  socket(PF_INET, SOCK_STREAM, PROTOCOL_TYPE);   
	if (serverfd == -1)
	        error_handling("socket() error");

	memset(&serv_adr, INIT_VAL_OF_MEM, sizeof(serv_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serverfd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	        error_handling("bind() error");
	
	if (listen(serverfd, NUM_OF_QUEUE) == -1)
        	error_handling("listen() error");


	clnt_adr_sz = sizeof(clnt_adr);    

	clientfd = accept(serverfd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	printf("Connected IP : %s\n\n", inet_ntoa(clnt_adr.sin_addr));
	
	sleep(1);
	read(clientfd, file_name, BUF_SIZE);
	printf("%s file sending start...\n",file_name);
	fp = fopen(file_name, "rb");

	if(fp != NULL)
	{
		fseek(fp ,0 ,SEEK_END);// file size 
		fileSize = ftell(fp);
		printf("file size :%d\n",fileSize);
		read_cnt = sprintf(buf,"%d",fileSize);
		buf[read_cnt] = 0;
		write(clientfd, buf, read_cnt+1);
		sleep(1);
		sendCounter =0;
		rewind(fp);
		while(1)
		{
			read_cnt = fread((void*)buf, DATA_SIZE, BUF_SIZE, fp);
			sendCounter += read_cnt;

			if(fileSize <= sendCounter)
			{
				write(clientfd, buf, read_cnt);
				printf("%s file send OK!\n",file_name);
				break;
			}
			write(clientfd, buf, read_cnt);
		}
	}
	else // no file.  return -1
	{
		buf[0] = '-';
		buf[1] = '1';
		buf[2] = 0; // null 
		write(clientfd, buf, 3);
	}
	sleep(1);
	fclose(fp);
	close(clientfd);
	close(serverfd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
