/*=============================================================
 *文件名称: minitorUlaneServer.h
 *创建时间: 2017.6.23
 *作    者: 郭世江
 *修改记录:
 *2017.6.23 创建文件
 *功能描述：业务处理
 * ===========================================================*/
#ifndef _MINITORSERVER_H_
#define _MINITORSERVER_H_
//头文件区域
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

//宏定义区域
#define SERVERINFOLEN        64
#define MAXLINE              10240
#define MAX_LISTEN           10
#define MAXCONNQ             30
#define AGENTLEN             64
#define ALLCLIENTNUM         -1             //群发
#define SINGLECLIENTNUM       0             //单发
#define LOGINNUM              1             //登录    

//错误码的定义区域
#define SERVINITERR         1000
#define DECODEEXTJSONERR    1001
#define ANSWERSUCC          1002

//SQL语句定义区域
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
 
//坐席实时状态SQL预处理区域
#define DELETE_AgentStateLog "DELETE FROM m_state"
#define INSERT_AgentStateLog "INSERT INTO m_state(ID, AgentID, Station, LogonTime, State, AUX_Code, \
Begintime, Pre_aux, pre_state, StateInfo, CustPhone, CallDirect, Split, CRT_DT )\
VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)"

//数据类型定义区域
typedef struct _UlaneServerInfo
{
	char           iSerId[SERVERINFOLEN];                   //服务端编号
	char           iSerIP[SERVERINFOLEN];                   //服务器IP
	unsigned short iSerPort;                                //服务器端的端口
	char           iDBHost[SERVERINFOLEN];                  //数据库的主机地址
	char           iDBUser[SERVERINFOLEN];                  //数据库的登录用户名
	char           iDBPasswd[SERVERINFOLEN];                //数据库登录密码
	char           iDBName[SERVERINFOLEN];                  //使用的数据库名字
	unsigned int   iDBPort;                                 //默认传0
  char           iDBUnixSock[SERVERINFOLEN];              //默认传空              
	unsigned long  iDBClientFlag;                           //默认传0                          
}UlaneServerInfo;

//客户端登录信息结构体
struct iClientInfo{
    int isEmpty;                   
    ULANE_Array agentId;            //客户端agentid
    int  flag;                      //标志位
};

//客户端登录结构体处理
struct client{
    struct iClientInfo clients_info[FD_SETSIZE];
    int maxfd;
};

typedef struct MsgToAgentId
{
	ULANE_Array iToAgentId;	
}msgAgentId;

//全局变量定义
struct client                   MsgData;
sem_t                           sem;
char                            retime[20];
UlaneServerInfo                 iServerInfo;
ULANESQL                        iSql;

/*=========================================================
 *函数入参:  iUlaneServerInfo    初始化信息存放的结构体
 *函数出参:  无
 *返回值:    成功返回 0， 失败返回错误码
 *功能说明:  初始化服务端的信息
 *===========================================================*/
int Ulane_ServerInfoInit(UlaneServerInfo* iServerInfo);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:   
 *功能说明:数据库初始化
 *===========================================================*/
ULANESQL Ulane_ServerDBInit(ULANESTR iHost, ULANESTR iUser, ULANESTR iPasswd, ULANESTR iDbName, ULANEUNSIGNINT iPort, ULANESTR iUnixSock, ULANELONG iClientFlag);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:   
 *功能说明:信号处理
 *===========================================================*/
void sigint(int signo);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:
 *功能说明:消息发送给所有客户端
 *===========================================================*/
static int MsgSendToAll(int iSockfd, char *Msg, int Msglen);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:
 *功能说明:把消息发送给所有的监控坐席
 *===========================================================*/
int MsgSendToAgentTypeOne(int iSockfd, char *Msg, int Msglen);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:
 *功能说明:把消息转发给普通坐席
 *===========================================================*/
int MsgSendToAgentTypeTwo(int iSockfd, char *Msg, int Msglen, char *iAgentId);

/*=========================================================
 *函数入参:  
 *函数出参:  
 *返回值:  
 *功能说明:转发消息,对已连接的客户端的数据收发处理
 *===========================================================*/
void HandleTransMsg(int iSockfd, char *Msg, int Msglen);

/*=========================================================
*函数入参: inJson  客户端发过来的Json串
*函数出参: outJson 处理之后传出的JSON串
					 outLen  处理出去的json串的长度
*返回值:   成功返回事件的类型， 失败返回NULL
*功能说明: 终端登录接口
*===========================================================*/
int TerminalLogin(int iSockfd, char *inJson, char** outJson);

/*=========================================================
*函数入参:无   
*函数出参:无
*返回值:返回当日日期字符串
*功能说明:获取系统的时间
*===========================================================*/
char *Timer();

/*=========================================================
*函数入参:inJson客户端发过来的Json串
*函数出参:outJson处理之后传出的JSON串			 
*返回值:    
*功能说明:分机状态明细统一处理
*===========================================================*/
int StationStateLog(char *inJson, char** outJson);

/*=========================================================
*函数入参:  inJson  客户端发过来的Json串
*函数出参:  outJson 处理之后传出的JSON串				 
*返回值:    
*功能说明:  签入签出明细统一处理
*===========================================================*/
int AgentSignInLog(char *inJson, char** outJson);

/*=========================================================
*函数入参:  inJson  客户端发过来的Json串
*函数出参:  outJson 处理之后传出的JSON串			 
*返回值:    
*功能说明:  呼入通话明细统一处理
*===========================================================*/
int VdnCallLog(char *inJson, char** outJson);

/*=========================================================
*函数入参:  inJson  客户端发过来的Json串
*函数出参:  outJson 处理之后传出的JSON串			 
*返回值:    
*功能说明:  呼出通话明细统一处理
*===========================================================*/
int StationCallLog(char *inJson, char** outJson);

/*=========================================================
*函数入参:  inJson  客户端发过来的Json串
*函数出参:  outJson 处理之后传出的JSON串				 
*返回值:    
*功能说明:  坐席实时状态统一处理
*===========================================================*/
int AgentStateLog(char *inJson, char** outJson);

/*=========================================================
*函数入参:  inJson  客户端发过来的Json串
*函数出参:  outJson 处理之后传出的JSON串			 
*返回值:    
*功能说明:  技能组实时排队信息统一处理(暂时不做处理)
*===========================================================*/
int SkillQueueInfo(char *inJson, char** outJson);

/*=========================================================
*函数入参:  
*函数出参:  		 
*返回值:    
*功能说明:举手消息处理
*===========================================================*/
int HandleRaiseHandMsg(int iSockfd, char *inJson);

/*=========================================================
*函数入参:  
*函数出参:  		 
*返回值:    
*功能说明:消息发送处理
*===========================================================*/
int HandleSendMsg(int iSockfd, char *inJson);

#ifdef __cplusplus
}
#endif

#endif
