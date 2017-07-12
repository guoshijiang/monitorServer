/*=============================================================
 *文件名称: parseJson.h
 *创建时间: 2017.6.19
 *作    者: 郭世江
 *修改记录:
 *   2017.6.19 创建文件
 *功能描述：
 *  封装解析JSON文件
 * ===========================================================*/
#ifndef __PARSEJSON_H
#define __PARSEJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

//宏的定义
#define MSGLEN         100     //结构体数组的容量


//数据类型定义
typedef char ULANE_Array[MSGLEN];

//登陆接口
typedef struct _ULANE_AgentLogon
{
	ULANE_Array iCmdName;
	ULANE_Array iMsgID;
	ULANE_Array iAgentid;
	ULANE_Array iAgenttype;
}ULANE_AgentLogon;



//分机状态明细
typedef struct _ULANE_ExtStateLog
{
	ULANE_Array iAgentID;
  ULANE_Array iStation;
  ULANE_Array iFrom_STR;
  ULANE_Array iState;
  ULANE_Array iAUX_Code;
  ULANE_Array iBegintime;
  ULANE_Array iEndtime;
  ULANE_Array iStateInfo;
  ULANE_Array iCustPhone;
  ULANE_Array iCallDirect;
  ULANE_Array iSplit;
}ULANE_ExtStateLog;

//呼入通话明细
typedef struct _ULANE_VDNCallLog
{
	ULANE_Array iVDN;   
	ULANE_Array iSplit;
	ULANE_Array iUCID;   
	ULANE_Array iCallID;    
	ULANE_Array iFrom_STR;
	ULANE_Array iANI;
	ULANE_Array iDNIS;
	ULANE_Array iEnter_Time;
	ULANE_Array iQueue_Time;
	ULANE_Array iDeliver_Time;
	ULANE_Array iEstab_Time;
	ULANE_Array iStation;
	ULANE_Array iAgentID; 
	ULANE_Array iEnd_Time;
	ULANE_Array iEnd_Reason;
	ULANE_Array iVDN_Dur;
	ULANE_Array iQueue_Dur;
	ULANE_Array iRing_Dur; 
	ULANE_Array iTalk_Dur; 
	ULANE_Array iHold_Count;
	ULANE_Array iHold_Dur;  
	ULANE_Array iTransferCount;
	ULANE_Array iConferenceCount;
}ULANE_VDNCallLog;

//呼出通话明细
typedef struct _ULANE_StationCallLog
{
	ULANE_Array iAgentID;
	ULANE_Array iStation;
	ULANE_Array iUCID;    
	ULANE_Array iCallID;    
	ULANE_Array iCallDirect;
	ULANE_Array iFrom_STR;
	ULANE_Array iANI;
	ULANE_Array iDNIS;    
	ULANE_Array iMakeCall_Time;
	ULANE_Array iDeliver_time;
	ULANE_Array iEstablish_time;	
	ULANE_Array iEnd_time; 
	ULANE_Array iEnd_Reason;
	ULANE_Array iVDN;     
	ULANE_Array iSplit;   
	ULANE_Array iVDN_Dur;
	ULANE_Array iTalk_Dur; 
	ULANE_Array iRing_Dur; 
	ULANE_Array iHold_Dur; 
	ULANE_Array iHoldCount;
	ULANE_Array iConsultCount;
	ULANE_Array iConferenceCount;
	ULANE_Array iNext_UCID;
	ULANE_Array iNext_CallID;
}ULANE_StationCallLog;

//签入签出明细
typedef struct _ULANE_AgentSignInLog
{
	ULANE_Array iAgentLogonID;
	ULANE_Array iFrom_STR;
	ULANE_Array iAgentID;
	ULANE_Array iStation;
	ULANE_Array iOpState;
	ULANE_Array iAgentSkill;	
	ULANE_Array iCDTime;
}ULANE_AgentSignInLog;

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: 无
 *返回值:   成功返回事件类型,失败返回错误码
 *功能说明: 解析JSON串的类型
 *===========================================================*/
char *EveNameParse(char *lpJStr);

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: 无
 *返回值:   成功返回JSON串内部数组的长度,失败返回错误码
 *功能说明: 获取json数组长度
 *===========================================================*/
int GetJsonArrayInnerLen(char *lpJStr);


/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: ageLogon      结构体数组
 *返回值:   成功返回0, 失败返回错误码
 *功能说明: 解析登陆明细的JSON
 *===========================================================*/
int LogoJsonDecode(char* lpJSte, ULANE_AgentLogon ageLogon[]);

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: extLog        结构体数组
 *返回值:   成功返回0, 失败返回错误码
 *功能说明: 解析分机状态明细的JSON
 *===========================================================*/
int ExtJsonDecode(char* lpJStr, ULANE_ExtStateLog extLog[]);  

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: vcllog        结构体数组
 *返回值:   成功返回0, 失败返回错误码
 *功能说明: 解析呼入通话明细的JSON
 *===========================================================*/
int TelinJsonDecode(char* lpJStr, ULANE_VDNCallLog vcllog[]);

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: sclLog        结构体数组
 *返回值:   成功返回0, 失败返回错误码
 *功能说明: 解析呼出通话明细的JSON
 *===========================================================*/
int TeloutExtJsonDecode(char* lpJStr, ULANE_StationCallLog sclLog[]);

/*=========================================================
 *函数入参: lpJStr        JSON串
 *函数出参: asiLog        结构体数组
 *返回值:   成功返回0, 失败返回错误码
 *功能说明: 解析签入签明细的JSON
 *===========================================================*/
int SignInOutJsonDecode(char* lpJStr, ULANE_AgentSignInLog asiLog[]);
#endif
