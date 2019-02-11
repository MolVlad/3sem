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

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);		\
		exit(1);					\
	}							\
} while(0)							\

int main()
{
	int n, clilen;
	char line[BUF_SIZE];

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	CHECK("socket", sockfd);
	printf("%d\n", sockfd);

	struct sockaddr_in servaddr, cliaddr;
	bzero(&cliaddr, sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(cliaddr)) < 0)
	{
		perror("bind");
		close(sockfd);
		exit(1);
	}

	while(1)
	{
		clilen = sizeof(cliaddr);
		printf("server\n");
		if((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}

		printf("%s\n", line);

		if(sendto(sockfd, line, strlen(line) + 1, 0, (struct sockaddr *) &cliaddr, clilen) < 0)
		{
			perror("sendto");
			close(sockfd);
			exit(1);
		}
	}

	return 0;
}
