#include"libs.h"
#include"general_config.h"
#include"fifo_config.h"

int createFIFO(const char * name)
{
	int result = mkfifo(name, PERMISSION);
	if((result == -1) && (errno = EEXIST))
	{
		printf("Need to remove FIFO: %s\n", name);
		remove(name);
		result = mkfifo(name, PERMISSION);
	}
	CHECK("mkfifo", result);

	int fifo = open(name, O_RDWR);
	CHECK("open fifo", fifo);

	return fifo;
}

int openFIFO(const char * name)
{
	int fifo = open(name, O_RDWR);
	CHECK("open fifo", fifo);

	return fifo;
}
