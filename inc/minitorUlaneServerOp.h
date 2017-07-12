/*=============================================================
 *�ļ�����: minitorUlaneServer.h
 *����ʱ��: 2017.6.23
 *��    ��: ������
 *�޸ļ�¼:
 *2017.6.23 �����ļ�
 *����������ҵ����
 * ===========================================================*/
#ifndef _MINITORSERVER_H_
#define _MINITORSERVER_H_
//ͷ�ļ�����
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <uuid/uuid.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h> 
#include <pthread.h>
#include "cJson.h"
#include "ulaneLog.h"
#include "config.h"
#include "ulaneMysql.h"
#include "ulaneStmtMysql.h"
#include "parseJson.h"
#include "createJson.h"
#include "poolSocket.h"

#ifdef __cplusplus
extern "C" {
#endif

//�궨������
#define SERVERINFOLEN        64
#define MAXLINE              10240
#define MAX_LISTEN           10
#define MAXCONNQ             30
#define AGENTLEN             64
#define ALLCLIENTNUM         -1             //Ⱥ��
#define SINGLECLIENTNUM       0             //����
#define LOGINNUM              1             //��¼    

//������Ķ�������
#define SERVINITERR         1000
#define DECODEEXTJSONERR    1001
#define ANSWERSUCC          1002

//SQL��䶨������
#define INSERT_StationStateLog "INSERT INTO m_stationstatelog \
(ID, AgentID, Station, From_STR, State, AUX_Code, Begintime, Endtime, StateInfo, CustPhone, CallDirect, Split)\
VALUES(?,?,?,?,?,?,?,?,?,?,?,?)"
 
#define INSERT_AgentSignOutInLog "INSERT INTO m_agentloginlog \
(ID, AgentLogonID, AgentID, Station, AgentSkill, OpState, CDTime, From_STR)\
VALUES(?,?,?,?,?,?,?,?)"
 
#define INSERT_VdnCallLog "INSERT INTO m_vdncallrecord \
(ID, VDN, CallID, ANI, DNIS, Enter_Time, Queue_Time, Deliver_Time, Estab_Time, End_Time, Station, AgentID, Split,\
End_Reason, VDN_Dur, Queue_Dur, Ring_Dur, Talk_Dur, Hold_Count, Hold_Dur, TransferCount, ConferenceCount, CRT_DT,\
UCID, From_STR) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
 
#define INSERT_StationCallLog "INSERT INTO m_stationcallrecord\
(ID, AgentID, Station, UCID, CallID, CallDirect, ANI, DNIS, MakeCall_Time, Deliver_Time, Establish_time, End_Time, End_Reason,VDN, Split,Talk_Dur, Ring_Dur, Hold_Dur,\
HoldCount, ConsultCount, ConferenceCount, Next_UCID, Next_CallID, From_STR, CRT_DT )\
VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
 
//��ϯʵʱ״̬SQLԤ��������
#define DELETE_AgentStateLog "DELETE FROM m_state"
#define INSERT_AgentStateLog "INSERT INTO m_state(ID, AgentID, Station, LogonTime, State, AUX_Code, \
Begintime, Pre_aux, pre_state, StateInfo, CustPhone, CallDirect, Split, CRT_DT )\
VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)"

//�������Ͷ�������
typedef struct _UlaneServerInfo
{
	char           iSerId[SERVERINFOLEN];                   //����˱��
	char           iSerIP[SERVERINFOLEN];                   //������IP
	unsigned short iSerPort;                                //�������˵Ķ˿�
	char           iDBHost[SERVERINFOLEN];                  //���ݿ��������ַ
	char           iDBUser[SERVERINFOLEN];                  //���ݿ�ĵ�¼�û���
	char           iDBPasswd[SERVERINFOLEN];                //���ݿ��¼����
	char           iDBName[SERVERINFOLEN];                  //ʹ�õ����ݿ�����
	unsigned int   iDBPort;                                 //Ĭ�ϴ�0
  char           iDBUnixSock[SERVERINFOLEN];              //Ĭ�ϴ���              
	unsigned long  iDBClientFlag;                           //Ĭ�ϴ�0                          
}UlaneServerInfo;

//�ͻ��˵�¼��Ϣ�ṹ��
struct iClientInfo{
    int isEmpty;                   
    char *agentId;                  //�ͻ���agentid
    int  flag;                      //��־λ
};

