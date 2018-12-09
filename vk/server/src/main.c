#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"communication.h"

HTableMap * htableMap;
BTreeMap * btreeMap;

int main()
{
	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	btreeMap = createBTree();
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
	while(1)
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
			Flag isAll = FALSE;
			while(isAll == FALSE)
			{
				result = scanHeader(&header, newsockfd);
				if(result == -1)
				{
					printf("Can't scan header, close connect\n");
					isAll = TRUE;
				}
				else
					isAll = serverFiniteStateMachine(&header, newsockfd);
			}

			deleteHTable(htableMap);
			deleteBTree(btreeMap);

			//end of process work
			exit(0);
		}
		else
		{
			//переписать, чтобы родитель сохранял информацию о процессе
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
