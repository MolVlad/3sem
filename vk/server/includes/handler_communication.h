#ifndef __HANDLER_COMMUNICATION_H__
#define __HANDLER_COMMUNICATION_H__

Flag sendToHandler(String * recipient, String * data);
Flag createAccount(String * login, String * password);
Flag checkAccount(String * login, String * password);

String * pidToString();
void sendToPipe(enum MessageType type, String * login, String * password, String * data);

#endif /* __HANDLER_COMMUNICATION_H__ */
