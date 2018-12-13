#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"config_serv_hand.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"sem.h"
#include"msg.h"
#include"fifo.h"

HTableMap * htableMap;
BTreeMap * btreeMap;
int semid;
int msgid;
int fifo;

void sigHandler(int nsig)
{
	printf("Exit from handler with signal SIGINT, nsig = %d\n", nsig);
	saveHTable(htableMap, HTABLE_STORAGE);
	CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
	remove(FIFO);
	printHTable(htableMap);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

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

	int result = scanStringFromStream(fifo, pid, -1);
	CHECK("scanStringFromStream", result);
	printStringToStream(STDOUT, pid);
	write(STDOUT, "\n", 1);

	int ret = atoi(pid->data);

	deleteString(pid);

	return ret;
}

void replyWithMSG(int pid, Flag isOK)
{
	struct MsgBufAnswer bufAnswer;
	bufAnswer.type = pid;
	bufAnswer.data.ACK = isOK;

	int result = msgsnd(msgid, (struct msgbuf *)&bufAnswer, sizeof(struct DataForAnswer), 0);
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
			result = scanStringFromStream(fifo, login, -1);
			CHECK("scanStringFromStream", result);

			result = scanStringFromStream(fifo, password, -1);
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
			result = scanStringFromStream(fifo, login, -1);
			CHECK("scanStringFromStream", result);

			result = scanStringFromStream(fifo, password, -1);
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
			result = scanStringFromStream(fifo, login, -1);
			CHECK("scanStringFromStream", result);

			result = scanTextFromStream(fifo, data, -1);
			CHECK("scanTextFromStream", result);

			printf("login:\n");
			result = printStringToStream(STDOUT, login);
			CHECK("printStringToStream", result);
			printf("\n");
			printf("data:\n");
			result = printStringToStream(STDOUT, data);
			CHECK("printStringToStream", result);
			printf("\n");

			isOK = TRUE;
			replyWithMSG(pid, isOK);

			break;
		case END:
			result = scanStringFromStream(fifo, login, -1);
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

int main()
{
	printf("Handler is running\n");
	(void) signal(SIGINT, sigHandler);

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	key_t key = getTheKey(FILE_FOR_KEY);

	fifo = createFIFO(FIFO);

	btreeMap = createBTree();
	assert(btreeMap);
	saveBTree(btreeMap, FILE_LIST);
	printBTree(btreeMap);

	semid = createSem(key, NUM_OF_SEM);
	semOperation(semid, fifoSynch, 1);
	semOperation(semid, listSynch, 1);

	msgid = createMsg(key);

	String * string = createString();
	int result;
	int type;
	while(1)
	{
		do
		{
			result = scanStringFromStream(fifo, string, -1);
			CHECK("scanStringFromStream", result);
			printStringToStream(STDOUT, string);
			write(STDOUT, "\n", 1);

			type = parseType(string);
			if(type == -1)
				printf("handler error type\n");
		} while(type < 0);

		handleRequest(type);
	}

	deleteString(string);

	return 0;
}
