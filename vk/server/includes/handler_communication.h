#ifndef __HANDLER_COMMUNICATION_H__
#define __HANDLER_COMMUNICATION_H__

Flag getAnswer();
Flag getPrivateFifoToRead();
Flag getPrivateFifoToWrite();
Flag sendToRecipient(String * recipient, String * data);
Flag createAccount(String * login, String * password);
Flag checkAccount(String * login, String * password);

void sendToPipe(enum MessageType type, String * login, String * password);

#endif /* __HANDLER_COMMUNICATION_H__ */
