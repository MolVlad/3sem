#ifndef __HANDLER_COMMUNICATION_H__
#define __HANDLER_COMMUNICATION_H__

Flag sendToHandler(String * recipient, String * data, int fifo);
Flag createAccount(String * login, String * password, int fifo);
Flag checkAccount(String * login, String * password, int fifo);


#endif /* __HANDLER_COMMUNICATION_H__ */
