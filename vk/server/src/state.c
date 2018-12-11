#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"state.h"
#include"handler_communication.h"
#include"user_communication.h"

Flag serverFiniteStateMachine(HeaderMessageStruct * header, int newsockfd, int fifo)
{
	static enum ServerState state = INIT;
	static Flag isAll = FALSE;
	Flag isOK;
	int result;

	if(header->type == END)
	{
		printf("End of communication\n");
		isAll = TRUE;
		return isAll;
	}

	//strings for blocks
	String * login = createString();
	String * password = createString();
	String * data = createString();

	printf("login:\n");
	result = stringGetAndPrint(newsockfd, login, header->loginSize);
	CHECK("stringGetAndCheck login", result);

	printf("password:\n");
	result = stringGetAndPrint(newsockfd, password, header->passwordSize);
	CHECK("stringGetAndCheck password", result);

	printf("data:\n");
	result = stringGetAndPrint(newsockfd, data, header->dataSize);
	CHECK("stringGetAndCheck password", result);

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

					isOK = createAccount(login, password, fifo);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
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

					isOK = checkAccount(login, password, fifo);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
						printf("Login success\n");
					}
					else
					{
						printf("Wrong login of password\n");
						sendReply(FALSE, newsockfd);
					}
					break;
				default:
					printf("Wrond msg type for init state\n");
					break;
			}

			break;
		case WAITING_REQUESTS:
			switch(header->type)
			{
				case MSG:
					isOK = sendToHandler(login, data, fifo);
					if(isOK == TRUE)
					{
						state = WAITING_REQUESTS;
						sendReply(TRUE, newsockfd);
						printf("Sent success\n");
					}
					else
					{
						printf("Send error\n");
						sendReply(FALSE, newsockfd);
					}

					break;
				case LIST_REQUEST:
					sendList(newsockfd);

					break;
				default:
					break;
			}

			break;
		default:
			printf("server state error\n");
			exit(1);
	}

	deleteString(login);
	deleteString(password);
	deleteString(data);

	return isAll;
}
