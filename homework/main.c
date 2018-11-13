#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define AMOUNT_OF_NUMBERS 100000000
#define MAX_RANDOM 1000

typedef struct Segment
{
	int firstElement;
	int lastElement;
} Segment;

float average = 0, dispersion = 0;
int completedThreads = 0;
int isAverageReady = 0;
int * array, * sum, *deviations;
int step;

void joinThreads(int numberOfThreads, pthread_t *thids);
void *thread(void * arg);
int allocateMemory(pthread_t ** thids, Segment ** segment, int numberOfThreads);
void freeMemory(pthread_t * thids, Segment * segment);
int scanNumberOfThreads();
void generateData();
int startThreads(int numberOfThreads, pthread_t * thids, Segment * segment);

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
	int i, result;

	startThreads(numberOfThreads, thids, segment);

	while(completedThreads != numberOfThreads);

	average /= AMOUNT_OF_NUMBERS;
	isAverageReady = 1;

	joinThreads(numberOfThreads, thids);
	freeMemory(thids, segment);

	dispersion /= AMOUNT_OF_NUMBERS;
	printf("average = %f, dispersion = %f\n", average, dispersion);

	double end = clock();
	printf("time: %f\n", (end - begin) / CLOCKS_PER_SEC);

	return 0;
}

void joinThreads(int numberOfThreads, pthread_t *thids)
{
	int i, result;

	for(i = 0; i < numberOfThreads; i++)
	{
		result = pthread_join(thids[i], (void **)NULL);

		if(result != 0)
		{
			printf("Error on thread join, return value = %d\n", result);
			exit(-1);
		}
	}
}

void *thread(void * arg)
{
	printf("In thread\n");
	struct Segment segment = *(struct Segment *)arg;

	int i;

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		average += array[i];

	// FIXIT: у вас несколько потоков увеличивают на 1 completedThreads ... это состояние гонки
	// Вы можете сделать синхронизацию через семафоры, либо просто разбить эту ф-ю на две:
	// одна для среднего, вторая для дисперсии
	completedThreads++;
	while(!isAverageReady);

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		dispersion += (array[i] - average) * (array[i] - average);

	printf("Good Bye\n");
	pthread_exit(0);
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

int startThreads(int numberOfThreads, pthread_t * thids, Segment * segment)
{
	int i, result;
	for(i = 0; i < numberOfThreads; i++)
	{
		segment[i].firstElement = i * step;
		segment[i].lastElement = (i + 1) * step - 1;

		result = pthread_create(&thids[i], NULL, thread, &segment[i]);
		if(result != 0)
		{
			return -1;
		}
	}
<<<<<<< HEAD
=======

	dispersion /= AMOUNT_OF_NUMBERS;

	free(thids);
	free(array);
	free(segment);

	printf("average = %f, dispersion = %f\n", average, dispersion);

	double end = clock();

	// FIXIT: приведите, пожалуйста, в комментарии в начале кода данные с измеренным ускорением. в последнем письме писал про wall-clock time и user_sys time
	// Вам нужно первое.
	printf("time: %f\n", (end - begin) / CLOCKS_PER_SEC);

	return 0;
>>>>>>> c66627d7c8f1663fa92e395ab50008fe89ff5c0f
}
