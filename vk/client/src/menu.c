#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"menu.h"

extern int sockfd;

void sendViaNet(enum MessageType type)
{
	int result;

	HeaderMessageStruct header;
	bzero(&header, sizeof(HeaderMessageStruct));
	header.type = type;

	String * login, * password, * data;

	switch(type)
	{
		case REG:
		case LOGIN:
			printf("login:\n");
			login = createString();
			assert(login);
			result = scanStringFromStream(STDIN, login, -1);
			CHECK("scanStringFromStream", result);
			header.loginSize = login->currentSize;

			printf("password:\n");
			password = createString();
			assert(password);
			result = scanStringFromStream(STDIN, password, -1);
			CHECK("scanStringFromStream", result);
			header.passwordSize = password->currentSize;

			header.dataSize = 0;

			break;
		case MSG:
			printf("login:\n");
			login = createString();
			assert(login);
			result = scanStringFromStream(STDIN, login, -1);
			CHECK("scanStringFromStream", result);
			header.loginSize = login->currentSize;

			header.passwordSize = 0;

			printf("data:\n");
			data = createString();
			assert(data);
			result = scanStringFromStream(STDIN, data, -1);
			CHECK("scanStringFromStream", result);
			header.dataSize = data->currentSize;

			break;
	}

	result = write(sockfd, &header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
	{
		printf("write header error\n");
		exit(0);
	}

	if(header.loginSize)
	{
		result = printStringToStream(sockfd, login);
		CHECK("printStringToStream login", result);
		deleteString(login);
	}

	if(header.passwordSize)
	{
		result = printStringToStream(sockfd, password);
		CHECK("printStringToStream password", result);
		deleteString(password);
	}

	if(header.dataSize)
	{
		result = printStringToStream(sockfd, data);
		CHECK("printStringToStream data", result);
		deleteString(data);
	}
}

Flag checkAccount()
{
	sendViaNet(LOGIN);
	return TRUE;
}

Flag createAccount()
{
	sendViaNet(REG);
	return TRUE;
}

void sendMessage()
{
	sendViaNet(MSG);
}
void readDialogue()
{

}
void userList()
{

}

void deleteMessage()
{

}
void clearHistory()
{

}
