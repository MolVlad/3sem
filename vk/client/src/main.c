#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"console.h"
#include"print.h"

//#define PORT 51000
//#define BUF_SIZE 1000

int main(int argc, char **argv)
{
/*	if(argc != 2)
	{
		printf("Usage: a.out <IP address>");
		exit(-1);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	int result = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("connect", result);

	int i, n;
	char sendline[BUF_SIZE], recvline[BUF_SIZE];
	for(i = 0; i < 3; i++)
	{
		printf("strint = ");
		fflush(stdin);
		fgets(sendline, 1000, stdin);

		n = write(sockfd, sendline, strlen(sendline) + 1);
		CHECK("write", n);

		n = read(sockfd, recvline, BUF_SIZE - 1);
		CHECK("read", n);

		printf("%s\n", recvline);
	}

	close(sockfd);
*/
	String * string = createString();
	assert(string);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INIT);
	do
	{
		scanStringFromStream(STDIN, string);

		#ifdef DEBUG_CONSOLE
		printf("In the main loop:\n");
		printString(string);
		#endif * DEBUG_CONSOLE *

		isAll = consoleFiniteStateMachine(parseCommand(string));
	}
	while(isAll == FALSE);

	deleteString(string);

	return 0;
}
