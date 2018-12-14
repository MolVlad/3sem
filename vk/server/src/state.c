#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"state.h"
#include"handler_communication.h"
#include"user_communication.h"
#include"global.h"

Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd)
{
	static enum ServerState state = INIT;
	static Flag isAll = FALSE;
	Flag isOK;
	int result;

	//strings for blocks
	String * login = createString();
	String * password = createString();
	String * data = createString();

	result = scanStringFromStream(newsockfd, login, header->loginSize);
	CHECK("stringGetAndCheck login", result);
	if(header->loginSize > 0)
	{
		result = printStringToStream(STDOUT, login);
		CHECK("printStringToStream", result);
		printf("\n");
	}

	result = scanStringFromStream(newsockfd, password, header->passwordSize);
	CHECK("stringGetAndCheck password", result);
	if(header->passwordSize > 0)
	{
		result = printStringToStream(STDOUT, password);
		CHECK("printStringToStream", result);
		printf("\n");
	}

	result = scanStringFromStream(newsockfd, data, header->dataSize);
	CHECK("stringGetAndCheck data", result);
	if(header->dataSize > 0)
	{
		result = printStringToStream(STDOUT, data);
		CHECK("printStringToStream", result);
		printf("\n");
	}

	switch(state)
	{
		case INIT:
			//it should be equal to zero in the init message (there are no data)
			if(header->dataSize != 0)
			{
				printf("Init error\n");
				break;
			}

			switch(header->type)
			{
				case REG:	//case with registration
					printf("\nUser want to register\n");

					isOK = createAccount(login, password);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
						userLogin = createString();
						userLogin = copyString(login);
						printf("Reg success\n");
					}
					else
					{
						printf("Reg error: already exists\n");
						sendReply(FALSE, newsockfd);
					}
					break;
				case LOGIN:	//case with login
					printf("\nUser want to login\n");

					isOK = checkAccount(login, password);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
						userLogin = createString();
						userLogin = copyString(login);
						printf("Login success\n");
					}
					else
					{
						printf("Wrong login of password\n");
						sendReply(FALSE, newsockfd);
					}
					break;
				case END:
					printf("End of communication\n");
					isAll = TRUE;
					return isAll;
				default:
					printf("Wrond msg type for init state\n");
					CHECK("", -1);
					break;
			}

			break;
		case WAITING_REQUESTS:
			switch(header->type)
			{
				case MSG:
					isOK = sendToRecipient(login, data);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
						printf("Sent success\n");
						printf("Let's read!\n");
					}
					else
					{
						printf("Send error\n");
						sendReply(FALSE, newsockfd);
					}

					break;
				case RCV:
					sendMessage(newsockfd);

					break;
				case LIST_REQUEST:
					sendList(newsockfd);

					break;
				case END:
					printf("End of communication\n");
					sendToPipe(END, userLogin, NULL);
					isAll = TRUE;
					return isAll;
				default:
					break;
			}

			break;
		default:
			printf("server state error\n");
			CHECK("", -1);
	}

	deleteString(login);
	deleteString(password);
	deleteString(data);

	return isAll;
}
