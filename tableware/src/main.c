#include"libs.h"
#include"configure.h"
#include"list.h"
#include"read_from_file.h"
#include"sem.h"
#include"shm.h"

void washingElement(List * listWetTableware, Data * data);

typedef struct Segment
{
	Data * data;
	int sem;
} Segment;

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
		char * data = attachMemory(shmid);

		strcpy(data, listTableware->first->data->name);

		printf("Start of work!\n");
		List * listWetTableware = createList("listWetTableware");
		Node * current;
		for(current = listTableware->first; current != NULL; current = current->next)
		{
			washingElement(listWetTableware, current->data);
			semOperation(semid, spaceOnTheTable, -1);
//			putOnTheTable();
		}

		CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
		CHECK("shmdt", shmdt(data));
		CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));

		deleteList(listTimes);
		deleteList(listTableware);
	}
	else
	{
		printf("Wiper!\n");

		List * listTimes = createList("listTimes");
		readTimesList(listTimes);
		printList(listTimes);

		printf("\nLet's connect to sem\n");
		key_t key = getTheKey(KEY_FILE);
		//int semid = connectToSem(key);

		printf("\nLet's connect to sem\n");
		int shmid = connectToShm(key);
		char * data = attachMemory(shmid);

		//List * listTableware = createList("listTableware");

		CHECK("shmdt", shmdt(data));
		deleteList(listTimes);
	}
}

/*void thread(void * arg)
{
	Segment segment = *(Segment *)arg;


}*/

void washingElement(List * listWetTableware, Data * data)
{
	printf("I'm cleaning %s for %d seconds\n", data->name, data->timeToWash);
	sleep(data->timeToWash);
	addToList(listWetTableware, copyData(data));
}
