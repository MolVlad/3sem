#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

int main()
{
	umask(0);
	open("mask_0", O_CREAT, 0666);

	umask(0666);
	open("mask_666", O_CREAT, 0666);

	umask(0);
	open("mask_777", O_CREAT, 0777);

	return 0;
}
