#include"libs.h"
#include"config.h"
#include"my_string.h"
#include"console.h"
#include"print.h"

int sockfd = 0;

int main(int argc, char **argv)
{
	char ip[] = "127.0.0.1";

	if(argc == 2)
		strcpy(ip, argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	CHECK("socket", sockfd);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(ip, &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	int result = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	CHECK("connect", result);

	String * string = createString();
	assert(string);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INIT);
	do
	{
		scanStringFromStream(STDIN, string, -1);

		isAll = consoleFiniteStateMachine(parseCommand(string));
	}
	while(isAll == FALSE);

	deleteString(string);
	close(sockfd);

	return 0;
}