//�ͻ��˵�¼�ṹ�崦��
struct client{
    struct iClientInfo clients_info[FD_SETSIZE];
    int maxfd;
};

typedef struct MsgToAgentId
{
	ULANE_Array iToAgentId;	
}msgAgentId;

//ȫ�ֱ�������
struct client                   MsgData;
sem_t                           sem;
char                            retime[20];
UlaneServerInfo                 iServerInfo;
ULANESQL                        iSql;

/*=========================================================
 *�������:  iUlaneServerInfo    ��ʼ����Ϣ��ŵĽṹ��
 *��������:  ��
 *����ֵ:    �ɹ����� 0�� ʧ�ܷ��ش�����
 *����˵��:  ��ʼ������˵���Ϣ
 *===========================================================*/
int Ulane_ServerInfoInit(UlaneServerInfo* iServerInfo);

/*=========================================================
 *�������:  
 *��������:  
 *����ֵ:   
 *����˵��:���ݿ��ʼ��
 *===========================================================*/
ULANESQL Ulane_ServerDBInit(ULANESTR iHost, ULANESTR iUser, ULANESTR iPasswd, ULANESTR iDbName, ULANEUNSIGNINT iPort, ULANESTR iUnixSock, ULANELONG iClientFlag);

/*=========================================================
 *�������:  
 *��������:  
 *����ֵ:   
 *����˵��:�źŴ���
 *===========================================================*/
void sigint(int signo);

/*=========================================================
 *�������:  
 *��������:  
 *����ֵ:
 *����˵��:��Ϣ���͸����пͻ���
 *===========================================================*/
int MsgSendToAll(int iSockfd, char *Msg, int Msglen);

/*=========================================================
 *�������:  
 *��������:  
 *����ֵ:����ϢȺ�����߷����ض���sockfd�ͻ�����   
 *����˵��:ת����Ϣ,�������ӵĿͻ��˵������շ�����
 *===========================================================*/
void HandleTransMsg(int iSockfd, char *Msg, int Msglen);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��
					  outLen  �����ȥ��json���ĳ���
*����ֵ:    �ɹ������¼������ͣ� ʧ�ܷ���NULL
*����˵��:�ն˵�¼�ӿ�
*===========================================================*/
int TerminalLogin(int iSockfd, char *inJson, char** outJson);

/*=========================================================
*�������:��   
*��������:��
*����ֵ:���ص��������ַ���
*����˵��:��ȡϵͳ��ʱ��
*===========================================================*/
char *Timer();

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��			 
*����ֵ:    
*����˵��:  �ֻ�״̬��ϸͳһ����
*===========================================================*/
int StationStateLog(char *inJson, char** outJson);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��				 
*����ֵ:    
*����˵��:  ǩ��ǩ����ϸͳһ����
*===========================================================*/
int AgentSignInLog(char *inJson, char** outJson);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��			 
*����ֵ:    
*����˵��:  ����ͨ����ϸͳһ����
*===========================================================*/
int VdnCallLog(char *inJson, char** outJson);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��			 
*����ֵ:    
*����˵��:  ����ͨ����ϸͳһ����
*===========================================================*/
int StationCallLog(char *inJson, char** outJson);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��				 
*����ֵ:    
*����˵��:  ��ϯʵʱ״̬ͳһ����
*===========================================================*/
int AgentStateLog(char *inJson, char** outJson);

/*=========================================================
*�������:  inJson  �ͻ��˷�������Json��
*��������:  outJson ����֮�󴫳���JSON��			 
*����ֵ:    
*����˵��:  ������ʵʱ�Ŷ���Ϣͳһ����(��ʱ��������)
*===========================================================*/
int SkillQueueInfo(char *inJson, char** outJson);

/*=========================================================
*�������:  
*��������:  		 
*����ֵ:    
*����˵��:������Ϣ����
*===========================================================*/
int RaiseHandMsg(int iSockfd, char *inJson, char** outJson);

/*=========================================================
*�������:  
*��������:  		 
*����ֵ:    
*����˵��:��Ϣ���ʹ���
*===========================================================*/
int HandleSendMsg(int iSockfd, char *inJson, char** outJson);

#ifdef __cplusplus
}
#endif

#endif