#include"libs.h"
#include"configure.h"
#include"fifo.h"

int openOrCreateFIFO(const char * name)
{
	int fd = open(name, O_RDWR);

	if(fd == -1)
	{
		printf("Need to create FIFO: %s\n", name);
		mkfifo(name, PERMISSION);
		fd = open(name, O_RDWR);
	}

	return fd;
}

void getID(char * id)
{
	int pid = getpid();

	int i;
	for(i = SIZE_ID - 1; i >= 0; i--)
	{
		id[i] = pid % 10 + '0';
		pid /= 10;
	}
}

int writeID(int fd, char * id)
{
	int n = write(fd, id, SIZE_ID);
	if(n != SIZE_ID)
	{
		printf("Error write name\n");
		return -1;
	}

	return 0;
}

int readID(int fd, char * id)
{
	int n = read(fd, id, SIZE_ID);
	if(n != SIZE_ID)
	{
		printf("Error read name: n = %d\n", n);
		return -1;
	}

	return 0;
}
