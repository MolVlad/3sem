#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SIZE 1000

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		perror(nameFunction);				\
		return 1;					\
	}							\
} while(0)							\

int find(const char * current_dir, const char * desired, int possibleDepth)
{
	if(possibleDepth < 0)
		return 0;

	DIR * dir = opendir(current_dir);
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
		char * path = (char *)calloc(strlen(current_dir) + strlen(dirent->d_name) + 2, sizeof(char));
		strcpy(path, current_dir);
		strcat(path, "/");
		strcat(path, dirent->d_name);

		result = stat(path, &buf);
		CHECK("stat", result);

		if(S_ISREG(buf.st_mode))
		{
			if(strcmp(dirent->d_name, desired) == 0)
				printf("%s\n", path);
		}

		if(S_ISDIR(buf.st_mode) && strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, ".."))
		{
			find(path, desired, possibleDepth - 1);
		}

		free(path);
		dirent = readdir(dir);
	}

	CHECK("closedir", closedir(dir));

	return 0;
}

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		printf("argc error\n");
		return -1;
	}

	int depth = atoi(argv[1]);

	find(".", argv[2], depth);
}
