#ifndef __STATE_H__
#define __STATE_H__

enum ServerState
{
	INIT = 0,
	WAITING_REQUESTS,
};

Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd, int fifo);

#endif /* __STATE_H__ */
