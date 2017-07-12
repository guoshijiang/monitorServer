/*=============================================================
 *文件名称: parseJson.c
 *创建时间: 2017.6.19
 *作    者: 郭世江
 *修改记录:
 *2017.6.19 创建文件
 *功能描述：
 *  封装解析JSON文件
 * ===========================================================*/
#include "parseJson.h"
#include "ulaneJsonLog.h"
#include "jsonCheck.h"

//获取以cmdName这样的json串的命令类型
char *CmdNameParse(char *lpJStr)
{
	 cJSON* pJson;
	 cJSON* pJsonObj;
	 int iRet = 0;
	 if(lpJStr == NULL)
   {
   	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
     return NULL;
   } 
 	 pJson = cJSON_Parse(lpJStr);
   if( pJson == NULL)
   {
	 	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"pJson is NULL");
	   //cJSON_Delete(pJson);
	   return NULL;
   }
   if (pJson->type == cJSON_Object )
   {
     pJsonObj = cJSON_GetObjectItem(pJson, "cmdName");
     if (pJsonObj == NULL )
     {
     	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"pJsonObj is NULL");
   		 cJSON_Delete(pJson);
 			 cJSON_Delete(pJsonObj); 
       return NULL;
     } 
     Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"%s\n", pJsonObj->valuestring);
     return pJsonObj->valuestring;
   }
   cJSON_Delete(pJson);
   //cJSON_Delete(pJsonObj); 
}

//解析举手请求的JSON串
int RaiseHandReqJsonParse(char* lpJStr, ULANE_RaiseHandReqMsg rhReqMsg[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"parse json err");
	  return -1;
	}
	
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "cmdName");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }    
  memcpy(rhReqMsg[0].iCmdName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cmdName:%s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "agentid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object agentid err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iAgentId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"msgID:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "raisehandid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object raisehandid err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iRaiseHandId,pJsonSub->valuestring,strlen(pJsonSub->valuestring));
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"raisehandid:%s\n", pJsonSub->valuestring); 

  pJsonSub = cJSON_GetObjectItem(iJsonObj, "eventtype");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get eventtype object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iEventType,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"eventtype:%s\n", pJsonSub->valuestring);
   
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "eventitem");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get eventitem object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iEventItem,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"eventitem:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "station");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get station object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iStation,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"station:%s\n", pJsonSub->valuestring);
   
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "memo");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get memo object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iMemo,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"memo:%s\n", pJsonSub->valuestring);
   
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "raisehandtime");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get raisehandtime object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iRaiseHandTime,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"raisehandtime:%s\n", pJsonSub->valuestring);
	
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "requestagent");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get requestagent object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iRequestAgent,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"requestagent:%s\n", pJsonSub->valuestring);
  
	 pJsonSub = cJSON_GetObjectItem(iJsonObj, "answertype");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get answertype object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iAnswerType,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"answertype:%s\n", pJsonSub->valuestring);
   
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "organizationid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get organizationid object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iOrganizationId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"organizationid:%s\n", pJsonSub->valuestring);
	  
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "organizationname");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get objector ganizationname err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(rhReqMsg[0].iOrganizationName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"organizationname:%s\n", pJsonSub->valuestring);
  cJSON_Delete(iJsonObj);
	return 0;
} 

//消息发送接口
int SendMsgJsonParse(char* lpJStr, ULANE_SendMsg uSendMsg[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"parse json err");
	  return -1;
	}
	
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "cmdName");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }    
  memcpy(uSendMsg[0].iCmdName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cmdName:%s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "agentid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object agentid err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iAgentId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"msgID:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "agentname");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object raisehandid err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iAgentName,pJsonSub->valuestring,strlen(pJsonSub->valuestring));
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"raisehandid:%s\n", pJsonSub->valuestring); 

  pJsonSub = cJSON_GetObjectItem(iJsonObj, "raisehandid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get eventtype object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iRaiseHandId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"eventtype:%s\n", pJsonSub->valuestring);
    
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "toagentid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get eventitem object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iToAgentId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"eventitem:%s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "toagent");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get eventitem object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iToAgent,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"eventitem:%s\n", pJsonSub->valuestring);
  
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "station");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get station object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iStation,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"station:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "sendtime");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get memo object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iSendTime,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"memo:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "messagetype");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get raisehandtime object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iMessageType,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"raisehandtime:%s\n", pJsonSub->valuestring);

	pJsonSub = cJSON_GetObjectItem(iJsonObj, "message");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get requestagent object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iMessage,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"requestagent:%s\n", pJsonSub->valuestring);
	  	                
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "organizationid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get answertype object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iOrganizationId,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"answertype:%s\n", pJsonSub->valuestring);
  
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "organizationname");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get organizationid object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(uSendMsg[0].iOrganizationName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"organizationid:%s\n", pJsonSub->valuestring);
	return 0;
}


