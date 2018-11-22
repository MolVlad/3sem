#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		perror(nameFunction);				\
		return 1;					\
	}							\
} while(0)							\

int main()
{
	DIR * dir = opendir(".");
	if(!dir)
	{
		printf("open error\n");
		return -1;
	}

	struct stat buf;
	struct dirent * dirent = readdir(dir);
	int result;
	while(dirent != NULL)
	{
		printf("name: %s\t", dirent->d_name);

		result = stat(dirent->d_name, &buf);
		CHECK("stat", result);
		printf("size: %ld\t", buf.st_size);

		if(S_ISREG(buf.st_mode))
			printf("mode: reg\t");
		if(S_ISDIR(buf.st_mode))
			printf("mode: dir\t");
		printf("\n");

		dirent = readdir(dir);
	}

	CHECK("closedir", closedir(dir));
}
