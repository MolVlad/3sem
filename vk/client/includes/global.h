#ifndef __GLOBAL_H__
#define __GLOBAL_H__

extern int sockfd;
extern char * ip;
extern int semid;
extern pthread_t thid;

#include"my_string.h"
extern String * stringKey;

#endif /* __GLOBAL_H__ */