//获取解析JSON串的类型
char *EveNameParse(char *lpJStr)
{
	 cJSON* pJson;
	 cJSON* pJsonObj;
	 if(lpJStr == NULL)
   {
   	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
     return NULL;
   }   
   pJson = cJSON_Parse(lpJStr);
   if( pJson == NULL)
   {
	 	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"pJson is NULL");
	   return NULL;
   }
   if (pJson->type == cJSON_Object )
   {
     pJsonObj = cJSON_GetObjectItem(pJson, "event_name");
     if (pJsonObj == NULL )
     {
     	 Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
   		 cJSON_Delete(pJson);
 			 cJSON_Delete(pJsonObj); 
       return NULL;
     } 
     return pJsonObj->valuestring;
   }
   cJSON_Delete(pJson);
   cJSON_Delete(pJsonObj); 
}

//获取json数组长度
int GetJsonArrayInnerLen(char *lpJStr)
{
	cJSON*       iJsonObj;
	cJSON*       iJsonArray;
	int          iArraySize;  
	iJsonObj=cJSON_Parse(lpJStr);  
	if (iJsonObj == NULL)  
	{  
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
	  return -1;
	}
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"lpJStr is NULL");
    	cJSON_Delete(iJsonObj);
    	cJSON_Delete(iJsonArray);
    	return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetArraySize: iArraySize=%d\n",iArraySize);
  }
  return iArraySize;
}

//解析logon的json
int LogoJsonDecode(char* lpJStr, ULANE_AgentLogon ageLogon[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"parse json err");
	  return -1;
	}
	
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "cmdName");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  memcpy(ageLogon[0].iCmdName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cmdName:%s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "msgID");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iMsgID,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"msgID:%s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "agentid");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iAgentid,pJsonSub->valuestring,strlen(pJsonSub->valuestring));
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"agentid:%s\n", pJsonSub->valuestring); 
 
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "agenttype");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iAgenttype,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"agenttype:%s\n", pJsonSub->valuestring);
  cJSON_Delete(iJsonObj);
	return 0;
}

//解析分机状态明细的JSON
int ExtJsonDecode(char* lpJStr, ULANE_ExtStateLog extLog[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	cJSON*       iJsonArray;
	cJSON*       iArrayItem;
	cJSON*       iItemObject;
	int          iArraySize;
	int          i;   
	
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"parse json err"); 
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"pJsonSub_1 : %s\n", pJsonSub->valuestring); 
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"pJsonSub_2 : %s\n", pJsonSub->valuestring); 
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get three object err3");
  	cJSON_Delete(iJsonObj);
  	cJSON_Delete(pJsonSub);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get json array length err");
    	cJSON_Delete(iJsonObj);
  	  cJSON_Delete(pJsonSub);	
  	  cJSON_Delete(iJsonArray);	
  	  return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetArraySize: iArraySize=%d\n",iArraySize);
    for(i=0;i<iArraySize;i++)  
    {   
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
       
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"State");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iState,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AUX_Code");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iAUX_Code,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Begintime");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);
 				memcpy(extLog[i].iBegintime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Endtime");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iEndtime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }    
      iArrayItem=cJSON_GetObjectItem(iItemObject,"StateInfo");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iStateInfo,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CustPhone");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iCustPhone,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallDirect");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);
 				memcpy(extLog[i].iCallDirect,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(extLog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
    }  
    cJSON_Delete(iJsonObj);  
  } 
  return 0;
}

//解析呼入通话明细的JSON
int TelinJsonDecode(char* lpJStr, ULANE_VDNCallLog vcllog[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	cJSON*       iJsonArray;
	cJSON*       iArrayItem;
	cJSON*       iItemObject;
	int          iArraySize;
	int          i;   
	
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{  
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"parse json err");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"get json array length err");
    	cJSON_Delete(iJsonObj);
  	  return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetArraySize: iArraySize=%d\n",iArraySize); 
    for(i=0;i<iArraySize;i++)  
    {   
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"json array %d\n", iArraySize); 
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iVDN,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"UCID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iUCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
       
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iCallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      } 
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ANI");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iANI,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"DNIS");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iDNIS,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
        
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Enter_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnter_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Queue_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iQueue_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Deliver_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iDeliver_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Estab_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEstab_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnd_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
 			  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 

      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Reason");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnd_Reason,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iVDN_Dur = iArrayItem->valueint;
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Queue_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iQueue_Dur = iArrayItem->valueint; 
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Talk_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iTalk_Dur = iArrayItem->valueint; 
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Ring_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iRing_Dur = iArrayItem->valueint;
      }
     	
     	iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iHold_Dur = iArrayItem->valueint;
      }
     
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Count");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iHold_Count = iArrayItem->valueint;
      }
      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"TransferCount");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iTransferCount = iArrayItem->valueint; 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConferenceCount");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
        vcllog[i].iConferenceCount = iArrayItem->valueint; 
      }
    } 
  }
  cJSON_Delete(iJsonObj);  
  return 0;
}

