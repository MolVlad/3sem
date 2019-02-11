#ifndef __HANDLER_STATE_MACHINES_H__
#define __HANDLER_STATE_MACHINES_H__

#include"server_msg_types.h"
#include"my_string.h"

int parseType(String * string);
void handleRequest(enum MessageType type);

#endif /* __HANDLER_STATE_MACHINES_H__ */
