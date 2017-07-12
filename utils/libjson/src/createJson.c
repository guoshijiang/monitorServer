/*=============================================================
 *文件名称: createJson.c
 *创建时间: 2017.6.26
 *作    者: 郭世江
 *修改记录:
 *   2017.6.26 创建文件
 *功能描述：
 *  创建JSON串
 * ===========================================================*/
#include "createJson.h"

//创建应答登录的JSON串
char* CreateAnswerJson(char *eventName, char *succErrBuf, char *errMsg)
{
	char *outJson = NULL;
	if(eventName == NULL || succErrBuf == NULL || errMsg == NULL)
	{
		printf("eventName, succBuf and errMsg is NULL\n");	
		return -1;
	}
	cJSON *AnsJson = cJSON_CreateObject();
	if(AnsJson == NULL)
	{
		printf("json is NULL");
		return -1;	
	}
	cJSON_AddStringToObject(AnsJson,"cmdName", eventName);
	cJSON_AddStringToObject(AnsJson,"result", succErrBuf);
	cJSON_AddStringToObject(AnsJson,"errormsg", errMsg);
  outJson = cJSON_Print(AnsJson);
  strcat(outJson, "eof");
  cJSON_Delete(AnsJson); 
  return outJson;
}