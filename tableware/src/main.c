#include"libs.h"
#include"configure.h"
#include"list.h"
#include"read_from_file.h"
#include"sem.h"
#include"shm.h"

void washingElement(Data * data);
void wipeElement(Data * data);
void *tranmsitTableware(void * arg);
int joinThreads(int numberOfThreads, pthread_t *thids);

typedef struct Segment
{
	int semid;
	Data * pieceOfMemory;
	Data * data;
} Segment;

void freeMemory(pthread_t * thids, Segment * segment);
int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads);

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

		key_t key = getTheKey(KEY_FILE);

		int semid = createSem(key);

		int shmid = createShm(key);
		Data * pieceOfMemory = attachMemory(shmid);

		putInMemory(pieceOfMemory, listTableware->first->data);
		printData(pieceOfMemory);

		printf("size = %d\n", listTableware->size);
		Segment * segment;
		pthread_t * thids;
		int numberOfThreads = listTableware->size;
		if(!allocateMemory(&thids, &segment, numberOfThreads))
		{
			printf("Allocate error\n");
			exit(-1);
		}

		printf("\nStart of work!\n\n");
		Node * current;
		int i = 0, result;
		for(current = listTableware->first; current != NULL; current = current->next)
		{
			washingElement(current->data);
			semOperation(semid, spaceOnTheTable, -1);

			segment[i].pieceOfMemory = pieceOfMemory;
			segment[i].data = current->data;
			segment[i].semid = semid;
			result = pthread_create(&thids[i], NULL, tranmsitTableware, &segment[i]);
			i++;
		}

		result = joinThreads(numberOfThreads, thids);
		CHECK("joinThreads", result);

		semOperation(semid, workIsDone, -1);
		printf("\nEnd of work!\n\n");

		CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
		CHECK("shmdt", shmdt(pieceOfMemory));
		CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));

		deleteList(listTimes);
		deleteList(listTableware);

		freeMemory(thids, segment);
	}
	else
	{
		printf("Wiper!\n");

		key_t key = getTheKey(KEY_FILE);
		int semid = connectToSem(key);

		int shmid = connectToShm(key);
		Data * pieceOfMemory = attachMemory(shmid);

		List * listTableware = createList("listTableware");

		printf("\nStart of work!\n\n");

		while(1)	//узнать, что передача завершена
		{
			semOperation(semid, readyToReadFromMemory, 1);
			semOperation(semid, allowedToReadFromMemory, -1);
			wipeElement(pieceOfMemory);
			addToList(listTableware, copyData(pieceOfMemory));
			semOperation(semid, spaceOnTheTable, 1);
		}

		semOperation(semid, workIsDone, 1);
		printf("\nEnd of work!\n\n");
		printList(listTableware);

		CHECK("shmdt", shmdt(pieceOfMemory));
		deleteList(listTableware);
	}
}

int joinThreads(int numberOfThreads, pthread_t *thids)
{
	int i, result;

	for(i = 0; i < numberOfThreads; i++)
	{
		result = pthread_join(thids[i], (void **)NULL);

		if(result != 0)
		{
			return -1;
		}
	}

	return 0;
}

int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads)
{
	*thids = calloc(numberOfThreads, sizeof(pthread_t));
	*segment = (struct Segment *)calloc(numberOfThreads, sizeof(struct Segment));

	return segment && thids;
}

void freeMemory(pthread_t * thids, Segment * segment)
{
	free(thids);
	free(segment);
}

void *tranmsitTableware(void * arg)
{
	printf("In thread for tableware transmit\n");
	Segment * segment = (Segment *)arg;

	printData(segment->pieceOfMemory);
	printData(segment->data);

	semOperation(segment->semid, readyToReadFromMemory, -1);
	putInMemory(segment->pieceOfMemory, segment->data);
	semOperation(segment->semid, allowedToReadFromMemory, 1);

	pthread_exit(0);
}

void wipeElement(Data * data)
{
	printf("\t\t\t\t\tWipe %s for %d seconds\n", data->name, data->timeToWipe);

	sleep(data->timeToWipe);
}

void washingElement(Data * data)
{
	printf("\t\t\t\t\tCleaning %s for %d seconds\n", data->name, data->timeToWash);

	sleep(data->timeToWash);
}
