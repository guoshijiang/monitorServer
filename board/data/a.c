#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *arr1[7] = {"aaa", "bbb", "cccc", "hhhh", "mmmm", "nnnn", "qqq"};
	char *arr2[3] = {"bbb", "hhhh", "bbbbb"};
	int i;
	int j;
	for(i = 0; i < 7; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(strcmp(arr1[i], arr2[j]) == 0)
			{
				printf("%d\n", i);
			}
		}
	}
	return 0;
}
