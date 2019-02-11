#ifndef __SERVER_FEEDBACK_H__
#define __SERVER_FEEDBACK_H__

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


Flag receiveAnswer();
int scanHeaderReverse(int fd, HeaderReverseMessageStruct * header);

#endif /* __SERVER_FEEDBACK_H__ */
