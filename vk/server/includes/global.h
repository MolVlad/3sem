#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include"config_serv_hand.h"

extern int generalFifo;
extern int semid;
extern int msgidForAnswer;
extern int msgidForTransmission;
extern String * stringPid;
extern String * userLogin;
extern char privateFifoName[PRIVATE_FIFO_NAME_SIZE];

#endif /* __GLOBAL_H__ */
