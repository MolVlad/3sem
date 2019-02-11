#include"libs.h"
#include"general_config.h"
#include"default_config.h"
#include"my_string.h"
#include"global_variables.h"
#include"server_connection.h"

int setConnect()
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
		CHECK("inet_aton", -1);
	}

	int result = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	return result;
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
		case RCV:
		case LIST_REQUEST:
		case END:
			header.loginSize = 0;
			header.passwordSize = 0;
			header.dataSize = 0;

			break;
	}

	result = write(sockfd, &header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
		CHECK("write header", -1);

	if(header.loginSize)
	{
		result = printStringToStream(sockfd, login);
		CHECK("printStringToStream login", result);
	}

	if(header.passwordSize)
	{
		result = printStringToStream(sockfd, password);
		CHECK("printStringToStream password", result);
	}

	if(header.dataSize)
	{
		result = printStringToStream(sockfd, data);
		CHECK("printStringToStream data", result);
	}

	deleteString(login);
	deleteString(password);
	deleteString(data);
}

void logOut(int socketFd)
{
	sendViaNet(END);
	close(socketFd);
	printf("Log out\n");
}
