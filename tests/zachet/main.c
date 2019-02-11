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

#define AMOUNT_OF_NUMBERS_FOR_MACHINE 10
#define MACHINE_NUM 5
#define DEFAULT_IP "127.0.0.1"
#define MAX_RANDOM 100

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

struct Message
{
	int array[AMOUNT_OF_NUMBERS_FOR_MACHINE];
};

struct ReverseMessage
{
	int result;
};

void generateData(int * array)
{
	srand(time(NULL));

	int i;
	for(i = 0; i < AMOUNT_OF_NUMBERS_FOR_MACHINE * MACHINE_NUM; i++)
		array[i] = rand() % MAX_RANDOM;

	printf("generated succesfull\n");
}

int setConnect(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if(inet_aton(DEFAULT_IP, &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		CHECK("inet_aton", -1);
	}

	int result = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("connet", result);

	return sockfd;
}

void printSum(int * array)
{
	int i, result = 0;
	for(i = 0; i < (AMOUNT_OF_NUMBERS_FOR_MACHINE * MACHINE_NUM); i++)
	{
		result += array[i];
		printf("%d\n", array[i]);
	}

	printf("result = %d\n", result);
}

void setNum(int * destination, int * source)
{
	int i;
	for(i = 0; i < AMOUNT_OF_NUMBERS_FOR_MACHINE; i++)
		destination[i] = source[i];
}

int main(int argc, char **argv)
{
	if(argc != MACHINE_NUM + 1)
	{
		printf("arg error: need %d numbers of ports\n", MACHINE_NUM);
		exit(1);
	}

	int * array = calloc(AMOUNT_OF_NUMBERS_FOR_MACHINE * MACHINE_NUM, sizeof(int));
	assert(array);

	generateData(array);
	printSum(array);

	int * sockfd = calloc(MACHINE_NUM, sizeof(int));
	int i, result;
	for(i = 0; i < MACHINE_NUM; i++)
	{
		sockfd[i] = setConnect(atoi(argv[i + 1]));
		CHECK("setConnect", sockfd[i]);
	}

	struct Message * message = calloc(MACHINE_NUM, sizeof(struct Message));
	assert(message);

	for(i = 0; i < MACHINE_NUM; i++)
	{
		setNum(message[i].array, &array[i * AMOUNT_OF_NUMBERS_FOR_MACHINE]);
		result = write(sockfd[i], &message[i], sizeof(struct Message));
		if(result != sizeof(struct Message))
		{
			printf("Write error. result = %d\n", result);
			return -1;
		}
	}

	struct ReverseMessage * reverseMessage = calloc(MACHINE_NUM, sizeof(struct ReverseMessage));
	assert(reverseMessage);

	int answer = 0;

	for(i = 0; i < MACHINE_NUM; i++)
	{
		result = read(sockfd[i], &reverseMessage[i], sizeof(struct ReverseMessage));
		if(result != sizeof(struct ReverseMessage))
		{
			printf("read error. result = %d\n", result);
			return -1;
		}

		answer += reverseMessage[i].result;
	}

	printf("general result = %d\n", answer);

	free(array);
	free(sockfd);

	free(message);
	free(reverseMessage);
}
