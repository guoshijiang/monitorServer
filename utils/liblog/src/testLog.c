#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ulaneLog.h"

int main()
{
	int ret = 10;
	char fileStr[100];
	//fileStr = getenv("HOME");
	//sprintf(fileStr, "%s/%s", getenv("HOME"), "log/guosj.txt");
	//printf("fstr= %s\n", fileStr);
	

	
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[3], ret,"asdfffffffffff");
	printf("write log ok\n");
	return 0;
}