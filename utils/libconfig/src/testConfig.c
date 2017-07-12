#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Config.h"

int main()
{
	char *str;
	char *str1;
	str = ReadConItem("ip");
	printf("ip=%s\n", str);
	str1 = ReadConItem("port");
	printf("port=%s\n", str1);
	return 0;
}