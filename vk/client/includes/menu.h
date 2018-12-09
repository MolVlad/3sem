#ifndef __MENU_H__
#define __MENU_H__

void setConnect();
void sendViaNet(enum MessageType type);

Flag checkAccount();
Flag createAccount();
void logOut(int socketFd);
void sendMessage();
void readDialogue();
void userList();
void deleteMessage();
void clearHistory();

#endif /* __MENU_H__ */
