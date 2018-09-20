#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

void GenerateString(int n, char * str);

int main()
{
	int n;

	scanf("%d", &n);

	char * str = malloc((1 << n + 1) * sizeof(char));
	assert(str);

	GenerateString(n, str);

	printf("%s\n", str);

	return 0;
}

void GenerateString(int n, char * str)
{
	int i;

	char * rec = malloc((1 << n + 1) * sizeof(char));
	char * c = malloc(2 * sizeof(char));
	c[1] = '\0';
	rec[0] = '\0';

	for(i = 0; i < n; i++)
	{
		strcpy(str, rec);
		c[0] = 'a' + i;
		strcat(str, c);
		strcat(str, rec);
		strcpy(rec, str);
	}

	free(rec);
	free(c);
}
