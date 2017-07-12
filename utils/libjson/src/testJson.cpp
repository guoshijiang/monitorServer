#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parseJson.h"
#include "cJSON.h"

int main()
{
	int i;
  cJSON *json = cJSON_CreateObject();
	cJSON_AddStringToObject(json,"cmdName","agentlogon");
	cJSON_AddStringToObject(json,"msgID","100000001");
	cJSON_AddStringToObject(json,"agentid","agentmonitor1");
	cJSON_AddStringToObject(json,"agenttype","agenttype_3");
  char *buf = cJSON_Print(json);
  strcat(buf, "eof");
	printf("%s\n", buf);
	
	ULANE_AgentLogon iLogon[1] = {{0}};
	LogoJsonDecode(buf, iLogon);
	printf("cmdName=%s,msgID=%s,agentid=%s,agenttype=%s\n",iLogon[0].iCmdName,iLogon[0].iMsgID,iLogon[0].iAgentid,iLogon[0].iAgenttype);  
	return 0;		
}
