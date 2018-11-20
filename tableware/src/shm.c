#include"libs.h"
#include"configure.h"
#include"list.h"
#include"sem.h"
#include"shm.h"

int createShm(key_t key)
{
	int shmid = shmget(key, sizeof(Data), PERMISSION | IPC_CREAT | IPC_EXCL);
	if(shmid < 0)
	{
		if(errno == EEXIST)
		{
			printf("Already exist. Need to remove\n");

			shmid = shmget(key, sizeof(Data), PERMISSION);
			CHECK("shmget", shmid);

			CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));

			shmid = shmget(key, sizeof(Data), PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("shmid", shmid);
		}
		else
		{
			printf("Some error");
			exit(2);
		}
	}
	else
		printf("Shm create succesfully\n");

	return shmid;
}

Data * attachMemory(int shmid)
{
	Data * pieceOfMemory = (Data *)shmat(shmid, NULL, 0);
	if(pieceOfMemory == (Data *) (-1))
	{
		printf("Attach memory error\n");
		exit(-5);
	}
	else
		printf("Attach memory succesfully\n");

	return pieceOfMemory;
}

int connectToShm(key_t key)
{
	int shmid;

	do shmid = shmget(key, sizeof(Data), PERMISSION);
	while((shmid == -1) && (errno ==ENOENT));

	if(shmid == -1)
	{
		printf("Some error");
		exit(4);
	}
	printf("shmget succeed\n");

	return shmid;
}
