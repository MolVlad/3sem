#ifndef __GENERAL_CONFIG_H__
#define __GENERAL_CONFIG_H__

#define STDIN 1
#define STDOUT 0

//number of \n in the string after that it ends
#define NUM_SPACE_TO_BREAK 2

#define PORT 51001

#define FILE_LIST "../txt/list.txt"

#define PERMISSION 0777

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
		exit(1);					\
	}							\
}while(0)							\

#endif /* __GENERAL_CONFIG_H__ */
