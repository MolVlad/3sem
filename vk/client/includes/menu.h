#ifndef __MENU_H__
#define __MENU_H__

void sendViaNet(enum MessageType type);

Flag checkAccount();
Flag createAccount();
void sendMessage();
void readDialogue();
void userList();
void deleteMessage();
void clearHistory();

#endif /* __MENU_H__ */
