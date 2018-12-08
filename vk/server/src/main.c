#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"

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

		int pid = fork();
		CHECK("fork", pid);
		if(pid == 0)
		{
			printf("pid = %d, ppid = %d\n", getpid(), getppid());

			close(sockfd);

			HeaderMessageStruct header;
			result = scanHeader(newsockfd, &header);
			CHECK("scanHeader", result);

			String * login = createString();
			String* password = createString();
			String * data = createString();

			printf("login:\n");
			result = stringGetAndPrint(newsockfd, login, header.loginSize);
			CHECK("stringGetAndCheck login", result);

			printf("password:\n");
			result = stringGetAndPrint(newsockfd, password, header.passwordSize);
			CHECK("stringGetAndCheck password", result);

			//it should be equal to zero at first time (there are no data)
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
					/////??????????????????? не работает
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
