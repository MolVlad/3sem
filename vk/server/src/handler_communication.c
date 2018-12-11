#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"handler_communication.h"

////////////здесь происходит синхронизация семафорами
////////////печататаем в fifo в нужном формате и затем ждем в очереди сообщений ответа
////////////ждем ответа в очереди сообщений
Flag sendToHandler(String * recipient, String * data, int fifo)
{
	Flag isOK = TRUE;

	return isOK;
}

Flag createAccount(String * login, String * password, int fifo)
{
	Flag isOK = TRUE;

	return isOK;
}

Flag checkAccount(String * login, String * password, int fifo)
{
	Flag isOK = TRUE;

	return isOK;
}
