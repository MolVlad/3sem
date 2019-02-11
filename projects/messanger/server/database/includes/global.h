#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include"htable.h"
#include"btree.h"

extern HTableMap * htableMap;
extern BTreeMap * btreeMap;
extern int semid;
extern int msgidForAnswer;
extern int msgidForTransmission;
extern int generalFifo;
extern char privateFifoName[PRIVATE_FIFO_NAME_SIZE];
extern int privateFifo;
extern String * stringForGeneralFifo;

#endif /* __GLOBAL_H__ */
