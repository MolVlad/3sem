#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"account.h"
#include"htable.h"

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

	deleteString(login);
	deleteString(password);

	return isOK;
}
