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
int msgidForAnswers;
int msgidForMessages;
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

enum MessageType parseType(String * string)
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

	int ret = atoi(pid->data);

	deleteString(pid);

	return ret;
}

void replyWithMSG(int pid, Flag isOK)
{
	if(isOK == TRUE)
		printf("handler true\n");
	else
		printf("handler false\n");
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
			printf("handler type login\n");
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
					insertToBTree(btreeMap, convertToBTreeData(login, getpid()));
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
			printf("handler type reg\n");
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
			}
			else
			{
				isOK = FALSE;
			}

			replyWithMSG(pid, isOK);

			break;
		case MSG:
			printf("handler type msg\n");
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
			printf("handler type end\n");
			result = scanStringFromStream(fifo, login, -1);
			CHECK("scanStringFromStream", result);
			deleteFromBTree(btreeMap, login);
			break;
		default:
			printf("handle request error: wrong type\n");
			exit(1);
	}

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

	btreeMap = createBTree();
	assert(btreeMap);

	key_t firstKey = getTheKey(FIRST_FILE_FOR_KEY);
	key_t secondKey = getTheKey(SECOND_FILE_FOR_KEY);

	semid = createSem(firstKey, NUM_OF_SEM);
	msgidForAnswers = createMsg(firstKey);
	msgidForMessages = createMsg(secondKey);

	fifo = createFIFO(FIFO);

	String * string = createString();
	int result;
	while(1)
	{
		result = scanStringFromStream(fifo, string, -1);
		CHECK("scanStringFromStream", result);
		printf("handler:\n");
		result = printStringToStream(STDOUT, string);
		CHECK("printStringToStream", result);
		printf("\n");

		handleRequest(parseType(string));
		clearString(string);
	}

	deleteString(string);



	/*
	//для регистрации

	//для входа
*/




	return 0;
}
