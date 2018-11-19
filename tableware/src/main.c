#include"libs.h"
#include"list.h"
#include"configure.h"

void readTimesList(List * listTimes);
void readTablewareList(List * listTableware, List * listTimes);
FILE * fileOpenAndCheck(const char * fileName);

void washingElement(Data * data);
void startOfWork();
void endOfWork();

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("argc error\n");
		return -1;
	}

	if(atoi(argv[1]) == 1)	//washer
	{
		printf("Washer!\n");

		List * listTimes = createList("listTimes");
		readTimesList(listTimes);
		printList(listTimes);

		List * listTableware = createList("listTableware");
		readTablewareList(listTableware, listTimes);
		printList(listTableware);

		startOfWork();

		int pid = fork();
		if(pid == 0)
		{

		}
		else
		{
			Node * current;
			for(current = listTableware->first; current != NULL; current = current->next)
			{
				washingElement(current->data);
//				waitingForSpaceOnTheTable();
//				putOnTheTable();
			}
		}

		endOfWork();

		deleteList(listTimes);
		deleteList(listTableware);
	}
	else
	{
		printf("Wiper!\n");

		List * listTimes = createList("listTimes");
		readTimesList(listTimes);
		printList(listTimes);

		List * listTableware = createList("listTableware");

		int pid = fork();
		if(pid == 0)
		{

		}
		else
		{

		}

		deleteList(listTimes);
	}
}

void startOfWork()
{
	printf("\nStart of work!\n");
}

void endOfWork()
{
	printf("\nEnd of work!\n");
}

void washingElement(Data * data)
{
	printf("I'm cleaning %s for %d seconds\n", data->name, data->timeToWash);
	sleep(data->timeToWash);
}

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
