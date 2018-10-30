#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define AMOUNT_OF_NUMBERS 100000000
#define MAX_RANDOM 1000

struct Segment
{
	int firstElement;
	int lastElement;
};

float average = 0, dispersion = 0;
int completedThreads = 0;
int isAverageReady = 0;
int * array, * sum, *deviations;

void *thread(void * arg)
{
	printf("In thread\n");
	struct Segment segment = *(struct Segment *)arg;

	int i;

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		average += array[i];

	completedThreads++;
	while(!isAverageReady);

	for(i = segment.firstElement; i <= segment.lastElement; i++)
		dispersion += (array[i] - average) * (array[i] - average);

	printf("Good Bye\n");
	pthread_exit(0);
}

int main()
{
	srand(time(NULL));

	array = calloc(AMOUNT_OF_NUMBERS, sizeof(int));

	int i;
	for(i = 0; i < AMOUNT_OF_NUMBERS; i++)
		array[i] = rand() % MAX_RANDOM;

	printf("generated succesfull\n");

	int numberOfThreads;
	printf("Enter number of threads\n");
	scanf("%d", &numberOfThreads);

	int step = AMOUNT_OF_NUMBERS / numberOfThreads;

	double begin = clock();

	int result;
	pthread_t * thids = malloc(numberOfThreads * sizeof(pthread_t));

	struct Segment * segment = (struct Segment *)malloc(numberOfThreads * sizeof(struct Segment));
	for(i = 0; i < numberOfThreads; i++)
	{
		segment[i].firstElement = i * step;
		segment[i].lastElement = (i + 1) * step - 1;

		result = pthread_create(&thids[i], NULL, thread, &segment[i]);
		if(result != 0)
		{
			printf("Error on thread create, return value = %d\n", result);
			exit(-1);
		}
	}

	while(completedThreads != numberOfThreads);

	average /= AMOUNT_OF_NUMBERS;
	isAverageReady = 1;

	for(i = 0; i < numberOfThreads; i++)
	{
		result = pthread_join(thids[i], (void **)NULL);

		if(result != 0)
		{
			printf("Error on thread join, return value = %d\n", result);
			exit(-1);
		}
	}

	dispersion /= AMOUNT_OF_NUMBERS;

	free(thids);
	free(array);
	free(segment);

	printf("average = %f, dispersion = %f\n", average, dispersion);

	double end = clock();

	printf("time: %f\n", (end - begin) / CLOCKS_PER_SEC);

	return 0;
}
