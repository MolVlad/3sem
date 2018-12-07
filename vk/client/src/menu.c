#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"menu.h"

Flag createAccount()
{
	printf("Let's create account\n");

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);

	printf("Login:\n");
	scanStringFromStream(STDIN, login);
	printf("Password:\n");
	scanStringFromStream(STDIN, password);

	Flag isOK = TRUE;

	/*
	HTableData * desired = findInHTable(htableMap, login);
	if(desired == NULL)
	{
		insertToHTable(htableMap, convertToHTableData(login, password));
		isOK = TRUE;
	}
	else
		isOK = FALSE;

	if(isOK == TRUE)
	{
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
		}
		if(pid == 0)
		{
			printf("fork!!\n");
			exit(0);
		}
		else
			insertToBTree(btreeMap, convertToBTreeData(login, "ip", pid));
	}
	*/

	deleteString(login);
	deleteString(password);

	return isOK;
}

Flag checkAccount()
{
	printf("Let's log in\n");

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);

	printf("Login:\n");
	scanStringFromStream(STDIN, login);
	printf("Password:\n");
	scanStringFromStream(STDIN, password);

	Flag isOK = TRUE;

	/*
	HTableData * desired = findInHTable(htableMap, login);

	if(desired == NULL)
		isOK = FALSE;
	else if(areStringSame(desired->password, password) == TRUE)
		isOK = TRUE;
	else
		isOK = FALSE;

	if(isOK == TRUE)
	{
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
		}
		if(pid == 0)
		{
			printf("fork!!\n");
			exit(0);
		}
		else
			insertToBTree(btreeMap, convertToBTreeData(login, "ip", pid));
	}
	*/
	deleteString(login);
	deleteString(password);

	return isOK;
}

void sendMessage()
{
	#ifdef DEBUG_MENU
	printf("sendMessage\n");
	#endif /* DEBUG_MENU */
}
void readDialogue()
{
	#ifdef DEBUG_MENU
	printf("readDialogue\n");
	#endif /* DEBUG_MENU */
}
void userList()
{
	#ifdef DEBUG_MENU
	printf("userList\n");
	#endif /* DEBUG_MENU */
}

void deleteMessage()
{
	#ifdef DEBUG_MENU
	printf("deleteMessage");
	#endif /* DEBUG_MENU */
}
void clearHistory()
{
	#ifdef DEBUG_MENU
	printf("clearHistory");
	#endif /* DEBUG_MENU */
}
