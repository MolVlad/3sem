#ifndef __SENDING_INSTRUCTIONS_BY_HANDLER_H__
#define __SENDING_INSTRUCTIONS_BY_HANDLER_H__

#include"general_config.h"

void replyWithMSG(int pid, Flag isOK);
void sendToRecipientPrivateFifoName(int pid);
void sendToSenderPrivateFifoName(int pid, Flag isOK);
int scanPid();
void increasePrivateFifoName(char * privateFifoName);

#endif /* __SENDING_INSTRUCTIONS_BY_HANDLER_H__ */
