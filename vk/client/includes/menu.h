#ifndef __MENU_H__
#define __MENU_H__

void setConnect();
void sendViaNet(enum MessageType type);

Flag checkAccount();
Flag createAccount();
void logOut(int socketFd);
Flag sendMessage();
void userList();

#endif /* __MENU_H__ */
