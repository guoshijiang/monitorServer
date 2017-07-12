#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "createJson.h"
#include "cJSON.h"

int main()
{
	char *eventName = "agentlogon"; 
	char *succErrBuf = "OK"; 
	char *errMsg = ""; 
	char *outJson = NULL;
	outJson = CreateAnswerJson(eventName, succErrBuf, errMsg);
	if(outJson == NULL)
	{
		printf("err msg\n");	
		return -1;
	}
	printf("outJson=%s\n", outJson);
	return 0;		
}
