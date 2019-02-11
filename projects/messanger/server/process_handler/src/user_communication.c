#include"libs.h"
#include"general_config.h"
#include"default_config.h"
#include"my_string.h"
#include"user_communication.h"
#include"database_communication.h"
#include"fifo_config.h"
#include"sem_config.h"
#include"global_variables.h"

int raiseServer()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("bind", result);

	result = listen(sockfd, BACKLOG);
	CHECK("listen", result);
	printf("Let's listen\n");

	return sockfd;
}

int sendReply(Flag flag, int newsockfd)
{
	if(flag == TRUE)
		replyViaNet(ACK,newsockfd);
	else
		replyViaNet(NACK, newsockfd);

	return 0;
}

void replyViaNet(enum ReverseMessageType type, int newsockfd)
{
	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = type;

	switch(type)
	{
		case ACK:
		case NACK:
			header.dataSize = 0;
			break;
		case LIST:
		case MESSAGE:
			printf("error: replyViaNet with type = %d\n", type);
			CHECK("replyViaNet", -1);
			break;
	}

	int result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
		CHECK("write header", -1);
}

//scan header from user to know sizes of blocks
int scanHeader(HeaderMessageStruct * header, int newsockfd)
{
	int result = read(newsockfd, header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
	{
		printf("Read header error. result = %d\n", result);
		return -1;
	}

	printf("Message. Header: ");
	printf("type = %d, login size = %d, ", header->type, header->loginSize);
	printf("password size = %d, data size = %d\n", header->passwordSize, header->dataSize);

	return 0;
}

void sendMessage(int newsockfd)
{
	int result;
	String * data = createString();

	Flag isOK = getPrivateFifoToRead();
	if(isOK == TRUE)
	{
		int privateFifoToRead = openFIFO(privateFifoName);
		printf("let's scan!\n");
		result = scanTextFromStream(privateFifoToRead, data, -1);
		CHECK("scanTextFromStream data", result);
		close(privateFifoToRead);
		remove(privateFifoName);
	}

	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = MESSAGE;
	header.dataSize = data->currentSize;

	result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
		CHECK("write header", -1);

	result = printStringToStream(newsockfd, data);
	CHECK("printStringToStream data", result);

	deleteString(data);
}

void sendList(int newsockfd)
{
	String * data = createString();

	int list = open(FILE_LIST, O_RDWR);
	CHECK("open list", list);

	semOperation(semid, userListSynch, -1);
	int result = scanTextFromStream(list, data, -1);
	CHECK("scanTextFromStream data", result);
	semOperation(semid, userListSynch, 1);

	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = LIST;
	header.dataSize = data->currentSize;

	result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
		CHECK("write header", -1);

	result = printStringToStream(newsockfd, data);
	CHECK("printStringToStream data", result);

	close(list);
	deleteString(data);
}
