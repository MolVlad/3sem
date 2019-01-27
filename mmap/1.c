#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>

#define CHECK(nameFunction, retValue)				\
do								\
	if(retValue < 0)					\
		perror(nameFunction);				\
while(0)							\

int main()
{
	int fdSource = open("lol.txt", O_RDWR);
	CHECK("open", fdSource);
	int fdDestination = open("kek.txt", O_RDWR | O_CREAT, 0666);
	CHECK("open", fdDestination);

	int result;
	struct stat statBuf;
	result = fstat(fdSource, &statBuf);
	CHECK("fstat", result);

	char * ptrSource = (char *)mmap(NULL, statBuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdSource, 0);
	if(ptrSource == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	printf("%s\n", ptrSource);
	printf("sizeof: %lu\nst_size: %ld\n", strlen(ptrSource), statBuf.st_size);

	result = ftruncate(fdDestination, statBuf.st_size);
	CHECK("ftruncate", result);

	char * ptrDestination = (char *)mmap(NULL, statBuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdDestination, 0);
	if(ptrDestination == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	close(fdSource);
	close(fdDestination);

	strcpy(ptrDestination, ptrSource);
	while(1);

	result = munmap(ptrSource, statBuf.st_size);
	CHECK("munmap", result);
	result = munmap(ptrDestination, statBuf.st_size);
	CHECK("munmap", result);

	return 0;
}
