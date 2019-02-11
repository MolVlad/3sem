#include"libs.h"
#include"general_config.h"
#include"global_variables.h"

#include"my_string.h"
#include"sem_config.h"
#include"server_feedback.h"

Flag receiveAnswer()
{
	Flag ret;

	HeaderReverseMessageStruct header;
	int result = scanHeaderReverse(sockfd, &header);
	CHECK("scanHeaderReverse", result);

	String * data = createString();

	switch(header.type)
	{
		case ACK:
			ret = TRUE;

			//it should be equal to zero in the init message (there are no data)
			if(header.dataSize != 0)
			{
				printf("ACK error, there are data\n");
				break;
			}

			break;
		case NACK:
			ret = FALSE;

			//it should be equal to zero in the init message (there are no data)
			if(header.dataSize != 0)
				printf("NACK error, there are data\n");

			break;
		case LIST:
			result = scanStringFromStream(sockfd, data, header.dataSize);
			CHECK("scanStringFromStream", result);
			printf("List of users online:\n");
			printStringToStream(STDOUT, data);

			break;
		case MESSAGE:
			result = scanStringFromStream(sockfd, data, header.dataSize);
			CHECK("scanStringFromStream", result);

			if(header.dataSize <= 2)
				ret = FALSE;
			else
			{
				ret = TRUE;
				printf("Message:\n");
				printStringToStream(STDOUT, data);
			}

			break;
	}

	deleteString(data);

	return ret;
}

//scan header from user to know sizes of blocks
int scanHeaderReverse(int fd, HeaderReverseMessageStruct * header)
{
	int result = read(fd, header, sizeof(HeaderReverseMessageStruct));
	if(result != sizeof(HeaderReverseMessageStruct))
	{
		printf("Scan header reverse error. Result = %d\n", result);
		return -1;
	}

	return 0;
}
