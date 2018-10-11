#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

#define COUNT 20
#define BUF_SIZE 100

int main()
{
	int fd1[2];
	int fd2[2];
	int a,b, res;

	pipe(fd2);
	pipe(fd1);

	int pid = fork();

	if(pid == 0)
	{
		close(fd1[1]);
		close(fd2[0]);

		read(fd1[0], &a, sizeof(int));
		read(fd1[0], &b, sizeof(int));

		res = a + b;
		write(fd2[1], &res, 1);

		close(fd1[0]);
		close(fd2[1]);

		exit(0);
	}
	else
	{
		close(fd2[1]);
		close(fd1[0]);

		printf("Print a b\n");
		scanf("%d %d", &a, &b);

		write(fd1[1], &a, sizeof(int));
		write(fd1[1], &b, sizeof(int));

		read(fd2[0], &res, 1);
		printf("a+b = %d\n", res);

		close(fd1[1]);
		close(fd2[0]);

		exit(0);
	}

	return 0;
}
