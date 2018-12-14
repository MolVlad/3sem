#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include"general_config.h"
#include"my_string.h"
#include"fifo_config.h"

extern pthread_t thid;
extern int handlerPid;
extern int generalFifo;
extern int semid;
extern int msgidForAnswer;
extern int msgidForTransmission;
extern int sockfd;
extern String * stringPid;
extern String * userLogin;
extern char privateFifoName[PRIVATE_FIFO_NAME_SIZE];

#endif /* __GLOBAL_H__ */
