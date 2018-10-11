#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

#define BUF_SIZE 10
#define STDOUT 1
#define STDIN 0

int main()
{
	char buf[BUF_SIZE];
	int n;
	int fd = open("lol.txt", O_CREAT | O_WRONLY, 0666);

	int fdStdOut = dup(STDOUT);
	dup2(fd, STDOUT);

	n = read(STDIN, buf, BUF_SIZE);
	write(STDOUT, buf, n);

	dup2(fdStdOut, STDOUT);

	n = read(STDIN, buf, BUF_SIZE);
	write(STDOUT, buf, n);
}
