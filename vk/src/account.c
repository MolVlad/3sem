#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"account.h"
#include"htable.h"

extern HTableMap * htableMap;

Flag checkAccount()
{
	printf("Let's log in\n");

	htableMap = createHTable();
	deleteHTable(htableMap);

	String * login = createString();
	String * password = createString();

	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);

/*	HTableData * trueData = findInHTable(htableMap, login);
	if(trueData == NULL)
		return FALSE;

	if(isCorrect(HTableData trueData, login, password) == TRUE)
		return TRUE;
*/
	return FALSE;
}

Flag createAccount()
{
	printf("Let's create account\n");

	String * login = createString();
	String * password = createString();

	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);


	return TRUE;
}


