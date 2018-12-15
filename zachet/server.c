#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<errno.h>

#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<signal.h>
#include<time.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/socket.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/msg.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		printf("%s failure\n", nameFunction);		\
		exit(1);					\
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\


#define AMOUNT_OF_NUMBERS_FOR_MACHINE 10
#define MACHINE_NUM 5
#define BACKLOG 1

struct Message
{
	int array[AMOUNT_OF_NUMBERS_FOR_MACHINE];
};

struct ReverseMessage
{
	int result;
};

int main(int argc, char **argv)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	if(argc != 2)
	{
		printf("arg error\n");
		exit(1);
	}
	printf("port num = %d\n", atoi(argv[1]));
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("bind", result);

	result = listen(sockfd, BACKLOG);
	CHECK("listen", result);
	printf("Let's listen\n");

	struct sockaddr_in cliaddr;
	unsigned int clilen = sizeof(cliaddr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
	CHECK("accept", newsockfd);

	struct Message message;
	result = read(newsockfd, &message, sizeof(struct Message));
	if(result != sizeof(struct Message))
	{
		printf("Read error. result = %d\n", result);
		return -1;
	}

	int answer = 0;
	int i;
	for(i = 0; i < AMOUNT_OF_NUMBERS_FOR_MACHINE; i++)
	{
		answer += message.array[i];
		printf("%d\n", message.array[i]);
	}

	printf("answer  %d\n", answer);

	struct ReverseMessage reverseMessage;
	reverseMessage.result = answer;

	result = write(newsockfd, &reverseMessage, sizeof(struct ReverseMessage));
	if(result != sizeof(struct ReverseMessage))
	{
		printf("Write error. result = %d\n", result);
		return -1;
	}

	close(sockfd);
	close(newsockfd);

	return 0;
}
