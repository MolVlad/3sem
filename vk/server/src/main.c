#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"

//scan header from user to know sizes of blocks
int scanHeader(int fd, HeaderMessageStruct * header)
{
	int result = read(fd, header, sizeof(HeaderMessageStruct));
	if(result != sizeof(HeaderMessageStruct))
		return -1;

	printf("\nMessage!\nHeader:\n");
	printf("type = %d, login size = %d\n", header->type, header->loginSize);
	printf("password size = %d, data size = %d\n", header->passwordSize, header->dataSize);

	return 0;
}

int main()
{
	HTableMap * htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	BTreeMap * btreeMap = createBTree();
	assert(btreeMap);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("bind", result);

	result = listen(sockfd, BACKLOG);
	CHECK("listen", result);
	printf("listen!\n");

	///////////////// сюда добавить выход по сигналу
	int k = 3;
	while(k--)
	{
		struct sockaddr_in cliaddr;
		unsigned int clilen = sizeof(cliaddr);
		int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
		CHECK("accept", newsockfd);

		//communicate with user in other process
		int pid = fork();
		CHECK("fork", pid);
		if(pid == 0)
		{
			//close first socket in child process
			close(sockfd);

			HeaderMessageStruct header;
			result = scanHeader(newsockfd, &header);
			CHECK("scanHeader", result);

			//strings for blocks
			String * login = createString();
			String* password = createString();
			String * data = createString();

			printf("login:\n");
			result = stringGetAndPrint(newsockfd, login, header.loginSize);
			CHECK("stringGetAndCheck login", result);

			printf("password:\n");
			result = stringGetAndPrint(newsockfd, password, header.passwordSize);
			CHECK("stringGetAndCheck password", result);

			//it should be equal to zero in the init message (there are no data)
			if(header.dataSize != 0)
			{
				printf("Init error\n");
				exit(0);
			}

			//check the correctness of the data
			Flag isOK;
			if(header.type == REG)	//case with registration
			{
				printf("\nUser want to register!!\n");

				HTableData * desired = findInHTable(htableMap, login);
				if(desired == NULL)	//if not founded means can create with this login
				{
					/////??????????????????? не работает
					insertToHTable(htableMap, convertToHTableData(login, password));
					isOK = TRUE;
				}
				else
					isOK = FALSE;
			}
			else if(header.type == LOGIN)	//case with login
			{
				printf("\nUser want to login!!\n");

				HTableData * desired = findInHTable(htableMap, login);

				if(desired == NULL)	//not founded
					isOK = FALSE;
				else if(stringCompare(desired->password, password) == 0)
					isOK = TRUE;	//founded and password is correct
				else
					isOK = FALSE;	//founded but passwords are different
			}
			else
				exit(-1);	//another type of init message is error

			if(isOK == TRUE)	//if data is correct
			{
				printf("That's OK\n");
				//insert to list (tree) of online users
				insertToBTree(btreeMap, convertToBTreeData(login, "ip", pid));

				//start constant communication
				Flag isAll = FALSE;
				while(isAll == FALSE)
				{
					result = scanHeader(newsockfd, &header);
					CHECK("scanHeader", result);
					if(header.loginSize)
					{
						printf("login:\n");
						result = stringGetAndPrint(newsockfd, login, header.loginSize);
						CHECK("stringGetAndCheck login", result);
					}

					if(header.passwordSize)
					{
						printf("password:\n");
						result = stringGetAndPrint(newsockfd, password, header.passwordSize);
						CHECK("stringGetAndCheck password", result);
					}

					if(header.dataSize)
					{
						printf("data:\n");
						result = stringGetAndPrint(newsockfd, data, header.dataSize);
						CHECK("stringGetAndCheck data", result);
					}

					isAll = TRUE;
				}
			}
			else
			{
				printf("Oh, problems!\n");
			}

			//delete all
			deleteString(login);
			deleteString(password);
			deleteString(data);
			deleteHTable(htableMap);
			deleteBTree(btreeMap);

			//end of work
			exit(0);
		}
		else
		{
			//parent waits for child (for debug)
			int status;
			waitpid(pid, &status, 0);
			printf("waitpid\n");
		}

		//close socket for this user n both process
		close(newsockfd);
	}

	//save list of logins/passwords and free memory
	saveHTable(htableMap, HTABLE_STORAGE);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);
	close(sockfd);

	return 0;
}
