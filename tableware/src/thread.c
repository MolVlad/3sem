#include"libs.h"
#include"configure.h"
#include"list.h"
#include"sem.h"
#include"shm.h"
#include"thread.h"

int joinThreads(int numberOfThreads, pthread_t *thids)
{
	int i, result;

	for(i = 0; i < numberOfThreads; i++)
	{
		result = pthread_join(thids[i], (void **)NULL);

		if(result != 0)
		{
			return -1;
		}
	}

	return 0;
}

int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads)
{
	*thids = calloc(numberOfThreads, sizeof(pthread_t));
	*segment = (struct Segment *)calloc(numberOfThreads, sizeof(struct Segment));

	return segment && thids;
}

void freeMemory(pthread_t * thids, Segment * segment)
{
	free(thids);
	free(segment);
}
