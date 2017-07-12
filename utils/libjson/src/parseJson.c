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
 
//获取解析JSON串的类型(单元测试通过)
char *EveNameParse(char *lpJStr)
{
	 cJSON* pJson;
	 cJSON* pJsonObj;
	 if(lpJStr == NULL)
   {
   	 printf("lpJStr is NULL\n");
     return NULL;
   }   
   pJson = cJSON_Parse(lpJStr);
   if( pJson == NULL)
   {
	 	 printf("pJson is NULL\n");
	   return NULL;
   }
   if (pJson->type == cJSON_Object )
   {
     pJsonObj = cJSON_GetObjectItem(pJson, "event_name");
     if (pJsonObj == NULL )
     {
   		 printf("pJsonObj is NULL\n");
   		 cJSON_Delete(pJson);
 			 cJSON_Delete(pJsonObj); 
       return NULL;
     } 
     return pJsonObj->valuestring;
   }
   cJSON_Delete(pJson);
   cJSON_Delete(pJsonObj); 
}

//获取json数组长度(单元测试通过)
int GetJsonArrayInnerLen(char *lpJStr)
{
	cJSON*       iJsonObj;
	cJSON*       iJsonArray;
	int          iArraySize;  
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{  
	  printf("parse json err\n");
	  return -1;
	}
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	printf("get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	printf("get json array err\n");	
    	cJSON_Delete(iJsonObj);
    	cJSON_Delete(iJsonArray);
    	return -1;
    }
    printf("cJSON_GetArraySize: iArraySize=%d\n",iArraySize);
  }
  return iArraySize;
}

int LogoJsonDecode(char* lpJStr, ULANE_AgentLogon ageLogon[])
{
	cJSON*       iJsonObj;
	cJSON*       pJsonSub;
	iJsonObj=cJSON_Parse(lpJStr);  
	if (!iJsonObj)  
	{  
	  printf("parse json err\n");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "cmdName");
  if(pJsonSub == NULL)
  {
    printf("get object item err\n");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  memcpy(ageLogon[0].iCmdName,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  printf("obj_1 : %s\n", pJsonSub->valuestring);
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "msgID");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iMsgID,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  printf("obj_2 : %s\n", pJsonSub->valuestring);
   pJsonSub = cJSON_GetObjectItem(iJsonObj, "agentid");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iAgentid,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  printf("obj_3 : %s\n", pJsonSub->valuestring);
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "agenttype");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  memcpy(ageLogon[0].iAgenttype,pJsonSub->valuestring,strlen(pJsonSub->valuestring)); 
  printf("obj_4 : %s\n", pJsonSub->valuestring);
  cJSON_Delete(iJsonObj);
	return 0;
}

//解析分机状态明细的JSON(单元测试通过)
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
	  printf("parse json err\n");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
    printf("get object item err\n");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  printf("obj_1 : %s\n", pJsonSub->valuestring);
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  printf("obj_2 : %s\n", pJsonSub->valuestring);
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	printf("get three object err3");
  	cJSON_Delete(iJsonObj);
  	cJSON_Delete(pJsonSub);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	printf("get json array length err\n");
    	cJSON_Delete(iJsonObj);
  	  cJSON_Delete(pJsonSub);	
  	  cJSON_Delete(iJsonArray);	
  	  return -1;
    }
    printf("cJSON_GetArraySize: iArraySize=%d\n",iArraySize);  
    for(i=0;i<iArraySize;i++)  
    {   
    	printf("json array %d\n", iArraySize);
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
       
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"State");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iState,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AUX_Code");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iAUX_Code,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Begintime");  
      if(iArrayItem!=NULL)  
      {  
         printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iBegintime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Endtime");  
      if(iArrayItem!=NULL)  
      {  
         printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iEndtime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }    
      iArrayItem=cJSON_GetObjectItem(iItemObject,"StateInfo");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iStateInfo,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CustPhone");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iCustPhone,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallDirect");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iCallDirect,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(extLog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
    }  
    cJSON_Delete(iJsonObj);  
  } 
  return 0;
}

//解析呼入通话明细的JSON(单元测试通过)
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
	  printf("parse json err\n");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
    printf("get object item err\n");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	printf("get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	printf("get json array length err\n");
    	cJSON_Delete(iJsonObj);
  	  return -1;
    }
    printf("cJSON_GetArraySize: iArraySize=%d\n",iArraySize);  
    for(i=0;i<iArraySize;i++)  
    {   
    	printf("json array %d\n", iArraySize);
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN");  
      if(iArrayItem!=NULL)  
      {  
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iVDN,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Split");  
      if(iArrayItem!=NULL)  
      { 
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iSplit,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 

      iArrayItem=cJSON_GetObjectItem(iItemObject,"UCID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iUCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iCallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ANI");  
      if(iArrayItem!=NULL)  
      {  
         printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iANI,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"DNIS");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iDNIS,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }    
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Enter_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnter_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Queue_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iQueue_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Deliver_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iDeliver_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Estab_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEstab_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnd_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Reason");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iEnd_Reason,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iVDN_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Queue_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iQueue_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Ring_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iRing_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Talk_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iTalk_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Count");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iHold_Count,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iHold_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"TransferCount");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iTransferCount,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConferenceCount");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(vcllog[i].iConferenceCount,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
    } 
  }
  cJSON_Delete(iJsonObj);  
  return 0;
}

