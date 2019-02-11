#ifndef __THREAD_H__
#define __THREAD_H__

typedef struct Segment
{
	int semid;
	Data * pieceOfMemory;
	Data * data;
} Segment;

void freeMemory(pthread_t * thids, Segment * segment);
int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads);
int joinThreads(int numberOfThreads, pthread_t *thids);

#endif /* __THREAD_H__ */
