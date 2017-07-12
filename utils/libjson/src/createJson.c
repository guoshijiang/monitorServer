/*=============================================================
 *�ļ�����: createJson.c
 *����ʱ��: 2017.6.26
 *��    ��: ������
 *�޸ļ�¼:
 *   2017.6.26 �����ļ�
 *����������
 *  ����JSON��
 * ===========================================================*/
#include "createJson.h"

//����Ӧ���¼��JSON��
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