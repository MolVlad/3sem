#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include"general_config.h"
#include"my_string.h"

extern pthread_t thid;
extern int generalFifo;
extern int semid;
extern int msgidForAnswer;
extern int msgidForTransmission;
extern String * stringPid;
extern char privateFifoName[PRIVATE_FIFO_NAME_SIZE];
extern String * userLogin;

#endif /* __GLOBAL_H__ */
