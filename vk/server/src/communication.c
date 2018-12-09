#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"communication.h"
#include"btree.h"
#include"htable.h"

int sendResponce(Flag flag, int newsockfd)
{
	printf("send responce: flag = %d\n", flag);

	if(flag == TRUE)
		responceViaNet(ACK,newsockfd);
	else
		responceViaNet(NACK, newsockfd);

	return 0;
}

void responceViaNet(enum ReverseMessageType type, int newsockfd)
{
	int result;

	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = type;

	String * data;

	switch(type)
	{
		case ACK:
		case NACK:
			header.dataSize = 0;
			break;
	}

	result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	CHECK("write to sockfd", result);
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("write header error\n");
		exit(0);
	}

	if(header.dataSize)
	{
		data = createString();
		result = printStringToStream(newsockfd, data);
		CHECK("printStringToStream data", result);
		deleteString(data);
	}
}

//scan header from user to know sizes of blocks
int scanHeader(HeaderMessageStruct * header, int newsockfd)
{
	int result = read(newsockfd, header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
		return -1;

	printf("\nMessage!\nHeader:\n");
	printf("type = %d, login size = %d\n", header->type, header->loginSize);
	printf("password size = %d, data size = %d\n", header->passwordSize, header->dataSize);

	return 0;
}

Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd)
{
	static enum ServerState state = INIT;
	static Flag isAll = FALSE;
	int result;

	if(header->type == END)
	{
		printf("End of communication\n");
		isAll = TRUE;
		return isAll;
	}

	//strings for blocks
	String * login = createString();
	String * password = createString();
	String * data = createString();

	printf("login:\n");
	result = stringGetAndPrint(newsockfd, login, header->loginSize);
	CHECK("stringGetAndCheck login", result);

	printf("password:\n");
	result = stringGetAndPrint(newsockfd, password, header->passwordSize);
	CHECK("stringGetAndCheck password", result);

	printf("data:\n");
	result = stringGetAndPrint(newsockfd, data, header->dataSize);
	CHECK("stringGetAndCheck password", result);

	switch(state)
	{
		case INIT:
			//it should be equal to zero in the init message (there are no data)
			if(header->dataSize != 0)
			{
				printf("Init error\n");
				break;
			}

			HTableData * desired;

			switch(header->type)
			{
				case REG:	//case with registration
					printf("\nUser want to register!!\n");

					desired = findInHTable(htableMap, login);
					if(desired == NULL)	//if not founded means can create with this login
					{
						state = WAITING_REQUESTS;
						insertToHTable(htableMap, convertToHTableData(login, password));
						insertToBTree(btreeMap, convertToBTreeData(login, "ip", getpid()));
						sendResponce(TRUE, newsockfd);
						printf("Reg success\n");
					}
					else
					{
						printf("Reg error: already exists\n");
						sendResponce(FALSE, newsockfd);
					}
					break;
				case LOGIN:	//case with login
					printf("\nUser want to login!!\n");

					desired = findInHTable(htableMap, login);
					printf("kek\n");

					if(desired == NULL)
					{
						sendResponce(FALSE, newsockfd);
						printf("Login error: not founded\n");
					}
					else if(stringCompare(desired->password, password) == 0)
					{
						state = WAITING_REQUESTS;	//founded and password is correct
						insertToBTree(btreeMap, convertToBTreeData(login, "ip", getpid()));
						sendResponce(TRUE, newsockfd);
						printf("Login success\n");
					}
					else
					{
						printf("Login error: wrong password\n");
						sendResponce(FALSE, newsockfd);
					}

						break;
				default:
					printf("Wrond msg type for init state\n");
					break;
			}

			break;
		case WAITING_REQUESTS:
			switch(header->type)
			{
				case MSG:
					printf("message:\n");
					printStringToStream(STDOUT, data);
					break;
				default:
					break;
			}

			break;
		default:
			printf("server state error\n");
			exit(1);
	}

	deleteString(login);
	deleteString(password);
	deleteString(data);

	return isAll;
}
