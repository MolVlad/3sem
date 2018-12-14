#ifndef __SERVER_STATE_MACHINES_H__
#define __SERVER_STATE_MACHINES_H__

#include"server_msg_types.h"

enum ServerState
{
	INIT = 0,
	WAITING_REQUESTS,
};

Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd);

#endif /* __SERVER_STATE_MACHINES_H__ */
