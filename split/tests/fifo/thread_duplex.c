#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>

#define BUF_SIZE 11
#define STDOUT 1
#define STDIN 0
#define PERMISSION 0777

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

void *thread(void * arg)
{
	char * fileOut = (char *)arg;

	if(mkfifo(fileOut, PERMISSION) == -1)
	{
		if(errno != EEXIST)
		{
			printf("mkfifo error\n");
			exit(1);
		}
		else
			printf("fifo exist\n");
	}
	else
		printf("mkfifo successfuly\n");

	int fifoOut = open(fileOut, O_WRONLY);
	CHECK("open", fifoOut);

	char bufOut[BUF_SIZE];
	int n;

	do
	{
		n = read(STDIN, bufOut, BUF_SIZE);
		if(n - write(fifoOut, bufOut, n))
		{
			printf("write error\n");
			exit(3);
		}
	}
	while(n);

	CHECK("close", close(fifoOut));

	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Error arg\n");
		return 1;
	}

	char * fileIn = argv[1];
	char * fileOut = argv[2];

	pthread_t thid;
	if(pthread_create(&thid, (pthread_attr_t *)NULL, thread, (void *)fileOut) != 0)
	{
		printf("Error pthread\n");
		exit(-1);
	}
	else
		printf("Thread created, thid = %d\n", (int)thid);

	if(mkfifo(fileIn, PERMISSION) == -1)
	{
		if(errno != EEXIST)
		{
			printf("mkfifo error\n");
			return 1;
		}
		else
			printf("fifo exist\n");
	}
	else
		printf("mkfifo successfuly\n");

	int fifoIn = open(fileIn, O_RDONLY);
	CHECK("open", fifoIn);

	char bufIn[BUF_SIZE];
	int n;

	do
	{
		n = read(fifoIn, bufIn, BUF_SIZE);
		if(n - write(STDOUT, bufIn, n))
		{
			printf("write error\n");
			return 3;
		}
	}
	while(n);

	CHECK("close", close(fifoIn));

	return 0;
}
