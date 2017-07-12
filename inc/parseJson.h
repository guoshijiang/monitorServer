/*=============================================================
 *�ļ�����: parseJson.h
 *����ʱ��: 2017.6.19
 *��    ��: ������
 *�޸ļ�¼:
 *   2017.6.19 �����ļ�
 *����������
 *  ��װ����JSON�ļ�
 * ===========================================================*/
#ifndef __PARSEJSON_H
#define __PARSEJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJson.h"
#include "jsonCheck.h"

#ifdef __cplusplus
extern "C" {
#endif

//��Ķ���
#define MSGLEN         100     //�ṹ�����������

//�����붨��
#define PARSEJSONERR   6000    //����JSON����

//�������Ͷ���
typedef char ULANE_Array[MSGLEN];

//��½�ӿ�
typedef struct _ULANE_AgentLogon
{
	ULANE_Array iCmdName;
	ULANE_Array iMsgID;
	ULANE_Array iAgentid;
	ULANE_Array iAgenttype;
}ULANE_AgentLogon;

//��������ӿ�
typedef struct _ULANE_RaiseHandReqMsg
{
	ULANE_Array	iCmdName;
	ULANE_Array	iAgentId;
	ULANE_Array	iRaiseHandId;
	ULANE_Array	iEventType;
	ULANE_Array	iEventItem;
	ULANE_Array	iStation;
	ULANE_Array	iMemo;
	ULANE_Array	iRaiseHandTime;
	ULANE_Array	iRequestAgent;
	ULANE_Array	iAnswerType;
	ULANE_Array	iOrganizationId; 
	ULANE_Array	iOrganizationName;
}ULANE_RaiseHandReqMsg;

//��Ϣ���ͽӿ�
typedef struct _ULANE_SendMsg
{
	ULANE_Array   iCmdName;
	ULANE_Array   iAgentId;
	ULANE_Array   iAgentName;
	ULANE_Array   iRaiseHandId;
	ULANE_Array   iToAgentId;
	ULANE_Array   iToAgent;
	ULANE_Array   iStation;
	ULANE_Array   iSendTime;  
	ULANE_Array   iMessageType;
	ULANE_Array   iMessage;
	ULANE_Array   iOrganizationId;
	ULANE_Array   iOrganizationName;
}ULANE_SendMsg;

//�ֻ�״̬��ϸ
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

//����ͨ����ϸ
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
	int         iVDN_Dur;
	int         iQueue_Dur;
	int         iRing_Dur; 
	int         iTalk_Dur; 
	int         iHold_Count;
	int         iHold_Dur;  
	int         iTransferCount;
	int         iConferenceCount;
}ULANE_VDNCallLog;

//����ͨ����ϸ
typedef struct _ULANE_StationCallLog
{
	ULANE_Array iAgentID;
	ULANE_Array iStation;
	ULANE_Array iUCID;
	ULANE_Array iFrom_STR;    
	ULANE_Array iCallID;    
	ULANE_Array iCallDirect;
	ULANE_Array iANI;
	ULANE_Array iDNIS;    
	ULANE_Array iMakeCall_Time;
	ULANE_Array iDeliver_time;
	ULANE_Array iEstablish_time;	
	ULANE_Array iEnd_time; 
	ULANE_Array iEnd_Reason;
	ULANE_Array iVDN;     
	ULANE_Array iSplit;  
	ULANE_Array iNext_UCID;
	ULANE_Array iNext_CallID;
	int         iTalk_Dur; 
	int         iRing_Dur; 
	int         iHold_Dur; 
	int         iHoldCount;
	int         iConsultCount;
	int         iConferenceCount;
	
}ULANE_StationCallLog;

//ǩ��ǩ����ϸ
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

//��ϯʵʱ״̬
typedef struct _ULANE_AgentStateLog
{
	ULANE_Array iAgentID;
	ULANE_Array iStation;
	ULANE_Array iLogonTime;
	ULANE_Array iState;
	ULANE_Array iAUX_Code;
	ULANE_Array iBegintime;	
	ULANE_Array iPre_state;
	ULANE_Array iPre_aux;
	ULANE_Array iStateInfo;
	ULANE_Array iCustPhone;
	ULANE_Array iCallDirect;
	ULANE_Array iSplit;
}ULANE_AgentStateLog;

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ��
 *����ֵ:   �ɹ������¼�����,ʧ�ܷ��ش�����
 *����˵��: ��ȡ��cmdName������json������������
 *===========================================================*/
char *CmdNameParse(char *lpJStr);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ��
 *����ֵ:   �ɹ������¼�����,ʧ�ܷ��ش�����
 *����˵��: ������������ӿڵ�JSON��
 *===========================================================*/
int RaiseHandReqJsonParse(char* lpJStr, ULANE_RaiseHandReqMsg rhReqMsg[]);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ��
 *����ֵ:   �ɹ������¼�����,ʧ�ܷ��ش�����
 *����˵��: ��Ϣ���ͽӿ�
 *===========================================================*/
int SendMsgJsonParse(char* lpJStr, ULANE_SendMsg uSendMsg[]);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ��
 *����ֵ:   �ɹ������¼�����,ʧ�ܷ��ش�����
 *����˵��: ����JSON��������
 *===========================================================*/
char *EveNameParse(char *lpJStr);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ��
 *����ֵ:   �ɹ�����JSON���ڲ�����ĳ���,ʧ�ܷ��ش�����
 *����˵��: ��ȡjson���鳤��
 *===========================================================*/
int GetJsonArrayInnerLen(char *lpJStr);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: ageLogon      �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: ������½��ϸ��JSON
 *===========================================================*/
int LogoJsonDecode(char* lpJSte, ULANE_AgentLogon ageLogon[]);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: extLog        �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: �����ֻ�״̬��ϸ��JSON
 *===========================================================*/
int ExtJsonDecode(char* lpJStr, ULANE_ExtStateLog extLog[]);  

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: vcllog        �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: ��������ͨ����ϸ��JSON
 *===========================================================*/
int TelinJsonDecode(char* lpJStr, ULANE_VDNCallLog vcllog[]);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: sclLog        �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: ��������ͨ����ϸ��JSON
 *===========================================================*/
int TeloutExtJsonDecode(char* lpJStr, ULANE_StationCallLog sclLog[]);

/*=========================================================
 *�������: lpJStr        JSON��
 *��������: asiLog        �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: ����ǩ��ǩ��ϸ��JSON
 *===========================================================*/
int SignInOutJsonDecode(char* lpJStr, ULANE_AgentSignInLog asiLog[]);


/*=========================================================
 *�������: lpJStr        JSON��
 *��������: asiLog        �ṹ������
 *����ֵ:   �ɹ�����0, ʧ�ܷ��ش�����
 *����˵��: ������ϯʵʱ״̬JSON
 *===========================================================*/
int AgentStateLogJsonDecode(char* lpJStr, ULANE_AgentStateLog asTimeLog[]);

#ifdef __cplusplus
}
#endif


#endif
