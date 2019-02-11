#include"configure.h"
#include"read_from_file.h"
#include"libs.h"
#include"list.h"

/* Function for reading list of tableware's times from .txt file */
void readTimesList(List * listTimes)
{
	FILE *file = fileOpenAndCheck(TABLEWARE_TIMES);

	int numberOfElements;
	fscanf(file, "%d\n", &numberOfElements);

	char name[MAX_SIZE_NAME];
	int timeToWash, timeToWipe;
	int i;
	for(i = 0; i < numberOfElements; i++)
	{
		fscanf(file, "%s %d %d\n", name, &timeToWash, &timeToWipe);
		addToList(listTimes, createData(name, timeToWash, timeToWipe));
	}

	fclose(file);
}

/* Function for reading list of tableware to wash from .txt file */
void readTablewareList(List * listTableware, List * listTimes)
{
	FILE *file = fileOpenAndCheck(TABLEWARE_LIST);

	int numberOfElements;
	fscanf(file, "%d\n", &numberOfElements);

	char name[MAX_SIZE_NAME];
	Data * data;
	int i;
	for(i = 0; i < numberOfElements; i++)
	{
		fscanf(file, "%s\n", name);
		data = findInList(listTimes, name);
		if(data == NULL)
		{
			printf("Not founded\n");
			continue;
		}

		addToList(listTableware, copyData(data));
	}

	fclose(file);
}

/* Function to make the code shorter */
FILE * fileOpenAndCheck(const char * fileName)
{
	FILE *file = fopen(fileName, "r");
	if(file == NULL)
	{
		printf("fopen error\n");
		exit(-1);
	}

	return file;
}
