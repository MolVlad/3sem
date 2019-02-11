#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

/* Runtime test results:
 * 1 thread	0.766585
 * 2 threads	0.593474
 * 3 threads	0.547082
 * 4 threads	0.528047
 * 5 threads	0.514218
 * 6 threads	0.501517
*/

#define AMOUNT_OF_NUMBERS 100000000
#define MAX_RANDOM 1000

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		printf("%s failure\n", nameFunction);		\
		return 1;					\
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

typedef struct Segment
{
	int firstElement;
	int lastElement;
} Segment;

typedef void *(*function)(void *);

float average = 0, dispersion = 0;
int * array;
int step;

void *threadForDispersion(void * arg);
void *threadForAverage(void * arg);
int joinThreads(int numberOfThreads, pthread_t *thids);
int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads);
void freeMemory(pthread_t * thids, Segment * segment);
int scanNumberOfThreads();
void generateData();
int startThreads(int numberOfThreads, pthread_t * thids, Segment * segment, function threadRoutine);

int main()
{
	int numberOfThreads = scanNumberOfThreads();
	step = AMOUNT_OF_NUMBERS / numberOfThreads;

	struct Segment * segment;
	pthread_t * thids;
	if(!allocateMemory(&thids, &segment, numberOfThreads))
	{
		printf("Allocate error\n");
		exit(-1);
	}

	generateData();

	double begin = clock();

	int result;

	result = startThreads(numberOfThreads, thids, segment, threadForAverage);
	CHECK("startThreads", result);
	result = joinThreads(numberOfThreads, thids);
	CHECK("joinThreads", result);
	average /= AMOUNT_OF_NUMBERS;

	result = startThreads(numberOfThreads, thids, segment, threadForDispersion);
	CHECK("startThreads", result);
	result = joinThreads(numberOfThreads, thids);
	CHECK("joinThreads", result);
	dispersion /= AMOUNT_OF_NUMBERS;

	printf("average = %f, dispersion = %f\n", average, dispersion);

	double end = clock();
	printf("time: %f\n", (end - begin) / CLOCKS_PER_SEC);

	freeMemory(thids, segment);

	return 0;
}

void *threadForAverage(void * arg)
{
	printf("In thread for average\n");
	struct Segment segment = *(struct Segment *)arg;

	int i;

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		average += array[i];

	printf("Good Bye from thread for average\n");
	pthread_exit(0);
}

void *threadForDispersion(void * arg)
{
	printf("In thread for dispersion\n");
	struct Segment segment = *(struct Segment *)arg;

	int i;

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		dispersion += (array[i] - average) * (array[i] - average);

	printf("Good Bye from thread for dispersion\n");
	pthread_exit(0);
}

int joinThreads(int numberOfThreads, pthread_t *thids)
{
	int i, result;

	for(i = 0; i < numberOfThreads; i++)
	{
		result = pthread_join(thids[i], (void **)NULL);

		if(result != 0)
			return -1;
	}

	return 0;
}

int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads)
{
	array = calloc(AMOUNT_OF_NUMBERS, sizeof(int));
	*thids = calloc(numberOfThreads, sizeof(pthread_t));
	*segment = (struct Segment *)calloc(numberOfThreads, sizeof(struct Segment));

	return segment && thids && array;
}

void freeMemory(pthread_t * thids, Segment * segment)
{
	free(thids);
	free(array);
	free(segment);
}

int scanNumberOfThreads()
{
	int numberOfThreads;

	printf("Enter number of threads\n");
	scanf("%d", &numberOfThreads);

	return numberOfThreads;
}

void generateData()
{
	srand(time(NULL));

	int i;
	for(i = 0; i < AMOUNT_OF_NUMBERS; i++)
		array[i] = rand() % MAX_RANDOM;

	printf("generated succesfull\n");
}

int startThreads(int numberOfThreads, pthread_t * thids, Segment * segment, function threadRoutine)
{
	int i, result;
	for(i = 0; i < numberOfThreads; i++)
	{
		segment[i].firstElement = i * step;
		segment[i].lastElement = (i + 1) * step - 1;

		result = pthread_create(&thids[i], NULL, threadRoutine, &segment[i]);
		if(result != 0)
		{
			return -1;
		}
	}

	return 0;
}
