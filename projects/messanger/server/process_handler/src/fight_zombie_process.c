#include"libs.h"
#include"fight_zombie_process.h"
#include"global_variables.h"

void *functionToFightZombies()
{
	int pid;
	while(1)
	{
		pid = waitpid(-1, NULL, 0);
		if(pid != -1)
			printf("Process %d have finished work\n", pid);
		else
			sleep(2);
	}
}

int createThreadToFightZombie()
{
	int result = pthread_create(&thid, NULL, functionToFightZombies, NULL);
	if(result != 0)
	{
		printf("Error on thread create, return value = %d\n", result);
		return -1;
	}
	printf("Thread to fight zombies created\n");

	return 0;
}
