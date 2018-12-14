#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"sem_config.h"
#include"msg_queue_config.h"
#include"fifo_config.h"
#include"server_msg_types.h"
#include"config_signal.h"
#include"handler_state_machines.h"

HTableMap * htableMap;
BTreeMap * btreeMap;
int semid;
int msgidForAnswer;
int msgidForTransmission;
int generalFifo;
char privateFifoName[PRIVATE_FIFO_NAME_SIZE];
int privateFifo;
String * stringForGeneralFifo;

int main()
{
	memset(privateFifoName, '0', PRIVATE_FIFO_NAME_SIZE);

	printf("Handler is running\n");
	configSignal();

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	generalFifo = createFIFO(GENERAL_FIFO);

	btreeMap = createBTree();
	assert(btreeMap);
	saveBTree(btreeMap, FILE_LIST);
	printBTree(btreeMap);

	key_t firstKey = getTheKey(FILE_FOR_KEY, 0);
	key_t secondKey = getTheKey(FILE_FOR_KEY, 1);

	msgidForAnswer = createMsgQueue(firstKey);
	msgidForTransmission = createMsgQueue(secondKey);

	semid = createSem(firstKey, NUM_OF_SEM);
	semOperation(semid, generalFifoSynch, 1);
	semOperation(semid, userListSynch, 1);

	stringForGeneralFifo = createString();
	int result;
	int type;
	while(1)
	{
		do
		{
			result = scanStringFromStream(generalFifo, stringForGeneralFifo, -1);
			CHECK("scanStringFromStream", result);

			type = parseType(stringForGeneralFifo);
			if(type == -1)
				printf("handler error type\n");
		} while(type < 0);

		handleRequest(type);
	}

	return 0;
}
