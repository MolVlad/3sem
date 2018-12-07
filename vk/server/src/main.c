#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"

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

	struct sockaddr_in cliaddr;
	unsigned int clilen = sizeof(cliaddr);
	HeaderMessageStruct header;

	/////for valgrind test
	int k = 1;
	while(k--)
	{
		int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
		CHECK("accept", newsockfd);

		int pid = fork();
		CHECK("fork", pid);
		if(pid == 0)
		{
			printf("pid = %d, ppid = %d\n", getpid(), getppid());

			close(sockfd);

			result = read(newsockfd, &header, sizeof(HeaderMessageStruct));
			if(result != sizeof(HeaderMessageStruct))
			{
				printf("read error\n");
				exit(0);
			}

			printf("\nUser!!!\n");
			printf("type = %d, login size = %d\n", header.type, header.loginSize);
			printf("password size = %d, data size = %d\n", header.passwordSize, header.dataSize);

			String * login = createString();
			String* password = createString();
			String * data = createString();

			printf("login:\n");
			clearString(login);
			result = scanStringFromStream(newsockfd, login, header.loginSize);
			if(result != header.loginSize)
			{
				printf("scan error: result = %d\n", result);
				exit(0);
			}
			printStringToStream(STDOUT, login);
			printf("\n");

			printf("password:\n");
			clearString(password);
			result = scanStringFromStream(newsockfd, password, header.passwordSize);
			if(result != header.passwordSize)
			{
				printf("read error\n");
				exit(0);
			}
			printStringToStream(STDOUT, password);
			printf("\n");

			if(header.dataSize != 0)
			{
				printf("Init error\n");
				exit(0);
			}

			Flag isOK;
			if(header.type == REG)
			{
				printf("\nUser want to reg!!\n");

				HTableData * desired = findInHTable(htableMap, login);
				if(desired == NULL)
				{
					insertToHTable(htableMap, convertToHTableData(login, password));
					isOK = TRUE;
				}
				else
					isOK = FALSE;
			}
			else if(header.type == LOGIN)
			{
				printf("\nUser want to login!!\n");

				HTableData * desired = findInHTable(htableMap, login);

				if(desired == NULL)
					isOK = FALSE;
				else if(stringCompare(desired->password, password) == 0)
					isOK = TRUE;
				else
					isOK = FALSE;
			}
			else
				exit(-1);

			if(isOK == TRUE)
			{
				printf("That's OK\n");
				insertToBTree(btreeMap, convertToBTreeData(login, "ip", pid));

				Flag isAll = FALSE;
				while(isAll == FALSE)
				{
					result = read(newsockfd, &header, sizeof(HeaderMessageStruct));
					if(result != sizeof(HeaderMessageStruct))
					{
						printf("read error\n");
						exit(0);
					}

					printf("\nMessage!!!\n");
					printf("type = %d, login size = %d\n", header.type, header.loginSize);
					printf("password size = %d, data size = %d\n", header.passwordSize, header.dataSize);

					if(header.loginSize)
					{
						printf("login:\n");
						clearString(login);
						result = scanStringFromStream(newsockfd, login, header.loginSize);
						if(result != header.loginSize)
						{
							printf("read error\n");
							exit(0);
						}
						printStringToStream(STDOUT, login);
						printf("\n");
					}

					if(header.passwordSize)
					{
						printf("password:\n");
						clearString(password);
						result = scanStringFromStream(newsockfd, password, header.passwordSize);
						if(result != header.passwordSize)
						{
							printf("read error\n");
							exit(0);
						}
						printStringToStream(STDOUT, password);
						printf("\n");
					}

					if(header.dataSize)
					{
						printf("data:\n");
						clearString(data);
						result = scanTextFromStream(newsockfd, data, header.dataSize);
						if(result != header.dataSize)
						{
							printf("read error, result = %d\n", result);
							exit(0);
						}
						printStringToStream(STDOUT, data);
						printf("\n");
					}

					isAll = TRUE;
				}
			}
			else
			{
				printf("Oh, problems!\n");
			}

			deleteString(login);
			deleteString(password);
			deleteString(data);
			deleteHTable(htableMap);
			deleteBTree(btreeMap);

			exit(0);
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
			printf("waitpid\n");
		}

		close(newsockfd);
	}

	saveHTable(htableMap, HTABLE_STORAGE);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);
	close(sockfd);

	return 0;
}
