#ifndef __SERVER_CONNECTION_H__
#define __SERVER_CONNECTION_H__

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


int setConnect();
void sendViaNet(enum MessageType type);
void logOut(int socketFd);

#endif /* __SERVER_CONNECTION_H__ */
