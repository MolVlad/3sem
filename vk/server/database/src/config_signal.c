#include"libs.h"
#include"my_string.h"
#include"fifo_config.h"
#include"htable.h"
#include"btree.h"
#include"general_config.h"
#include"global.h"
#include"config_signal.h"

void configSignal()
{
	(void) signal(SIGINT, sigHandler);
}

void sigHandler(int nsig)
{
	printf("Exit from handler with signal SIGINT, nsig = %d\n", nsig);
	saveHTable(htableMap, HTABLE_STORAGE);
	CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
	remove(GENERAL_FIFO);
	close(generalFifo);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);
	deleteString(stringForGeneralFifo);

	exit(0);
}
