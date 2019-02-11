#include"libs.h"
#include"general_config.h"

#include"server_api.h"
#include"server_connection.h"
#include"server_feedback.h"

Flag checkAccount()
{
	sendViaNet(LOGIN);
	return receiveAnswer();
}

Flag createAccount()
{
	sendViaNet(REG);
	return receiveAnswer();
}

void userList()
{
	sendViaNet(LIST_REQUEST);
	receiveAnswer();
}

Flag sendMessage()
{
	sendViaNet(MSG);
	return receiveAnswer();
}

Flag receiveMessage()
{
	sendViaNet(RCV);
	return receiveAnswer();
}
