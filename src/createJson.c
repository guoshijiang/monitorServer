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
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"eventName, succBuf and errMsg is NULL");
		return NULL;
	}
	cJSON *AnsJson = cJSON_CreateObject();
	if(AnsJson == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"json is NULL");
		return NULL;	
	}
	cJSON_AddStringToObject(AnsJson,"cmdName", eventName);
	cJSON_AddStringToObject(AnsJson,"result", succErrBuf);
	cJSON_AddStringToObject(AnsJson,"errormsg", errMsg);
  outJson = cJSON_Print(AnsJson);
  if(outJson == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"outJson is NULL");
  	return NULL;	
  }
  strcat(outJson, "eof");
  cJSON_Delete(AnsJson); 
  return outJson;
}

//举手信息实时推送接口
char* CreateRaiseHandMsg(char *inJson)
{
	int                  iRet;
	cJSON                *iSendMsgJson;
	cJSON                *iSendArrayMsg;
	char*                JsonArr;
	int                  outLen;
	ULANE_RaiseHandReqMsg        rhReqMsg[1] = {{0}};
	if(inJson == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"inJson is NULL");
		return NULL;	
	}	
	iRet = RaiseHandReqJsonParse(inJson, rhReqMsg);
	if(iRet == -1)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], iRet,"SendMsgJsonParse Is Error");
		return NULL;
	}
	iSendMsgJson = cJSON_CreateObject();
	if(iSendMsgJson == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateObject Is Error");
		return NULL;	
	}
	cJSON_AddStringToObject(iSendMsgJson,"msg_id", "1000101010");
	cJSON_AddStringToObject(iSendMsgJson,"event_name","OnRaisehandEvent");
	
	iSendArrayMsg = cJSON_CreateArray(); 
	if(iSendArrayMsg == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateArray Is Error");
		return NULL;
	}   
  cJSON *ObjSendMsg = cJSON_CreateObject(); 
  if(ObjSendMsg == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateObject Is Error");
		return NULL;
  } 
  
  cJSON *MsgItem=cJSON_CreateString(rhReqMsg[0].iAgentId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }  
  cJSON_AddItemToObject(ObjSendMsg, "agentid", MsgItem);  

  MsgItem=cJSON_CreateString(rhReqMsg[0].iRaiseHandId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"raisehandid",MsgItem);  

  MsgItem=cJSON_CreateString(rhReqMsg[0].iEventType); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg, "eventtype", MsgItem);  

  MsgItem=cJSON_CreateString(rhReqMsg[0].iEventItem); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"eventitem", MsgItem);  


  MsgItem=cJSON_CreateString(rhReqMsg[0].iStation);  
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"station",MsgItem);  
	
	MsgItem=cJSON_CreateString(rhReqMsg[0].iMemo);  
	if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"memo",MsgItem);

  MsgItem=cJSON_CreateString(rhReqMsg[0].iRaiseHandTime); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"raisehandtime",MsgItem);

  MsgItem=cJSON_CreateString(rhReqMsg[0].iRequestAgent);  
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"requestagent",MsgItem);
  
  MsgItem=cJSON_CreateString(rhReqMsg[0].iAnswerType);
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }  
  cJSON_AddItemToObject(ObjSendMsg,"answertype",MsgItem);
  
  MsgItem=cJSON_CreateString(rhReqMsg[0].iOrganizationId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"organizationid",MsgItem);
  
  MsgItem=cJSON_CreateString(rhReqMsg[0].iOrganizationName); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"organizationname",MsgItem);
  
  cJSON_AddItemToArray(iSendArrayMsg, ObjSendMsg);   
  cJSON_AddItemToObject(iSendMsgJson,"event_body",iSendArrayMsg);
  
  JsonArr = cJSON_Print(iSendMsgJson);
  if(JsonArr == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_Print Is Error");
		return NULL;
  }
  strcat(JsonArr, "eof"); 
  cJSON_Delete(iSendMsgJson);
  return JsonArr;;
}

// 消息发送接口
char* CreateSendMessage(char *inJson)
{
	int                  iRet;
	cJSON                *iSendMsgJson;
	cJSON                *iSendArrayMsg;
	char*                JsonArr;
	int                  outLen;
	ULANE_SendMsg        sMsg[1] = {{0}};
	if(inJson == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"inJson is NULL");
		return NULL;	
	}	
	iRet = SendMsgJsonParse(inJson, sMsg);
	if(iRet == -1)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], iRet,"SendMsgJsonParse Is Error");
		return NULL;
	}
	iSendMsgJson = cJSON_CreateObject();
	if(iSendMsgJson == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateObject Is Error");
		return NULL;	
	}
	cJSON_AddStringToObject(iSendMsgJson,"msg_id", "1000101010");
	cJSON_AddStringToObject(iSendMsgJson,"event_name","OnRaisehandEvent");
	
	iSendArrayMsg = cJSON_CreateArray(); 
	if(iSendArrayMsg == NULL)
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateArray Is Error");
		return NULL;
	}   
  cJSON *ObjSendMsg = cJSON_CreateObject(); 
  if(ObjSendMsg == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateObject Is Error");
		return NULL;
  } 
  
  cJSON *MsgItem=cJSON_CreateString(sMsg[0].iAgentId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }  
  cJSON_AddItemToObject(ObjSendMsg, "agentid", MsgItem);  

  MsgItem=cJSON_CreateString(sMsg[0].iAgentName); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"agentname",MsgItem);  

  MsgItem=cJSON_CreateString(sMsg[0].iRaiseHandId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg, "raisehandid", MsgItem);  

  MsgItem=cJSON_CreateString(sMsg[0].iToAgentId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"toagentid", MsgItem);  

  MsgItem=cJSON_CreateString(sMsg[0].iToAgent);  
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"toagent",MsgItem);  
    
	MsgItem=cJSON_CreateString(sMsg[0].iStation);  
	if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"station",MsgItem);

  MsgItem=cJSON_CreateString(sMsg[0].iSendTime); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"sendtime",MsgItem);

  MsgItem=cJSON_CreateString(sMsg[0].iMessageType);  
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }
  cJSON_AddItemToObject(ObjSendMsg,"messagetype",MsgItem);
              
  MsgItem=cJSON_CreateString(sMsg[0].iMessage);
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  }  
  cJSON_AddItemToObject(ObjSendMsg,"message",MsgItem);

  MsgItem=cJSON_CreateString(sMsg[0].iOrganizationId); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"organizationid",MsgItem);
      	               
  MsgItem=cJSON_CreateString(sMsg[0].iOrganizationName); 
  if(MsgItem == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_CreateString Is Error");
		return NULL;
  } 
  cJSON_AddItemToObject(ObjSendMsg,"organizationname",MsgItem);
  
  cJSON_AddItemToArray(iSendArrayMsg, ObjSendMsg);   
  cJSON_AddItemToObject(iSendMsgJson,"event_body",iSendArrayMsg);
  
  JsonArr = cJSON_Print(iSendMsgJson);
  if(JsonArr == NULL)
  {
  	Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], CREATEJSONLOGOERR,"cJSON_Print Is Error");
		return NULL;
  }
  strcat(JsonArr, "eof"); 
  cJSON_Delete(iSendMsgJson);
  return JsonArr;;
}