//解析呼出通话明细的JSON(单元测试未做)
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
	  printf("parse json err\n");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
    printf("get object item err\n");
    cJSON_Delete(iJsonObj);  
    return -1;
  }
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);  
 		return -1;
  }
  printf("obj_2 : %s\n", pJsonSub->valuestring);
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	printf("get three object err3");
  	cJSON_Delete(iJsonObj); 
 		return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize < 0)
    {
    	printf("get array length err\n");
	    cJSON_Delete(iJsonObj); 
	 		return -1;
    }
    printf("cJSON_GetArraySize: iArraySize=%d\n",iArraySize);  
    for(i=0;i<iArraySize;i++)  
    {   
    	printf("json array %d\n", iArraySize);
      iItemObject=cJSON_GetArrayItem(iJsonArray, i);
   
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      { 
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 

      iArrayItem=cJSON_GetObjectItem(iItemObject,"UCID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iUCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iCallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CallDirect");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iCallDirect,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      {  
         printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"ANI");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iANI,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  

      iArrayItem=cJSON_GetObjectItem(iItemObject,"DNIS");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iDNIS,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"MakeCall_Time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iMakeCall_Time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }

      iArrayItem=cJSON_GetObjectItem(iItemObject,"Deliver_time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iDeliver_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Establish_time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEstablish_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_time");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEnd_time,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"End_Reason");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iEnd_Reason,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iVDN,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"VDN_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iVDN_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Talk_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iTalk_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Ring_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iRing_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Hold_Dur");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iHold_Dur,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"HoldCount");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iHoldCount,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConsultCount");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iConsultCount,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }           			
      iArrayItem=cJSON_GetObjectItem(iItemObject,"ConferenceCount");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iConferenceCount,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Next_UCID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iNext_UCID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Next_CallID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(sclLog[i].iNext_CallID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));    
      }
    }
  }
  cJSON_Delete(iJsonObj);  
 	return 0;
}

//解析签入签明细的JSON(单元测试未做)
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
	  printf("parse json err\n");
	  return -1;
	}
	pJsonSub = cJSON_GetObjectItem(iJsonObj, "msg_id");
  if(pJsonSub == NULL)
  {
    printf("get object item err\n");
    cJSON_Delete(iJsonObj);
    return -1;
  }
  pJsonSub = cJSON_GetObjectItem(iJsonObj, "event_name");
  if(pJsonSub == NULL)
  {
 		printf("get object err\n");
 		cJSON_Delete(iJsonObj);
 		return -1;
  }
  iJsonArray = cJSON_GetObjectItem(iJsonObj, "event_body");   
  if(iJsonArray==NULL)
  {
  	printf("get three object err3");
  	cJSON_Delete(iJsonObj);
  	return -1;
  }
  else  
  {  
    iArraySize=cJSON_GetArraySize(iJsonArray);
    if(iArraySize == 0)
    {
    	printf("get json array err\n");
    	cJSON_Delete(iJsonObj);
  	  return -1;
    }
    printf("cJSON_GetArraySize: iArraySize=%d\n",iArraySize);  
    for(i=0;i<iArraySize;i++)  
    {   
    	printf("json array %d\n", iArraySize);
      iItemObject=cJSON_GetArrayItem(iJsonArray, i); 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentLogonID");  
      if(iArrayItem!=NULL)  
      {  
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iAgentLogonID,iArrayItem->valuestring,strlen(iArrayItem->valuestring)); 
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"From_STR");  
      if(iArrayItem!=NULL)  
      { 
      	printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iFrom_STR,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      } 
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentID");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iAgentID,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"Station");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iStation,iArrayItem->valuestring,strlen(iArrayItem->valuestring));  
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"OpState");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iOpState,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }  
      iArrayItem=cJSON_GetObjectItem(iItemObject,"AgentSkill");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iAgentSkill,iArrayItem->valuestring,strlen(iArrayItem->valuestring));     
      }
      iArrayItem=cJSON_GetObjectItem(iItemObject,"CDTime");  
      if(iArrayItem!=NULL)  
      {  
        printf("cJSON_GetObjectItem: type=%d, string is %s\n",iArrayItem->type,iArrayItem->string);  
 				memcpy(asiLog[i].iCDTime,iArrayItem->valuestring,strlen(iArrayItem->valuestring));   
      }    
    }  
  } 
  cJSON_Delete(iJsonObj);
  return 0;
}