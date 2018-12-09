#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

int sendResponce(Flag flag, int newsockfd);
void responceViaNet(enum ReverseMessageType type, int newsockfd);
int scanHeader(HeaderMessageStruct * header, int newsockfd);
Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd);

#endif /* __COMMUNICATION_H__ */
