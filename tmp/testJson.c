#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJson.h"
#include "ulaneLog.h"
#include "parseJson.h"
#include "createJson.h"
          
int main()
{
	int i;
  cJSON *json = cJSON_CreateObject();
	cJSON_AddStringToObject(json,"cmdName", "cmd_sendmessage");
	cJSON_AddStringToObject(json,"agentid","1001");
	cJSON_AddStringToObject(json,"agentname","guosj");
	cJSON_AddStringToObject(json,"raisehandid","100120");
	cJSON_AddStringToObject(json,"toagentid","201012");
	cJSON_AddStringToObject(json,"toagent","wwdsdsdsdsdsa");
	cJSON_AddStringToObject(json,"station","100101");
	cJSON_AddStringToObject(json,"sendtime","2017-1-1");
	cJSON_AddStringToObject(json,"messagetype","guodjjss");
	cJSON_AddStringToObject(json,"message","1222233");
	cJSON_AddStringToObject(json,"organizationid","1221");
	cJSON_AddStringToObject(json,"organizationname","jiangge");
  char *buf = cJSON_Print(json);
  strcat(buf, "eof");
	char *str;
	ULANE_SendMsg uSendMsg[1] = {{0}};
  str = CreateSendMessage(buf);
  printf("%s\n", str);
	return 0;	
}


/*
int test()
{
	int ret  = 0;
	cJSON *json = cJSON_CreateObject();
	cJSON_AddStringToObject(json,"cmdName", "cmd_raisehand");
	cJSON_AddStringToObject(json,"agentid","1001");
	cJSON_AddStringToObject(json,"raisehandid","guosj");
	cJSON_AddStringToObject(json,"eventtype","100120");
	cJSON_AddStringToObject(json,"eventitem","201012");
	cJSON_AddStringToObject(json,"station","100101");
	cJSON_AddStringToObject(json,"memo","2017-1-1");
	cJSON_AddStringToObject(json,"raisehandtime","guodjjss");
	cJSON_AddStringToObject(json,"requestagent","1222233");
	cJSON_AddStringToObject(json,"answertype","1221");
	cJSON_AddStringToObject(json,"organizationid","jiangge");
	cJSON_AddStringToObject(json,"organizationname","jiangge");
  char *buf = cJSON_Print(json);
  strcat(buf, "eof");
	//printf("%s\n", buf);
	char *out;
	out = CreateRaiseHandMsg(buf);
	if(ret == -1)
	{
		printf("CreateRaiseHandMsg Error\n");
		return -1;	
	}	
	printf("%s\n", out);
}
*/