//解析呼出通话明细的JSON
int TeloutExtJsonDecode(char* lpJStr, ULANE_StationCallLog sclLog[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	cJSON*       iJsonArray;
	cJSON*       iArrayItem;
	cJSON*       iItemObject;
	int          iArraySize;
	int          i;   
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"parse json err");   
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"get object item err");
    cJSON_Delete(iJsonObj);  
    return -1;
  }
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"get object err");
 		cJSON_Delete(iJsonObj);  
 		return -1;
  }
  Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"obj_2 : %s\n", pJsonSub->valuestring);
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"get three object err3");
  	cJSON_Delete(iJsonObj); 
 		return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"get array length err");
	    cJSON_Delete(iJsonObj); 
	 		return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"cJSON_GetArraySize: iArraySize=%d\n",iArraySize);  
    for(i=0;i<iArraySize;i++)  
    { 
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], PARSEJSONERR,"json array %d\n", iArraySize); 
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(sclLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"UCID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iUCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string); 
 				memcpy(sclLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iCallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallDirect");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iCallDirect,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ANI");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iANI,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"DNIS");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iDNIS,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"MakeCall_Time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iMakeCall_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Deliver_time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iDeliver_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Establish_time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEstablish_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_time");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEnd_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Reason");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEnd_Reason,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iVDN,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Next_UCID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iNext_UCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Next_CallID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iNext_CallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Talk_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iTalk_Dur = iArrayItem->valueint;    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Ring_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iRing_Dur = iArrayItem->valueint;    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Dur");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iHold_Dur = iArrayItem->valueint;    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"HoldCount");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iHoldCount = iArrayItem->valueint;    
      }      
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConsultCount");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iConsultCount = iArrayItem->valueint;    
      }         			
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConferenceCount");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], PARSEJSONERR,"cJSON_GetObjectItem: type=%d, int is %s\n",iArrayItem->type,iArrayItem->string);  
 				sclLog[i].iConferenceCount = iArrayItem->valueint;    
      }
    }
  }
  cJSON_Delete(iJsonObj);  
 	return 0;
}

//解析签入签明细的JSON(单元测试通过)
int SignInOutJsonDecode(char* lpJStr, ULANE_AgentSignInLog asiLog[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	cJSON*       iJsonArray;
	cJSON*       iArrayItem;
	cJSON*       iItemObject;
	int          iArraySize;
	int          i;   
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "parse json err"); 
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize == 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get json array err");
    	cJSON_Delete(iJsonObj);
  	  return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetArraySize: iArraySize=%d\n",iArraySize); 
    for(i=0;i<iArraySize;i++)  
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "json array %d\n", iArraySize);   
      iItemObject=cJSON_GetArrayItem(iJsonArray, i); 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentLogonID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iAgentLogonID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asiLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asiLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"OpState");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iOpState,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentSkill");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asiLog[i].iAgentSkill,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CDTime");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asiLog[i].iCDTime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }    
    }  
  } 
  cJSON_Delete(iJsonObj);
  return 0;
}

//解析坐席实时状态JSON
int AgentStateLogJsonDecode(char* lpJStr, ULANE_AgentStateLog asTimeLog[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	cJSON*       iJsonArray;
	cJSON*       iArrayItem;
	cJSON*       iItemObject;
	int          iArraySize;
	int          i;   
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{ 
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "parse json err"); 
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get object item err");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get object err");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize == 0)
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[4], -1, "get json array err");
    	cJSON_Delete(iJsonObj);
  	  return -1;
    }
    Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetArraySize: iArraySize=%d\n",iArraySize); 
    for(i=0;i<iArraySize;i++)  
    {
    	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "json array %d\n", iArraySize);   
      iItemObject=cJSON_GetArrayItem(iJsonArray, i); 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asTimeLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asTimeLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"LogonTime");  
      if(iArrayItem!=NULL)  
      {
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iLogonTime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"State");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iState,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AUX_Code");  
      if(iArrayItem!=NULL)  
      {  
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asTimeLog[i].iAUX_Code,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Begintime");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iBegintime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"pre_state");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iPre_state,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Pre_aux");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iPre_aux,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"StateInfo");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iStateInfo,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CustPhone");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iCustPhone,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }   
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallDirect");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iCallDirect,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      { 
      	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 1, "cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);   
 				memcpy(asTimeLog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
    }  
  } 
  cJSON_Delete(iJsonObj);
  return 0;	
}