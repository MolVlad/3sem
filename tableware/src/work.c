#include"libs.h"
#include"configure.h"
#include"list.h"
#include"sem.h"
#include"thread.h"
#include"work.h"

/* Function for raise the flag that is all */
void sayThatIsAll(Data * pieceOfMemory, int semid)
{
	pieceOfMemory->itIsAll = 1;
	semOperation(semid, allowedToReadFromMemory, 1);
}

/* Function for thread for transmit tableware parallel to washing */
void *tranmsitTableware(void * arg)
{
	printf("In thread for transmit:\n");
	Segment * segment = (Segment *)arg;

	printData(segment->data);

	semOperation(segment->semid, readyToReadFromMemory, -1);
	putInMemory(segment->pieceOfMemory, segment->data);
	semOperation(segment->semid, allowedToReadFromMemory, 1);

	pthread_exit(0);
}

/* Delay for wipe */
void wipeElement(Data * data)
{
	printf("\t\t\t\t\tWipe %s for %d seconds\n", data->name, data->timeToWipe);

	sleep(data->timeToWipe);
}

/* Delay for wash */
void washingElement(Data * data)
{
	printf("\t\t\t\t\tCleaning %s for %d seconds\n", data->name, data->timeToWash);

	sleep(data->timeToWash);
}
