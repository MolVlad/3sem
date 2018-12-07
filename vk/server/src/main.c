#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"

#define PORT 51000
#define BACKLOG 1000
#define BUF_SIZE 1000

HTableMap * htableMap = NULL;
BTreeMap * btreeMap = NULL;

int main()
{/*
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

	struct sockaddr_in cliaddr;
	unsigned int clilen = sizeof(cliaddr);
	int newsockfd;
	int n;
	char line[BUF_SIZE];

	while(1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
		CHECK("accept", newsockfd);

		while((n = read(newsockfd, line, BUF_SIZE - 1) > 0))
		{
			n = write(newsockfd, line, strlen(line) + 1);
			CHECK("write", n);
		}

		CHECK("read", n);

		close(newsockfd);
	}

	close(sockfd);
*/
	String * string = createString();
	assert(string);

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	btreeMap = createBTree();
	assert(btreeMap);

	printHTable(htableMap);
	saveHTable(htableMap, HTABLE_STORAGE);
	printBTree(btreeMap);

	deleteString(string);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

	return 0;
}
