#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>

#define BUF_SIZE 10
#define STDOUT 1
#define STDIN 0
#define PERMISSION 0777

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
	}
	else
		printf("mkfifo successfuly\n");

	int fifoOut = open(fileOut, O_WRONLY);
	if(fifoOut == -1)
	{
		printf("open error\n");
		exit(2);
	}
	else
		printf("open %s successfuly\n", fileOut);;

	char bufOut[BUF_SIZE];
	int n;

	while(1)
	{
		n = read(STDIN, bufOut, BUF_SIZE);
		if(n - write(fifoOut, bufOut, n))
		{
			printf("write error\n");
			exit(3);
		}
	}

	if(close(fifoOut) == -1)
	{
		printf("close error\n");
		exit(4);
	}
	else
		printf("close fifoOut successfuly\n");

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
	}
	else
		printf("mkfifo successfuly\n");

	int fifoIn = open(fileIn, O_RDONLY);
	if(fifoIn == -1)
	{
		printf("open error\n");
		return 2;
	}
	else
		printf("open %s successfuly\n", fileIn);;

	char bufIn[BUF_SIZE];
	int n;

	while(1)
	{
		n = read(fifoIn, bufIn, BUF_SIZE);
		if(n - write(STDOUT, bufIn, n))
		{
			printf("write error\n");
			return 3;
		}
	}

	if(close(fifoIn) == -1)
	{
		printf("close error\n");
		return 4;
	}
	else
		printf("close fifoIn successfuly\n");

	return 0;
}
