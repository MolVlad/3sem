#ifndef __SEM_H__
#define __SEM_H__

#define CLIENT_FILE_FOR_KEY "client"
#define NUM_OF_SEM 2

enum SemName
{
	communicationWithServer,	//mutex for communicate
	startOfCommunication,	//synch of start of work (thread can't start before user login\reg)
};

key_t getTheKey(const char * keyFileName);
int createSem(key_t key, int num);
void semOperation(int semid, enum SemName semName, int operation);
int connectToSem(key_t key, int num);

#endif /* __SEM_H__ */
