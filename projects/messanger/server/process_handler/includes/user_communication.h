#ifndef ___USER_COMMUNICATION_H__
#define __USER_COMMUNICATION_H__

//for socket
#define BACKLOG 1000

#include"server_msg_types.h"

int raiseServer();
int sendReply(Flag flag, int newsockfd);
void replyViaNet(enum ReverseMessageType type, int newsockfd);
int scanHeader(HeaderMessageStruct * header, int newsockfd);
void sendList(int newsockfd);
void sendMessage(int newsockfd);

#endif /* __USER_COMMUNICATION_H__ */
