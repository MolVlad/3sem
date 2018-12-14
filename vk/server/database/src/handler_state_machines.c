#include"libs.h"
#include"my_string.h"
#include"handler_state_machines.h"
#include"server_msg_types.h"
#include"fifo_config.h"
#include"btree.h"
#include"sending_instruction_by_handler.h"
#include"global.h"
#include"general_config.h"
#include"htable.h"

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
