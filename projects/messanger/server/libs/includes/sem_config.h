#ifndef __SEM_H__
#define __SEM_H__

#define NUM_OF_SEM 3

enum SemName
{
	generalFifoSynch = 0,
	userListSynch,
	serverThreadSynch,
};

int createSem(key_t key, int num);
void semOperation(int semid, enum SemName semName, int operation);
int connectToSem(key_t key, int num);

#endif /* __SEM_H__ */
