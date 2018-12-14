#ifndef __GENERAL_CONFIG_H__
#define __GENERAL_CONFIG_H__

#define PORT 51000

#define FILE_FOR_KEY "txt/key.txt"
#define GENERAL_FIFO "txt/general_fifo"
#define PRIVATE_FIFO_NAME_SIZE 6
#define FILE_LIST "txt/list.txt"

#define PERMISSION 0666

enum MessageType
{
	LOGIN = 0,
	REG,
	MSG,
	RCV,
	LIST_REQUEST,
	END,
};

typedef struct
{
	enum MessageType type;

	int loginSize;
	int passwordSize;
	int dataSize;
} HeaderMessageStruct;

enum ReverseMessageType
{
	ACK = 0,
	NACK,
	LIST,
	MESSAGE,
};

typedef struct
{
	enum ReverseMessageType type;

	int dataSize;
} HeaderReverseMessageStruct;

typedef enum Flag
{
	FALSE = 0,
	TRUE,
} Flag;

#define STDIN 1
#define STDOUT 0

#define CHECK(nameFunction, retValue)				\
do								\
{								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);				\
		kill(getpid(), SIGINT);				\
	}							\
}while(0)							\

#endif /* __GENERAL_CONFIG_H__ */
