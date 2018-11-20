#include"libs.h"
#include"configure.h"
#include"fifo.h"
#include"sem.h"
#include"synch.h"

key_t waitForWiper()
{
	int callboardFIFO = openOrCreateFIFO(CALLBOARD_FIFO);

	char id[SIZE_ID];
	getID(id);
	printf("%s\n", id);
	int result = writeID(callboardFIFO, id);
	CHECK("writeID", result);

	do
	{
		result = open(id, O_WRONLY);
	}
	while(result == -1);

	return getTheKey(id);
}

key_t waitForWasher()
{
	int callboardFIFO = openOrCreateFIFO(CALLBOARD_FIFO);

	char id[SIZE_ID];
	int result = readID(callboardFIFO, id);
	CHECK("readID", result);

	printf("%s\n", id);
	mkfifo(id, PERMISSION);
	open(id, O_RDONLY);

	return getTheKey(id);
}
