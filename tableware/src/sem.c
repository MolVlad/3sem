#include"libs.h"
#include"configure.h"
#include"sem.h"

key_t getTheKey(const char * keyFileName)
{
	key_t key = ftok(keyFileName, 0);
	CHECK("ftok", key);

	return key;
}

int createSem(key_t key)
{
	int semid = semget(key, NUM_OF_SEM, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
			printf("Sem already exist. Need to remove\n");

			semid = semget(key, NUM_OF_SEM, PERMISSION);
			CHECK("semget", semid);

			CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));

			semid = semget(key, NUM_OF_SEM, PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("semid", semid);
		}
		else
		{
			perror("semget");
			return 2;
		}
	}

	semOperation(semid, spaceOnTheTable, SPACE_ON_THE_TABLE);

	return semid;
}

int connectToSem(key_t key)
{
	int semid;
	do semid = semget(key, NUM_OF_SEM, PERMISSION);
	while((semid == -1) && (errno ==ENOENT));

	if(semid == -1)
	{
		perror("semget");
		exit(4);
	}

	return semid;
}

void semOperation(int semid, enum SemName semName, int operation)
{
	struct sembuf semBuf;

	semBuf.sem_op = operation;
	semBuf.sem_flg = 0;
	semBuf.sem_num = semName;

	int result = semop(semid, &semBuf, 1);
	CHECK("semop", result);
	printf("sem_num = %d, sem_op = %d\n", semName, operation);
}
