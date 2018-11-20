#include"libs.h"
#include"configure.h"
#include"list.h"
#include"read_from_file.h"
#include"sem.h"
#include"shm.h"
#include"thread.h"
#include"work.h"
#include"synch.h"

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

		key_t key = waitForWiper();

		List * listTimes = createList("listTimes");
		readTimesList(listTimes);
		printList(listTimes);

		List * listTableware = createList("listTableware");
		readTablewareList(listTableware, listTimes);
		printList(listTableware);

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

		sayThatIsAll(pieceOfMemory, semid);

		semOperation(semid, workIsDone, -1);
		printf("\nEnd of work!\n\n");

		CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
		CHECK("shmdt", shmdt(pieceOfMemory));
		CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));

		deleteList(listTimes);
		deleteList(listTableware);

		freeMemory(thids, segment);
	}
	else	//wiper
	{
		printf("Wiper!\n");

		key_t key = waitForWasher();

		int semid = connectToSem(key);

		int shmid = connectToShm(key);
		Data * pieceOfMemory = attachMemory(shmid);

		List * listTableware = createList("listTableware");

		printf("\nStart of work!\n\n");

		while(!pieceOfMemory->itIsAll)
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
