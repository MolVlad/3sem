#ifndef __SEM_H__
#define __SEM_H__

#define NUM_OF_SEM 3

enum SemName
{
	fifoSynch = 0,
	listSynch,
	serverThread,
};

key_t getTheKey(const char * keyFileName);
int createSem(key_t key, int num);
void semOperation(int semid, enum SemName semName, int operation);
int connectToSem(key_t key, int num);

#endif /* __SEM_H__ */
