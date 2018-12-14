#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"sem.h"
#include"msg.h"
#include"fifo.h"

HTableMap * htableMap;
BTreeMap * btreeMap;
int semid;
int msgidForAnswer;
int msgidForTransmission;
int generalFifo;
char privateFifoName[PRIVATE_FIFO_NAME_SIZE];
int privateFifo;
String * stringForGeneralFifo;

void increasePrivateFifoName(char * privateFifoName);

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

int parseType(String * string)
{
	if(strcmp(string->data, "login") == 0)
		return LOGIN;
	if(strcmp(string->data, "reg") == 0)
		return REG;
	if(strcmp(string->data, "msg") == 0)
		return MSG;
	if(strcmp(string->data, "end") == 0)
		return END;

	return -1;
}

int scanPid()
{
	String * pid = createString();

	int result = scanStringFromStream(generalFifo, pid, -1);
	CHECK("scanStringFromStream", result);

	int ret = atoi(pid->data);

	deleteString(pid);

	return ret;
}

void sendToRecipientPrivateFifoName(int pid)
{
	struct MsgBufTransmission bufTransmission;
	bufTransmission.type = pid;

	strcpy(bufTransmission.data.privateFifo, privateFifoName);

	int result = msgsnd(msgidForTransmission, (struct msgbuf *)&bufTransmission, sizeof(struct DataForTransmission), 0);
	CHECK("msgsnd", result);
}

void sendToSenderPrivateFifoName(int pid, Flag isOK)
{
	struct MsgBufAnswer bufAnswer;
	bufAnswer.type = pid;
	bufAnswer.data.ACK = isOK;

	if(isOK == TRUE)
		strcpy(bufAnswer.data.privateFifo, privateFifoName);

	int result = msgsnd(msgidForAnswer, (struct msgbuf *)&bufAnswer, sizeof(struct DataForAnswer), 0);
	CHECK("msgsnd", result);
}

void replyWithMSG(int pid, Flag isOK)
{
	struct MsgBufAnswer bufAnswer;
	bufAnswer.type = pid;
	bufAnswer.data.ACK = isOK;

	int result = msgsnd(msgidForAnswer, (struct msgbuf *)&bufAnswer, sizeof(struct DataForAnswer), 0);
	CHECK("msgsnd", result);
}

void handleRequest(enum MessageType type)
{
	int pid = scanPid();
	int result;

	String * login = createString();
	String * password = createString();
	String * data = createString();

	HTableData * desired;

	Flag isOK = TRUE;

	switch(type)
	{
		case LOGIN:
			result = scanStringFromStream(generalFifo, login, -1);
			CHECK("scanStringFromStream", result);

			result = scanStringFromStream(generalFifo, password, -1);
			CHECK("scanStringFromStream", result);

			desired = findInHTable(htableMap, login);
			if(desired == NULL)
			{
				isOK = FALSE;
			}
			else if(stringCompare(desired->password, password) == 0)
			{
				BTreeData * same = findInBTree(btreeMap, login);
				if(same == NULL)
				{
					isOK = TRUE;
					insertToBTree(btreeMap, convertToBTreeData(login, pid));
					saveBTree(btreeMap, FILE_LIST);
				}
				else
					isOK = FALSE;
			}
			else
			{
				isOK = FALSE;
			}

			replyWithMSG(pid, isOK);

			break;
		case REG:
			result = scanStringFromStream(generalFifo, login, -1);
			CHECK("scanStringFromStream", result);

			result = scanStringFromStream(generalFifo, password, -1);
			CHECK("scanStringFromStream", result);

			desired = findInHTable(htableMap, login);
			if(desired == NULL)	//if not founded means can create with this login
			{
				isOK = TRUE;
				insertToHTable(htableMap, convertToHTableData(login, password));
				insertToBTree(btreeMap, convertToBTreeData(login, pid));
				saveBTree(btreeMap, FILE_LIST);
			}
			else
			{
				isOK = FALSE;
			}

			replyWithMSG(pid, isOK);

			break;
		case MSG:
			result = scanStringFromStream(generalFifo, login, -1);
			CHECK("scanStringFromStream", result);

			printf("login:\n");
			result = printStringToStream(STDOUT, login);
			CHECK("printStringToStream", result);
			printf("\n");

			BTreeData * recipient = findInBTree(btreeMap, login);
			if(recipient != NULL)
			{
				increasePrivateFifoName(privateFifoName);
				privateFifo = createFIFO(privateFifoName);
				isOK = TRUE;
				sendToRecipientPrivateFifoName(recipient->pid);
			}
			else
				isOK = FALSE;

			sendToSenderPrivateFifoName(pid, isOK);

			break;
		case END:
			result = scanStringFromStream(generalFifo, login, -1);
			CHECK("scanStringFromStream", result);
			deleteFromBTree(btreeMap, login);
			saveBTree(btreeMap, FILE_LIST);
			break;
		default:
			printf("handle request error: wrong type\n");
			replyWithMSG(pid, FALSE);
	}

	printStringToStream(STDOUT, login);
	write(STDOUT, "\n", 1);
	printStringToStream(STDOUT, password);
	write(STDOUT, "\n", 1);
	printStringToStream(STDOUT, data);
	write(STDOUT, "\n", 1);

	deleteString(login);
	deleteString(password);
	deleteString(data);
}

void increasePrivateFifoName(char * privateFifoName)
{
	int size = strlen(privateFifoName);
	int i;
	for(i = size - 1; i >=0; i--)
	{
		if(privateFifoName[i] < '9')
		{
			privateFifoName[i]++;
			return;
		}
		else
			privateFifoName[i] = '0';
	}

	memset(privateFifoName, '0', size);
}

int main()
{
	memset(privateFifoName, '0', PRIVATE_FIFO_NAME_SIZE);

	printf("Handler is running\n");
	(void) signal(SIGINT, sigHandler);

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

	msgidForAnswer = createMsg(firstKey);
	msgidForTransmission = createMsg(secondKey);

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
