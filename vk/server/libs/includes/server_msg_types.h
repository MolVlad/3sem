#ifndef __SERVER_MESSAGE_TYPES_H__
#define __SERVER_MESSAGE_TYPES_H__

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

#endif /* __SERVER_MESSAGE_TYPES_H__ */
