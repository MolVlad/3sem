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

	HTableData * desired = findInHTable(htableMap, login);
	if(desired == NULL)
	{
		insertToHTable(htableMap, convertToHTableData(login, password));
		return TRUE;
	}

	return FALSE;
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

	HTableData * desired = findInHTable(htableMap, login);

	if(desired == NULL)
		return FALSE;

	if(areNamesSame(desired->password, password) == TRUE)
		return TRUE;

	return FALSE;
}
