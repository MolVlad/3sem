#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"account.h"
#include"htable.h"

Flag checkAccount()
{
	printf("Let's log in\n");

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);

	int i;
	for(i = 0; i < 10; i++)
	{

	}
	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);

	insertToHTable(htableMap, convertToHTableData(login, password));

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
	assert(login);
	String * password = createString();
	assert(login);

	printf("Login = ");
	scanString(login);
	printf("Password = ");
	scanString(password);

	return TRUE;
}
