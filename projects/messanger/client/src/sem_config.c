#include"libs.h"
#include"general_config.h"
#include"sem_config.h"

key_t getTheKey(const char * keyFileName, int id)
{
	key_t key = ftok(keyFileName, id);
	CHECK("ftok", key);

	return key;
}

int createSem(key_t key, int num)
{
	int semid = semget(key, num, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
			//printf("Sem already exist. Need to remove\n");

			semid = semget(key, num, PERMISSION);
			CHECK("semget", semid);

			CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));

			semid = semget(key, num, PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("semid", semid);
		}
		else
		{
			perror("semget");
			return 2;
		}
	}

	return semid;
}

int connectToSem(key_t key, int num)
{
	int semid;
	do semid = semget(key, num, PERMISSION);
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
}
