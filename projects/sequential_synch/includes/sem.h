#ifndef __SEM_H__
#define __SEM_H__

#define NUM_OF_SEM 4

enum SemName
{
	spaceOnTheTable = 0,
	readyToReadFromMemory,
	allowedToReadFromMemory,
	workIsDone,
};

key_t getTheKey(const char * keyFileName);
int createSem(key_t key);
void semOperation(int semid, enum SemName semName, int operation);
int connectToSem(key_t key);

#endif /* __SEM_H__ */
