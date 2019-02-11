#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/wait.h>

#define BUF_SIZE 1000
#define PORT 51000

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);		\
		exit(1);					\
	}							\
} while(0)							\

int main(int argc, char **argv)
{
	int n, len;
	char sendline[BUF_SIZE], recvline[BUF_SIZE];
	struct sockaddr_in servaddr, cliaddr;

	if(argc != 2)
	{
		printf("Usage: a.out <IP address>");
		exit(-1);
	}

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	CHECK("socket", sockfd);
	printf("%d\n", sockfd);

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0)
	{
		perror("bind");
		close(sockfd);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	printf("String = ");
	fgets(sendline, BUF_SIZE, stdin);

	if(sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("sendto");
		close(sockfd);
		exit(1);
	}

	printf("lol\n");

	if(recvfrom(sockfd, recvline, BUF_SIZE, 0, (struct sockaddr *) NULL, NULL) < 0)
	{
		perror("recvfrom");
		close(sockfd);
		exit(1);
	}

	printf("%s\n", recvline);
	close(sockfd);

	return 0;
}
