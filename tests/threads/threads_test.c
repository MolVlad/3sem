#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int a = 0;

void *mythread(void * arg)
{
	pthread_t mythid = pthread_self();

	a = a + 1;

	printf("Thread %d, Calculation result = %d\n", mythid, a);

	return NULL;
}

int main()
{
	pthread_t mythid;

	mythid = pthread_self();
	a = a + 1;

	printf("Thread %d, Calculation relust = %d\n", mythid, a);

	pthread_t thid;
	int result = pthread_create(&thid, (pthread_attr_t *)NULL, mythread, NULL);
	if(result != 0)
	{
		printf("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
	printf("Thread created, thid = %d\n", thid);

	pthread_t newthid;
	result = pthread_create(&newthid, (pthread_attr_t *)NULL, mythread, NULL);
	if(result != 0)
	{
		printf("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
	printf("Thread created, thid = %d\n", newthid);

	pthread_join(newthid, (void **)NULL);
	pthread_join(thid, (void **)NULL);

	return 0;
}
