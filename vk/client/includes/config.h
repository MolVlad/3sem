#ifndef __CONFIG_H__
#define __CONFIG_H__

#define PORT 51000
#define DEFAULT_IP "127.0.0.1"

#define SIZE_IP 16
#define STDIN 1
#define STDOUT 0

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

#define CHECK(nameFunction, retValue)				\
do								\
{								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);				\
		kill(getpid(), SIGINT);				\
	}							\
}while(0)							\

#endif /* __CONFIG_H__ */
