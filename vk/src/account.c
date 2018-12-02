#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"account.h"
#include"htable.h"
#include"btree.h"

Flag createAccount()
{
	printf("Let's create account\n");

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);

	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);


	Flag isOK;

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

	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);

	Flag isOK;

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

	deleteString(login);
	deleteString(password);

	return isOK;
}
