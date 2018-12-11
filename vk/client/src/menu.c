#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"menu.h"
#include"print.h"
#include"global.h"

void setConnect()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(ip, &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(-1);
	}

	int result = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("connect", result);
}

//scan header from user to know sizes of blocks
int scanHeaderReverse(int fd, HeaderReverseMessageStruct * header)
{
	int result = read(fd, header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("Scan header reverse error. Result = %d\n", result);
		return -1;
	}

	return 0;
}

Flag receiveAnswer()
{
	Flag ret;

	HeaderReverseMessageStruct header;
	int result = scanHeaderReverse(sockfd, &header);
	if(result == -1)
	{
		printf("Connection error: \n");
		printf("failed to accept header\n");
		exit(-1);
	}

	String * data;

	switch(header.type)
	{
		case ACK:
			ret = TRUE;

			//it should be equal to zero in the init message (there are no data)
			if(header.dataSize != 0)
			{
				printf("ACK error, there are data\n");
				break;
			}

			break;
		case NACK:
			ret = FALSE;

			//it should be equal to zero in the init message (there are no data)
			if(header.dataSize != 0)
			{
				printf("NACK error, there are data\n");
				break;
			}

			break;
		case LIST:
			data = createString();
			result = scanStringFromStream(sockfd, data, header.dataSize);
			CHECK("scanStringFromStream", result);
			printf("List of users online:\n");
			printStringToStream(STDOUT, data);

			break;
	}

	return ret;
}

void sendViaNet(enum MessageType type)
{
	int result;

	HeaderMessageStruct header;
	bzero(&header, sizeof(HeaderMessageStruct));
	header.type = type;

	String * login = NULL;
	String * password = NULL;
	String * data = NULL;

	switch(type)
	{
		case REG:
		case LOGIN:
			printf("login:\n");
			login = createString();
			result = scanStringFromStream(STDIN, login, -1);
			CHECK("scanStringFromStream login", result);
			header.loginSize = login->currentSize;

			printf("password:\n");
			password = createString();
			result = scanStringFromStream(STDIN, password, -1);
			CHECK("scanStringFromStream password", result);
			header.passwordSize = password->currentSize;

			header.dataSize = 0;

			break;
		case MSG:
			printf("recipient:\n");
			login = createString();
			result = scanStringFromStream(STDIN, login, -1);
			CHECK("scanStringFromStream login", result);
			header.loginSize = login->currentSize;

			header.passwordSize = 0;

			printf("data:\n");
			data = createString();
			assert(data);
			result = scanTextFromStream(STDIN, data, -1);
			CHECK("scanStringFromStream", result);
			header.dataSize = data->currentSize;

			break;
		case LIST_REQUEST:
		case END:
			header.loginSize = 0;
			header.passwordSize = 0;
			header.dataSize = 0;

			break;
	}

	printf("before header\n");

	result = write(sockfd, &header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
	{
		printf("write header error. result = %d\n", result);
		exit(-1);
	}

	printf("after header before login\n");

	if(header.loginSize)
	{
		result = printStringToStream(sockfd, login);
		CHECK("printStringToStream login", result);
	}

	printf("before password\n");

	if(header.passwordSize)
	{
		result = printStringToStream(sockfd, password);
		CHECK("printStringToStream password", result);
	}

	printf("before data\n");

	if(header.dataSize)
	{
		result = printStringToStream(sockfd, data);
		CHECK("printStringToStream data", result);
	}

	printf("after data\n");

	deleteString(login);
	deleteString(password);
	deleteString(data);
}

void logOut(int socketFd)
{
	printLogOut();
	sendViaNet(END);
	close(socketFd);
}

Flag checkAccount()
{
	sendViaNet(LOGIN);
	return receiveAnswer();
}

Flag createAccount()
{
	sendViaNet(REG);
	return receiveAnswer();
}

Flag sendMessage()
{
	sendViaNet(MSG);
	return receiveAnswer();
}

void userList()
{
	sendViaNet(LIST_REQUEST);
	receiveAnswer();
}
