#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"user_communication.h"
#include"fifo.h"
#include"sem.h"
#include"global.h"

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
			exit(-1);
			break;
	}

	int result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("write header error. result = %d\n", result);
		exit(-1);
	}
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
	//////////////////////чтение от другого процесса. ???
	String * data = createString();
	printf("Print message:\n");
	int result = scanTextFromStream(STDIN, data, -1);
	CHECK("scanTextFromStream data", result);
///////////////////////////////////////////
	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = MESSAGE;
	header.dataSize = data->currentSize;

	result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("write header error. result = %d\n", result);
		exit(-1);
	}

	result = printStringToStream(newsockfd, data);
	CHECK("printStringToStream data", result);

	deleteString(data);
}

void sendList(int newsockfd)
{
	String * data = createString();

	int list = open(FILE_LIST, O_RDWR);
	CHECK("open list", list);

	semOperation(semid, listSynch, -1);
	int result = scanTextFromStream(list, data, -1);
	CHECK("scanTextFromStream data", result);
	semOperation(semid, listSynch, 1);

	HeaderReverseMessageStruct header;
	bzero(&header, sizeof(HeaderReverseMessageStruct));
	header.type = LIST;
	header.dataSize = data->currentSize;

	result = write(newsockfd, &header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("Header write error. result = %d\n", result);
		exit(0);
	}

	result = printStringToStream(newsockfd, data);
	CHECK("printStringToStream data", result);

	close(list);
	deleteString(data);
}
