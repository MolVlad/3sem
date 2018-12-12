#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"config_serv_hand.h"
#include"my_string.h"
#include"handler_communication.h"
#include"sem.h"
#include"global.h"
#include"my_string.h"

String * pidToString()
{
	String * ret = createString();

	int i, del = 1000000, pid = getpid();
	char c;
	for(i = 0; i < 7; i++)
	{
		c = pid / del + '0';
		pid = pid % del;
		del = del / 10;
		putInString(ret, &c);
	}

	return ret;
}
////////////здесь происходит синхронизация семафорами
////////////печататаем в fifo в нужном формате и затем ждем в очереди сообщений ответа
////////////ждем ответа в очереди сообщений
Flag sendToHandler(String * recipient, String * data)
{
	Flag isOK = TRUE;

	sendToPipe(MSG, recipient, NULL, data);

	return isOK;
}

Flag createAccount(String * login, String * password)
{
	Flag isOK = TRUE;

	printf("before send to pipe\n");
	sendToPipe(REG, login, password, NULL);
	printf("after send to pipe\n");

	//semOperation

	return isOK;
}

Flag checkAccount(String * login, String * password)
{
	Flag isOK = TRUE;

	sendToPipe(LOGIN, login, password, NULL);

	return isOK;
}

void sendToPipe(enum MessageType type, String * login, String * password, String * data)
{
	int result;

	switch(type)
	{
		case REG:
			result = write(fifo, "reg\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}
			break;
		case LOGIN:
			result = write(fifo, "login\n", 6);
			if(result != 6)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		case MSG:
			result = write(fifo, "msg\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		case END:
			result = write(fifo, "end\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		default:
			printf("sendToPipe type error\n");
			exit(-1);
	}

	result = printStringToStream(fifo, stringPid);
	CHECK("printStringToStream", result);
	write(fifo, "\n", 1);

	if(login)
	{
		result = printStringToStream(fifo, login);
		CHECK("printStringToStream", result);
		write(fifo, "\n", 1);
	}

	if(password)
	{
		result = printStringToStream(fifo, password);
		CHECK("printStringToStream", result);
		write(fifo, "\n", 1);
	}

	if(data)
	{
		result = printStringToStream(fifo, data);
		CHECK("printStringToStream", result);
	}
}
