/*=============================================================
 *文件名称: minitorUlaneServer.c
 *创建时间: 2017.6.23
 *作    者: 郭世江
 *修改记录:
 *2017.6.23 创建文件
 *功能描述：业务处理
 * ===========================================================*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h> 
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<time.h>
#include "ulaneMysql.h"
#include "ulaneStmtMysql.h"
#include "minitorUlaneServerOp.h"
#include "queueMsg.h"

#define STRING_SIZE 1024

//全局变量定义
ULANE_ExtStateLog       iStateLog[1024]      = {{0}};    
ULANE_AgentSignInLog    iSignInOutLog[1024]  = {{0}};
ULANE_VDNCallLog        iVDNCallLog[1024]    = {{0}};
ULANE_StationCallLog    iStationCallLog[1024]= {{0}};
ULANE_AgentStateLog     iAgentStateLog[1024] = {{0}};
ULANE_RaiseHandReqMsg   iRHandRegMsg[1024]   = {{0}};   //举手
ULANE_SendMsg           iSendMsg[1024]       = {{0}};   //发送消息
ULANE_AgentLogon        iLogon[1024]         = {{0}};   //登录

//初始化服务端的信息
int Ulane_ServerInfoInit(UlaneServerInfo* iServerInfo) 
{
	char              *uSerId;
	char              *uSerIP;
	char              *uSerPort;
	int               iUserPort;
	char              *uDBHost;
	char              *uDBUser;
	char              *uDBPwd;
	char              *uDBName;
	char              *uDBPort;
	int               iUDBPort;
	char              *uDBUnixSock;
	char              *uDBClientFlag;
	int               iUDBClientFlag; 
	//ULANESQL          uSql;
	
	uSerId = ReadConItem("iSerId");
	if(uSerId == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uSerId is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iSerId, uSerId); 
	
	uSerIP = ReadConItem("serverIP");
	if(uSerIP == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uSerIP is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iSerIP, uSerIP);
	
	uSerPort = ReadConItem("ServerPort");
	if(uSerPort == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uSerPort is NULL");
		return SERVINITERR;	
	}
	iUserPort = atoi(uSerPort);
	iServerInfo->iSerPort  = iUserPort;
	
	//数据库部分
	uDBHost = ReadConItem("iDBHost");
	if(uDBHost == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBHost is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iDBHost, uDBHost); 
	
	uDBUser = ReadConItem("iDBUser");
	if(uDBUser == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBUser is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iDBUser, uDBUser);  

	uDBPwd = ReadConItem("iDBPasswd");
	if(uDBPwd == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBPwd is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iDBPasswd, uDBPwd);  
	
	uDBName = ReadConItem("iDBName");
	if(uDBName == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBName is NULL");
		return SERVINITERR;	
	}
	strcpy(iServerInfo->iDBName, uDBName);  
	
	uDBPort = ReadConItem("iDBPort");
	if(uDBPort == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBPort is NULL");
		return SERVINITERR;	
	}
	iUDBPort = atoi(uDBPort);
	iServerInfo->iDBPort = iUDBPort;
	
	uDBUnixSock = ReadConItem("iDBUnixSock");
	if(uDBUnixSock == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBUnixSock is NULL");
		strcpy(iServerInfo->iDBUnixSock, uDBUnixSock);
	}
	strcpy(iServerInfo->iDBUnixSock, uDBUnixSock);
	  
	uDBClientFlag = ReadConItem("iDBClientFlag");
	if(uDBClientFlag == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], readConItemERR,"uDBClientFlag is NULL");
		return SERVINITERR;	
	}
	iUDBClientFlag = atoi(uDBClientFlag);
	iServerInfo->iDBClientFlag = iUDBClientFlag;
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[1], readConItemERR,"System init ok");
	return 0;	
}

//数据库初始化
ULANESQL Ulane_ServerDBInit(ULANESTR iHost, ULANESTR iUser, ULANESTR iPasswd, ULANESTR iDbName, ULANEUNSIGNINT iPort, ULANESTR iUnixSock, ULANELONG iClientFlag)
{
	ULANESQL          uSql;
	ULANESQL          iUsql;
	uSql = UlaneDB_init(NULL);
	if (uSql == NULL) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], SERVINITERR,"system init of databases init error");
		return NULL;
	}
	
	iUsql = UlaneDB_Connect(uSql, iHost, iUser, iPasswd, iDbName, iPort, iUnixSock, iClientFlag);
	if(iUsql == NULL) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], SERVINITERR,"system init of databases init connect error");
		return NULL;
	}
	return 	iUsql;
}

//等待子进程结束并处理善后工作
void sigint(int signo)
{								
  int       i;
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], SERVINITERR,"Server is closing");
  for (i = 0; i < MsgData.maxfd; i++)
  {
  	close(i);
  }   
  exit(0);
}

 //把消息发送给所有客户端，除了iSockfd客户端, 群发
static int MsgSendToAll(int iSockfd, char *Msg, int Msglen)
{
	int         iTimeout   =   3;             //超时时间处理
	int         iRet;
  int         index;
  for (index = 0; index <= MsgData.maxfd ; index++)
  {				
    if (MsgData.clients_info[index].isEmpty == 0 || index == iSockfd)
    {
  	  continue;
    } 
    iRet =  sckServer_send(index, iTimeout, Msg, strlen(Msg));
	  if (iRet == Sck_ErrPeerClosed)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
	    break;
	  }
	  else if (iRet == Sck_ErrTimeOut)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
		  continue;
	  }
	  else if (iRet != 0)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
		  break;
	  } 
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"Msg = %s\n", Msg);  
  }
  return 0;
}

//把消息发送给所有监控坐席
int MsgSendToAgentTypeOne(int iSockfd, char *Msg, int Msglen)
{
	int         iTimeout   =   3;             //超时时间处理
	int         iRet;
  int         index;
  for(index = 0; index <= MsgData.maxfd ; index++)
  {				
    if(MsgData.clients_info[index].isEmpty == 0 || index == iSockfd || MsgData.clients_info[index].flag == 2 || MsgData.clients_info[index].flag == 3)
    {
  	  continue;
    } 
    iRet =  sckServer_send(index, iTimeout, Msg, strlen(Msg));
	  if (iRet == Sck_ErrPeerClosed)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
	    break;
	  }
	  else if (iRet == Sck_ErrTimeOut)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
		  continue;
	  }
	  else if (iRet != 0)
	  {
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
		  break;
	  } 
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"Msg = %s\n", Msg);  
  }
  return 0;
}

//把消息转发给普通坐席
int MsgSendToAgentTypeTwo(int iSockfd, char *Msg, int Msglen, char *iAgentId)
{
	int         iTimeout   =   3;             //超时时间处理
	int         iRet;
  int         index;
  if(iAgentId == NULL)
  {
  	return -1;
  }
  for(index = 0; index <= MsgData.maxfd ; index++)
  {				
    if(MsgData.clients_info[index].isEmpty == 0 || index == iSockfd || MsgData.clients_info[index].flag == 1 || MsgData.clients_info[index].flag == 3 || MsgData.clients_info[index].agentId == NULL)
    {
  	  continue;
    } 
    if(strcmp(iAgentId, MsgData.clients_info[index].agentId) == 0)
    {
  	  iRet =  sckServer_send(index, iTimeout, Msg, strlen(Msg));
		  if (iRet == Sck_ErrPeerClosed)
		  {
			  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
		    break;
		  }
		  else if (iRet == Sck_ErrTimeOut)
		  {
			  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
			  continue;
		  }
		  else if (iRet != 0)
		  {
			  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
			  break;
		  } 
		  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"Msg = %s\n", Msg); 
    } 
  }
  return 0;
}

//举手消息处理
int HandleRaiseHandMsg(int iSockfd, char *inJson)
{
	int                     iRet                 = 0;
	int                     iTimeout;
	char                    *sAgentId;
  char*                   iAnswerJson;
  char*                   outData;
  
  iRet  = RaiseHandReqJsonParse(inJson, iRHandRegMsg);
	if(iRet == -1)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"RaiseHand Json DeCode Error\n");
  	outData = CreateAnswerJson(iRHandRegMsg[0].iCmdName, "Fail", "Msg Error");
  	if(outData == NULL)
	  {
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
	  	return -1;
	  }
	  return -1;
  }  
	outData = CreateAnswerJson(iRHandRegMsg[0].iCmdName, "Ok", "Success");
	if(outData == NULL)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
  	return -1;
  }  
  iRet =  sckServer_send(iSockfd, iTimeout, outData, strlen(outData));
  if (iRet == Sck_ErrPeerClosed)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
    return -1;
  }
  else if (iRet == Sck_ErrTimeOut)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
	  return -1;
  }
  else if (iRet != 0)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
	  return -1;
  } 
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"outData = %s", outData);
  
 	iAnswerJson = CreateRaiseHandMsg(inJson);
 	if(iAnswerJson == NULL)
 	{
 		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"CreateRaiseHandMsg Is Error");
 		return -1;
 	} 
 	//举手处理，发给所有的客户端
	iRet = MsgSendToAll(iSockfd, iAnswerJson, strlen(iAnswerJson));
	if(iRet = 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"Success Send Raise Hand Message%s\n", iAnswerJson);	
	}
	return 0;
}

//消息处理中心--对已连接的客户端的数据收发处理
void HandleTransMsg(int iSockfd, char *Msg, int Msglen)
{	
	int         index;							
	int         iTimeout   =   3;             //超时时间处理
	int         iRet;
	char*       iEventType;                   //事件类型 
	char*       outExtJson;  
	char        *cmdType;                     //cmd的事件类型 
	char        *RaiseHandMsg;                //举手请求
	char        *SendMsg;                     //消息发送
	int         msgId;
	char        *outData;
	char*       iLoginData;
	char        *Login_outData;               //登录应答数据
	int         Login_outLenght;
	
	printf("Msg = %s\n", Msg);
	
	//获取消息队列ID
	msgId = GetMsgQueue();
	if(msgId < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], 100,"CreatMsgQueue Error");
		return ;
	}
	
	cmdType  = CmdNameParse(Msg);
	if(cmdType == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], PARSEJSONERR,"cmdType Is NULL");
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], PARSEJSONERR,"%s\n", Msg);
		
		//处理消息分发和把消息推到消息队列
		iEventType = EveNameParse(Msg);
		if(iEventType == NULL)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], PARSEJSONERR,"iEventType Is NULL");
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], PARSEJSONERR,"%s\n", Msg);	
			return ;
		}
		if(strcmp(iEventType, "stationstatelog")==0)
		{
			//分机状态明分发  1
			iRet = MsgSendToAgentTypeOne(iSockfd, Msg, Msglen);
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			}
			//分机状态明细推到消息队列
			SendQueueMsg(msgId,mainThread_Type,Msg);
		}
		else if(strcmp(iEventType, "agentsigninlog") == 0 || strcmp(iEventType, "agentsignoutlog") == 0)
		{
			//签入签出明细分发  2
			iRet = MsgSendToAgentTypeOne(iSockfd, Msg, Msglen);
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			}
			//签入签出明细推到消息队列
			SendQueueMsg(msgId,mainThread_Type,Msg);                     
		}
		else if(strcmp(iEventType, "vdncalllog") == 0)
		{
			//呼入通话明细分发  3
			iRet = MsgSendToAgentTypeOne(iSockfd, Msg, Msglen);
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			}
			//呼入通话明细推到消息队列
			SendQueueMsg(msgId,mainThread_Type,Msg);                             
		}
		else if(strcmp(iEventType, "stationcalllog") == 0)  
		{ 
			//呼出通话明细分发  4 
			iRet = MsgSendToAgentTypeOne(iSockfd, Msg, Msglen);
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			}
			
			//呼出通话明细推到消息队列
			SendQueueMsg(msgId,mainThread_Type,Msg);                          
		}
		else if(strcmp(iEventType, "agentstatelog") == 0)  
		{
			//坐席实时状态分发到所有监控坐席  5
			iRet = MsgSendToAgentTypeOne(iSockfd, Msg, Msglen);
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			} 
			
			//坐席实时状态推到消息队列
			SendQueueMsg(msgId,mainThread_Type,Msg); 
			
			//坐席实时状态分发普通监控坐席
			iRet = MsgSendToAgentTypeTwo(iSockfd, Msg, Msglen, "2001");
			if(iRet == 0)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[2], 200,"%s\n", Msg);	
			}                        
		}
		return ;
	}
	else if(strcmp(cmdType, "agentlogon") == 0)
	{
		Login_outLenght = TerminalLogin(iSockfd, Msg, &Login_outData);
		if(Login_outLenght == -1)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"TerminalLogin error");
			return ;
		} 
		iRet =  sckServer_send(iSockfd, iTimeout, Login_outData, strlen(Login_outData));
		if (iRet == Sck_ErrPeerClosed)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
			return ;
		}
		else if (iRet == Sck_ErrTimeOut)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
			return ;
		}
		else if (iRet != 0)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
			return ;
		}
	}
	else if(strcmp(cmdType, "cmd_raisehand") == 0)
	{	
		HandleRaiseHandMsg(iSockfd, Msg);
		if(iRet == -1)
		{
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"RaiseHandMsg Error");	
		}
	}
	else if(strcmp(cmdType, "cmd_sendmessage") == 0)
	{
		HandleSendMsg(iSockfd, Msg);
	}
	else
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Error");
		return ;
	}
} 

//处理消息发送接口
int HandleSendMsg(int iSockfd, char *inJson)
{
	int                     iRet         = 0;
	int                     outLen;	
	int                     index;
	int                     iTimeout;
	char                    *sAgentId;
	int                     iTem          = 0;
	msgAgentId              toAgentId[1024]      = {{0}};
  int                     i;
  char*                   iAnswerJson;
  char*                   outData;
	iRet  = SendMsgJsonParse(inJson, iSendMsg);
	if(iRet == -1)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"RaiseHand Json DeCode Error\n");
  	outData = CreateAnswerJson(iLogon[0].iCmdName, "Fail", "500 Do not reciver client raisehand json");
  	if(outData == NULL)
	  {
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
	  	return -1;
	  }
	  return -1;
  }  
	outData = CreateAnswerJson(iLogon[0].iCmdName, "Ok", "Success");
	if(outData == NULL)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
  	return -1;
  }  
  iRet =  sckServer_send(iSockfd, iTimeout, outData, strlen(outData));
  if (iRet == Sck_ErrPeerClosed)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
    return -1;
  }
  else if (iRet == Sck_ErrTimeOut)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
	  return -1;
  }
  else if (iRet != 0)
  {
	  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
	  return -1;
  } 
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"outData = %s", outData);
  
 	iAnswerJson = CreateSendMessage(inJson);
 	if(iAnswerJson == NULL)
 	{
 		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"CreateSendMessage Is Error");
 		return -1;
 	} 
  sAgentId = strtok(iSendMsg[0].iToAgentId, "|");
 	while(sAgentId != NULL)
 	{	
 		memcpy(toAgentId[iTem].iToAgentId, sAgentId, strlen(sAgentId));  
 		sAgentId = strtok(NULL, "|");
 		iTem++;
 	}
 	printf("iTem = %d\n", iTem);
 	for(index = 0; index <= MsgData.maxfd; index++)
  {				
    if (MsgData.clients_info[index].isEmpty == 0 || index == iSockfd || MsgData.clients_info[index].agentId == NULL)
    {
  	  continue;
    }
    for(i = 0; i < iTem; i++)
    {
    	if(toAgentId[i].iToAgentId == NULL)
    	{
    		 continue;	
    	}
    	printf("toAgentId[%d].iToAgentId = %s\n", i, toAgentId[i].iToAgentId);
    	if(strcmp(MsgData.clients_info[index].agentId, toAgentId[i].iToAgentId) == 0)
    	{
    		iRet =  sckServer_send(index, iTimeout, iAnswerJson, strlen(iAnswerJson));
			  if (iRet == Sck_ErrPeerClosed)
			  {
				  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server examination one of client close");
			    break;
			  }
			  else if (iRet == Sck_ErrTimeOut)
			  {
				  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Server TimeOut");
				  continue;
			  }
			  else if (iRet != 0)
			  {
				  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"sckServer_send() err");
				  break;
			  } 
			  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], index,"iAnswerJson = %s\n", iAnswerJson); 
    	}
    	else
    	{
    		continue;
    	}
    }   
  }
  return 0;	
}

//获取Unix下的当前时间
char *Timer()
{
	time_t ctime = time(0);
	struct tm tim = *localtime(&ctime);
	int day = tim.tm_mday;
	int month = tim.tm_mon + 1;
	int year = tim.tm_year + 1900;
	int second = tim.tm_sec;
	int minute = tim.tm_min;
	int hour = tim.tm_hour;
	sprintf(retime, "%02d%02d%02d", hour, minute, second);
	return retime;
}

//终端登录统一处理
int TerminalLogin(int iSockfd, char *inJson, char** outJson)
{
	int                     iRet   = 0;
	char                    *outData = NULL;
	int                     outLen;
  iRet = LogoJsonDecode(inJson, iLogon);
  if(iRet == -1)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Json DeCode Error\n");
  	outData = CreateAnswerJson(iLogon[0].iCmdName, "Fail", "500 Do not reciver client login json");
  	if(outData == NULL)
	  {
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
	  	return -1;
	  }
	  *outJson = outData;
	  outLen  = strlen(outData);
  	return outLen;
  }
  MsgData.clients_info[iSockfd].agentId = iLogon[0].iAgentid;
  if(strcmp(iLogon[0].iAgenttype, "agenttype_1") == 0)
  {
  	MsgData.clients_info[iSockfd].flag = 1;
  }
  if(strcmp(iLogon[0].iAgenttype, "agenttype_2") == 0)
  {
  	MsgData.clients_info[iSockfd].flag = 2;
  } 
  if(strcmp(iLogon[0].iAgenttype, "agenttype_3") == 0)
  {
  	MsgData.clients_info[iSockfd].flag = 3;
  }
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"MsgData.clients_info[iSockfd].agentId=%s\n", MsgData.clients_info[iSockfd].agentId);
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"cmdName=%s,msgID=%s,agentid=%s,agenttype=%s\n",iLogon[0].iCmdName,iLogon[0].iMsgID,iLogon[0].iAgentid,iLogon[0].iAgenttype);	
  outData = CreateAnswerJson(iLogon[0].iCmdName, "Ok", "Successfull!");
  if(outData == NULL)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Create Answer Json Error\n");
  	return -1;
  }
  *outJson = outData;
  outLen = strlen(outData);
  return outLen;
}

//分机状态明细入库处理
int StationStateLog(char *inJson, char** outJson)
{
	int                iRet          =   0;
	int                jsonLength    =   0;               //json内数组的长度
	int                i             =   0;
	ULANESTMT          sqlStmt;   
	ULANEINT           iParamCount;
	char               iStrData[STRING_SIZE];
	ULANEBINDSTR       iBind[100];
	my_ulonglong       iAffectedRows;
	
	uuid_t             iUuid;
  char               uuidStr[36];
	
	char               ID[STRING_SIZE];
	char               AgentID[STRING_SIZE];
	char               Station[STRING_SIZE];
	char						   From_STR[STRING_SIZE]; 
	char						   State[STRING_SIZE];    
	char						   AUX_Code[STRING_SIZE]; 
	char						   Begintime[STRING_SIZE];
	char						   Endtime[STRING_SIZE];  
	char						   StateInfo[STRING_SIZE];
	char						   CustPhone[STRING_SIZE];
	char						   CallDirect[STRING_SIZE];
	char						   Split[STRING_SIZE];
		            
	char                BeginTimeIsNull;
	char                EndTimeIsNull;
	
	ULANELONG           IDLength;
  ULANELONG           AgentIDLength;
  ULANELONG           StationLength;
  ULANELONG           From_STRLength;
  ULANELONG           StateLength;
  ULANELONG           AUX_CodeLength;
  ULANELONG           BegintimeLength;
  ULANELONG           EndtimeLength;
  ULANELONG           StateInfoLength;
  ULANELONG           CustPhoneLength;
  ULANELONG           CallDirectLength;
  ULANELONG           SplitLength;
 
	//获取json串中的内数组的长度
	jsonLength = GetJsonArrayInnerLen(inJson);
	if(jsonLength == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//json串解析
	iRet = ExtJsonDecode(inJson, iStateLog);
	if(iRet == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//初始化预处理环境句柄
  sqlStmt = ULANE_StmtInit(iSql); 
	if(sqlStmt == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"ULANE_StmtInit(), out of memory");
	  return -1;
	}
	
	//向预处理环境句柄添加带(???)的sql语句
	if(ULANE_StmtPrepare(sqlStmt, INSERT_StationStateLog, strlen(INSERT_StationStateLog))) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtPrepare(), INSERT failed");
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "prepare INSERT successful");

	//辅助性函数获取站位付个数
	iParamCount = ULANE_StmtParamCount(sqlStmt);		
	if (iParamCount != 12)
	{ 	
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "invalid parameter count returned by DB");					
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "total parameters in INSERT: %d\n", iParamCount);
	
  memset(iBind, 0, sizeof(iBind));
  //1
  iBind[0].buffer_type= MYSQL_TYPE_STRING;
	iBind[0].buffer= (char *)&ID;			     
	iBind[0].is_null= 0;  
	iBind[0].length= &IDLength;	
	     
	//2                    
	iBind[1].buffer_type= MYSQL_TYPE_STRING;
	iBind[1].buffer= (char *)AgentID;			     
	iBind[1].buffer_length= STRING_SIZE;
	iBind[1].is_null= 0;
	iBind[1].length= &AgentIDLength;				         
	 
	//3
	iBind[2].buffer_type= MYSQL_TYPE_STRING;
	iBind[2].buffer= (char *)&Station;		     
	iBind[2].is_null= 0;			 		         
	iBind[2].length= &StationLength;
	
	//4
	iBind[3].buffer_type= MYSQL_TYPE_STRING;
	iBind[3].buffer= (char *)&From_STR;		     
	iBind[3].is_null= 0;			 		         
	iBind[3].length= &From_STRLength;    
        
	//5
	iBind[4].buffer_type= MYSQL_TYPE_STRING;
	iBind[4].buffer= (char *)&State;		     
	iBind[4].is_null= 0;			 		         
	iBind[4].length= &StateLength;
	
	//6
	iBind[5].buffer_type= MYSQL_TYPE_STRING;
	iBind[5].buffer= (char *)&AUX_Code;		     
	iBind[5].is_null= 0;			 		         
	iBind[5].length= &AUX_CodeLength;  

	//7
	iBind[6].buffer_type= MYSQL_TYPE_STRING;
	iBind[6].buffer= (char *)&Begintime;		     
	iBind[6].is_null= &BeginTimeIsNull;			 		         
	iBind[6].length= &BegintimeLength;
	
	//8
	iBind[7].buffer_type= MYSQL_TYPE_STRING;
	iBind[7].buffer= (char *)&Endtime;		     
	iBind[7].is_null= &EndTimeIsNull;			 		         
	iBind[7].length= &EndtimeLength;
	 
	//9
	iBind[8].buffer_type= MYSQL_TYPE_STRING;
	iBind[8].buffer= (char *)&StateInfo;		     
	iBind[8].is_null= 0;			 		         
	iBind[8].length= &StateInfoLength;
	
	//10
	iBind[9].buffer_type= MYSQL_TYPE_STRING;
	iBind[9].buffer= (char *)&CustPhone;		     
	iBind[9].is_null= 0;			 		         
	iBind[9].length= &CustPhoneLength;
	
	//11
	iBind[10].buffer_type= MYSQL_TYPE_STRING;
	iBind[10].buffer= (char *)&CallDirect;		     
	iBind[10].is_null= 0;			 		         
	iBind[10].length= &CallDirectLength;
	
	//12
	iBind[11].buffer_type= MYSQL_TYPE_STRING;
	iBind[11].buffer= (char *)&Split;		     
	iBind[11].is_null= 0;			 		         
	iBind[11].length= &SplitLength;
	 
	//把赋值以后的数组 添加到预处理环境句柄
	if (ULANE_StmtBindParam(sqlStmt,iBind))
	{ 
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "ULANE_StmtBindParam() failed");		
	  return -1;
	}
	
	for(i = 0; i < jsonLength; i++)
	{
		//产生uuid的函数
	  uuid_generate(iUuid);
	  uuid_unparse(iUuid, uuidStr);
		
		//1
		strncpy(ID, uuidStr, STRING_SIZE); 					
		IDLength= strlen(ID);	
		    
		//2        							          
		strncpy(AgentID, iStateLog[i].iAgentID, STRING_SIZE); 					
		AgentIDLength= strlen(AgentID);
		
		//3	
		strncpy(Station, iStateLog[i].iStation, STRING_SIZE); 					
		StationLength= strlen(Station);							       
		
		//4
		strncpy(From_STR, iStateLog[i].iFrom_STR, STRING_SIZE); 					
		From_STRLength= strlen(From_STR);							     
	 
		//5
		strncpy(State, iStateLog[i].iState, STRING_SIZE); 					
		StateLength= strlen(State);							     
		
		//6
		strncpy(AUX_Code, iStateLog[i].iAUX_Code, STRING_SIZE); 					
		AUX_CodeLength= strlen(AUX_Code);							     
		
		//7--iBegintime					     
		BeginTimeIsNull = 1;	
		
		//8--iEndtime				     
		EndTimeIsNull= 1;	
		  
		//9
		strncpy(StateInfo, iStateLog[i].iStateInfo, STRING_SIZE); 					
		StateInfoLength= strlen(StateInfo);							      

		//10
		strncpy(CustPhone, iStateLog[i].iCustPhone, STRING_SIZE); 					
		CustPhoneLength= strlen(CustPhone);							     
		
		//11--CallDirect[STRING_SIZE]; 
		strncpy(CallDirect, iStateLog[i].iCallDirect, STRING_SIZE); 					
		CallDirectLength= strlen(CallDirect);							     
		
	  //12--Split[STRING_SIZE]; 
		strncpy(Split, iStateLog[i].iSplit, STRING_SIZE); 					
		SplitLength= strlen(Split);	
		
		//执行已经完全填好值的SQL语句 
		if (ULANE_StmtExecute(sqlStmt)) 
		{ 
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtExecute(), 1 failed");			
		  return -1;
		}		
						     	                            
		iAffectedRows= mysql_stmt_affected_rows(sqlStmt);
		if (iAffectedRows != 1)
	  { 
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "invalid affected rows by databASE");	
		  return -1;
		}
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "total affected rows(insert 1): %lu\n",(unsigned long)iAffectedRows);	
	}
	
	if(ULANE_StmtClose(sqlStmt))	
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "failed while closing the statement");	
	  return -1;
	}	
	*outJson = inJson;
}

//签入签出明细入库处理
int AgentSignInLog(char *inJson, char** outJson)
{
	int                iRet          =   0;
	int                jsonLength    =   0;               //json内数组的长度
	int                i             =   0;
	ULANESTMT          sqlStmt;   
	ULANEINT           iParamCount;
	char               iStrData[STRING_SIZE];
	ULANEBINDSTR       iBind[100];
	my_ulonglong       iAffectedRows;
	
	uuid_t             iUuid;
  char               uuidStr[36];
 
  char               ID[STRING_SIZE];
	char               AgentLogonID[STRING_SIZE];
	char						   AgentID[STRING_SIZE]; 
	char						   Station[STRING_SIZE];    
	char						   AgentSkill[STRING_SIZE];
	char						   OpState[STRING_SIZE]; 
	char						   CDTime[STRING_SIZE];
	char               From_STR[STRING_SIZE];
	
	char               CDTimeIsNull;
	char               AgentSkillIsNull;
	
	ULANELONG          IDLength;
  ULANELONG          AgentLogonIDLength;
  ULANELONG          AgentIDLength;
  ULANELONG          StationLength;
  ULANELONG          AgentSkillLength;
  ULANELONG          OpStateLength;
  ULANELONG          CDTimeLength;
  ULANELONG          From_STRLength;
  
  //获取json串中的内数组的长度
	jsonLength = GetJsonArrayInnerLen(inJson);
	if(jsonLength == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//json串解析
	iRet = SignInOutJsonDecode(inJson, iSignInOutLog);
	if(iRet == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//初始化预处理环境句柄
  sqlStmt = ULANE_StmtInit(iSql); 
	if(sqlStmt == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"ULANE_StmtInit(), out of memory");
	  return -1;
	}
	
	//向预处理环境句柄添加带(???)的sql语句
	if(ULANE_StmtPrepare(sqlStmt, INSERT_AgentSignOutInLog, strlen(INSERT_AgentSignOutInLog))) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtPrepare(), INSERT failed");
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "prepare INSERT successful");

	//辅助性函数获取站位付个数
	iParamCount = ULANE_StmtParamCount(sqlStmt);		
	if (iParamCount != 8)
	{ 	
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "invalid parameter count returned by DB");					
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "total parameters in INSERT: %d\n", iParamCount);
	   
	 memset(iBind, 0, sizeof(iBind));
  //1
  iBind[0].buffer_type= MYSQL_TYPE_STRING;
	iBind[0].buffer= (char *)&ID;		
	iBind[0].buffer_length= STRING_SIZE;	     
	iBind[0].is_null= 0;  
	iBind[0].length= &IDLength;	
	     
	//2                    
	iBind[1].buffer_type= MYSQL_TYPE_STRING;
	iBind[1].buffer= (char *)AgentLogonID;			     
	iBind[1].buffer_length= STRING_SIZE;
	iBind[1].is_null= 0;
	iBind[1].length= &AgentLogonIDLength;		
		 	
	//3
	iBind[2].buffer_type= MYSQL_TYPE_STRING;
	iBind[2].buffer= (char *)&AgentID;	
	iBind[2].buffer_length= STRING_SIZE;	     
	iBind[2].is_null= 0;			 		         
	iBind[2].length= &AgentIDLength;    
                 
	//4
	iBind[3].buffer_type= MYSQL_TYPE_STRING;
	iBind[3].buffer= (char *)&Station;
	iBind[3].buffer_length= STRING_SIZE;		     
	iBind[3].is_null= 0;			 		         
	iBind[3].length= &StationLength;
	
	//5
	iBind[4].buffer_type= MYSQL_TYPE_STRING;
	iBind[4].buffer= (char *)&AgentSkill;	
	iBind[4].buffer_length= STRING_SIZE;	     
	iBind[4].is_null= &AgentSkillIsNull;			 		         
	iBind[4].length= &AgentSkillLength;

	//6
	iBind[5].buffer_type= MYSQL_TYPE_STRING;
	iBind[5].buffer= (char *)&OpState;
	iBind[5].buffer_length= STRING_SIZE;		     
	iBind[5].is_null= 0;			 		         
	iBind[5].length= &OpStateLength;  
	
	//7
	iBind[6].buffer_type= MYSQL_TYPE_STRING;
	iBind[6].buffer= (char *)&CDTime;
	iBind[6].buffer_length= STRING_SIZE;		     
	iBind[6].is_null= &CDTimeIsNull;			 		         
	iBind[6].length= &CDTimeLength;
	
	//8
	iBind[7].buffer_type= MYSQL_TYPE_STRING;
	iBind[7].buffer= (char *)&From_STR;	
	iBind[7].buffer_length= STRING_SIZE;	     
	iBind[7].is_null= 0;			 		         
	iBind[7].length= &From_STRLength;
	
	//把赋值以后的数组 添加到预处理环境句柄
	if (ULANE_StmtBindParam(sqlStmt,iBind))
	{ 
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "ULANE_StmtBindParam() failed");		
	  return -1;
	}
	
	for(i = 0; i < jsonLength; i++)
	{
		//产生uuid的函数
	  uuid_generate(iUuid);
	  uuid_unparse(iUuid, uuidStr);
		
		//1
		strncpy(ID, uuidStr, STRING_SIZE); 					
		IDLength= strlen(ID);	
      
		//2        							          
		strncpy(AgentLogonID, iSignInOutLog[i].iAgentLogonID, STRING_SIZE); 					
		AgentLogonIDLength= strlen(AgentLogonID);
		
		//3	
		strncpy(AgentID, iSignInOutLog[i].iAgentID, STRING_SIZE); 					
		AgentIDLength= strlen(AgentID);							       
		   
		//4
		strncpy(Station, iSignInOutLog[i].iStation, STRING_SIZE); 					
		StationLength= strlen(Station);	
		
		//5
		strncpy(AgentSkill, iSignInOutLog[i].iAgentSkill, STRING_SIZE); 					
		AgentSkillLength= strlen(AgentSkill);	
		AgentSkillIsNull = 0;						     
		
		//6
		strncpy(OpState, iSignInOutLog[i].iOpState, STRING_SIZE); 					
		OpStateLength= strlen(OpState);							     
		
		//7			     
		strncpy(CDTime, iSignInOutLog[i].iCDTime, STRING_SIZE); 					
		CDTimeLength= strlen(CDTime);	
		CDTimeIsNull = 0;
		
		//8			     
		strncpy(From_STR, iSignInOutLog[i].iFrom_STR, STRING_SIZE); 					
		From_STRLength= strlen(From_STR);	
		
		//执行已经完全填好值的SQL语句 
		if (ULANE_StmtExecute(sqlStmt)) 
		{ 
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtExecute(), 1 failed");			
		  return -1;
		}		
						     	                            
		iAffectedRows= mysql_stmt_affected_rows(sqlStmt);
		if (iAffectedRows != 1)
	  { 
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "invalid affected rows by databASE");	
		  return -1;
		}
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "total affected rows(insert 1): %lu\n",(unsigned long)iAffectedRows);	
	}
	
	if(ULANE_StmtClose(sqlStmt))	
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "failed while closing the statement");	
	  return -1;
	}	
	*outJson = inJson;
}

//呼入通话明细入库处理
int VdnCallLog(char *inJson, char** outJson)
{
	int                iRet          =   0;
	int                jsonLength    =   0;               //json内数组的长度
	int                i             =   0;
	ULANESTMT          sqlStmt;   
	ULANEINT           iParamCount;
	char               iStrData[STRING_SIZE];
	ULANEBINDSTR       iBind[100];
	my_ulonglong       iAffectedRows;
	
	uuid_t             iUuid;
  char               uuidStr[36];
  
  char               ID[STRING_SIZE];
	char               VDN[STRING_SIZE];
	char						   Split[STRING_SIZE]; 
	char						   UCID[STRING_SIZE];    
	char						   CallID[STRING_SIZE];
	char						   From_STR[STRING_SIZE]; 
	char						   ANI[STRING_SIZE];
	char               DNIS[STRING_SIZE];
	char               Enter_Time[STRING_SIZE];
	char               Queue_Time[STRING_SIZE];
	char						   Deliver_Time[STRING_SIZE]; 
	char						   Estab_Time[STRING_SIZE];    
	char						   Station[STRING_SIZE];
	char						   AgentID[STRING_SIZE]; 
	char						   End_Time[STRING_SIZE];
	char               End_Reason[STRING_SIZE];
	int                VDN_Dur;
	int                Queue_Dur;
	int 						   Ring_Dur; 
	int 						   Talk_Dur;    
	int 						   Hold_Count;
	int 						   Hold_Dur; 
	int  						   TransferCount;
	int                ConferenceCount;
	char               CRT_DT[STRING_SIZE];
	     
	char               CRT_DTIsNull;
	     
	ULANELONG          IDLength;
  ULANELONG          VDNLength;
  ULANELONG          SplitLength;
  ULANELONG          UCIDLength;
  ULANELONG          CallIDLength;
  ULANELONG          From_STRLength;
  ULANELONG          ANILength;
  ULANELONG          DNISLength; 
  ULANELONG          Enter_TimeLength;
	ULANELONG					 Queue_TimeLength;
	ULANELONG					 Deliver_TimeLength;
	ULANELONG					 Estab_TimeLength;
	ULANELONG					 StationLength;
	ULANELONG					 AgentIDLength;
	ULANELONG					 End_TimeLength;
	ULANELONG					 End_ReasonLength;
	ULANELONG					 VDN_DurLength;
	ULANELONG					 Queue_DurLength;
	ULANELONG					 Ring_DurLength;
	ULANELONG					 Talk_DurLength;
	ULANELONG					 Hold_CountLength;
	ULANELONG					 Hold_DurLength;
	ULANELONG					 TransferCountLength;
	ULANELONG					 ConferenceCountLength; 
	ULANELONG					 CRT_DTLength;

  //获取json串中的内数组的长度
	jsonLength = GetJsonArrayInnerLen(inJson);
	if(jsonLength == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//json串解析
	iRet = TelinJsonDecode(inJson, iVDNCallLog);
	if(iRet == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//初始化预处理环境句柄
  sqlStmt = ULANE_StmtInit(iSql); 
	if(sqlStmt == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"ULANE_StmtInit(), out of memory");
	  return -1;
	}
	
	//向预处理环境句柄添加带(???)的sql语句
	if(ULANE_StmtPrepare(sqlStmt, INSERT_VdnCallLog, strlen(INSERT_VdnCallLog))) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtPrepare(), INSERT failed");
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "prepare INSERT successful");

	//辅助性函数获取站位付个数
	iParamCount = ULANE_StmtParamCount(sqlStmt);		
	if (iParamCount != 25)
	{ 	
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "invalid parameter count returned by DB");					
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "total parameters in INSERT: %d\n", iParamCount);
  memset(iBind, 0, sizeof(iBind));
  //1
  iBind[0].buffer_type= MYSQL_TYPE_STRING;
	iBind[0].buffer= (char *)&ID;		
	iBind[0].buffer_length= STRING_SIZE;	     
	iBind[0].is_null= 0;  
	iBind[0].length= &IDLength;	
	     
	//2                    
	iBind[1].buffer_type= MYSQL_TYPE_STRING;
	iBind[1].buffer= (char *)VDN;			     
	iBind[1].buffer_length= STRING_SIZE;
	iBind[1].is_null= 0;
	iBind[1].length= &VDNLength;		
		 	
	//3
	iBind[2].buffer_type= MYSQL_TYPE_STRING;
	iBind[2].buffer= (char *)&CallID;	
	iBind[2].buffer_length= STRING_SIZE;	     
	iBind[2].is_null= 0;			 		         
	iBind[2].length= &CallIDLength;    
                 
	//4
	iBind[3].buffer_type= MYSQL_TYPE_STRING;
	iBind[3].buffer= (char *)&ANI;
	iBind[3].buffer_length= STRING_SIZE;		     
	iBind[3].is_null= 0;			 		         
	iBind[3].length= &ANILength;
	
	//5
	iBind[4].buffer_type= MYSQL_TYPE_STRING;
	iBind[4].buffer= (char *)&DNIS;
	iBind[4].buffer_length= STRING_SIZE;		     
	iBind[4].is_null= 0;			 		         
	iBind[4].length= &DNISLength;  
	
	//6
	iBind[5].buffer_type= MYSQL_TYPE_STRING;
	iBind[5].buffer= (char *)&Enter_Time;
	iBind[5].buffer_length= STRING_SIZE;		     
	iBind[5].is_null= 0;			 		         
	iBind[5].length= &Enter_TimeLength;

	//7
	iBind[6].buffer_type= MYSQL_TYPE_STRING;
	iBind[6].buffer= (char *)&Queue_Time;	
	iBind[6].buffer_length= STRING_SIZE;	     
	iBind[6].is_null= 0;			 		         
	iBind[6].length= &Queue_TimeLength;
	
	 //8
  iBind[7].buffer_type= MYSQL_TYPE_STRING;
	iBind[7].buffer= (char *)&Deliver_Time;		
	iBind[7].buffer_length= STRING_SIZE;	     
	iBind[7].is_null= 0;  
	iBind[7].length= &Deliver_TimeLength;	
	
	//9                    
	iBind[8].buffer_type= MYSQL_TYPE_STRING;
	iBind[8].buffer= (char *)Estab_Time;			     
	iBind[8].buffer_length= STRING_SIZE;
	iBind[8].is_null= 0;
	iBind[8].length= &Estab_TimeLength;		
		 	
	//10
	iBind[9].buffer_type= MYSQL_TYPE_STRING;
	iBind[9].buffer= (char *)&End_Time;	
	iBind[9].buffer_length= STRING_SIZE;	     
	iBind[9].is_null= 0;			 		         
	iBind[9].length= &End_TimeLength;    
     
	//11
	iBind[10].buffer_type= MYSQL_TYPE_STRING;
	iBind[10].buffer= (char *)&Station;
	iBind[10].buffer_length= STRING_SIZE;		     
	iBind[10].is_null= 0;			 		         
	iBind[10].length= &StationLength;
	
	//12
	iBind[11].buffer_type= MYSQL_TYPE_STRING;
	iBind[11].buffer= (char *)&AgentID;	
	iBind[11].buffer_length= STRING_SIZE;	     
	iBind[11].is_null= 0;			 		         
	iBind[11].length= &AgentIDLength;

	//13
	iBind[12].buffer_type= MYSQL_TYPE_STRING;
	iBind[12].buffer= (char *)&Split;
	iBind[12].buffer_length= STRING_SIZE;		     
	iBind[12].is_null= 0;			 		         
	iBind[12].length= &SplitLength;  
   
	//14
	iBind[13].buffer_type= MYSQL_TYPE_STRING;
	iBind[13].buffer= (char *)&End_Reason;
	iBind[13].buffer_length= STRING_SIZE;		     
	iBind[13].is_null= 0;			 		         
	iBind[13].length= &End_ReasonLength;
	
	//15
	iBind[14].buffer_type= MYSQL_TYPE_LONG;
	iBind[14].buffer= (char *)&VDN_Dur;	
	iBind[14].buffer_length= STRING_SIZE;	     
	iBind[14].is_null= 0;			 		         
	iBind[14].length= 0;

	//16
	iBind[15].buffer_type= MYSQL_TYPE_LONG;
	iBind[15].buffer= (char *)&Queue_Dur;	
	iBind[15].buffer_length= STRING_SIZE;	     
	iBind[15].is_null= 0;			 		         
	iBind[15].length= 0;
	
	//17
	iBind[16].buffer_type= MYSQL_TYPE_LONG;
	iBind[16].buffer= (char *)&Ring_Dur;	
	iBind[16].buffer_length= STRING_SIZE;	     
	iBind[16].is_null= 0;			 		         
	iBind[16].length= 0;
	
	//18
	iBind[17].buffer_type= MYSQL_TYPE_LONG;
	iBind[17].buffer= (char *)&Talk_Dur;	
	iBind[17].buffer_length= STRING_SIZE;	     
	iBind[17].is_null= 0;			 		         
	iBind[17].length= 0;
	
	//19
	iBind[18].buffer_type= MYSQL_TYPE_LONG;
	iBind[18].buffer= (char *)&Hold_Count;	
	iBind[18].buffer_length= STRING_SIZE;	     
	iBind[18].is_null= 0;			 		         
	iBind[18].length= 0;
	  
	//20
	iBind[19].buffer_type= MYSQL_TYPE_LONG;
	iBind[19].buffer= (char *)&Hold_Dur;	
	iBind[19].buffer_length= STRING_SIZE;	     
	iBind[19].is_null= 0;			 		         
	iBind[19].length= 0;
	
	//21
	iBind[20].buffer_type= MYSQL_TYPE_LONG;
	iBind[20].buffer= (char *)&TransferCount;	
	iBind[20].buffer_length= STRING_SIZE;	     
	iBind[20].is_null= 0;			 		         
	iBind[20].length= 0;
	
	//22
	iBind[21].buffer_type= MYSQL_TYPE_LONG;
	iBind[21].buffer= (char *)&ConferenceCount;	
	iBind[21].buffer_length= STRING_SIZE;	     
	iBind[21].is_null= 0;			 		         
	iBind[21].length= 0;
	
	//23
	iBind[22].buffer_type= MYSQL_TYPE_STRING;
	iBind[22].buffer= (char *)&CRT_DT;	
	iBind[22].buffer_length= STRING_SIZE;	     
	iBind[22].is_null= &CRT_DTIsNull;			 		         
	iBind[22].length= &CRT_DTLength;
	
	//24
	iBind[23].buffer_type= MYSQL_TYPE_STRING;
	iBind[23].buffer= (char *)&UCID;	
	iBind[23].buffer_length= STRING_SIZE;	     
	iBind[23].is_null= 0;			 		         
	iBind[23].length= &UCIDLength;
	
	//25
	iBind[24].buffer_type= MYSQL_TYPE_STRING;
	iBind[24].buffer= (char *)&From_STR;	
	iBind[24].buffer_length= STRING_SIZE;	     
	iBind[24].is_null= 0;			 		         
	iBind[24].length= &From_STRLength;
	
	//把赋值以后的数组 添加到预处理环境句柄
	if (ULANE_StmtBindParam(sqlStmt,iBind))
	{ 
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "ULANE_StmtBindParam() failed");		
	  return -1;
	}
	
	for(i = 0; i < jsonLength; i++)
	{
		//产生uuid的函数
	  uuid_generate(iUuid);
	  uuid_unparse(iUuid, uuidStr);
		
		//1
		strncpy(ID, uuidStr, STRING_SIZE); 					
		IDLength= strlen(ID);	
      
		//2        							          
		strncpy(VDN, iVDNCallLog[i].iVDN, STRING_SIZE); 					
		VDNLength= strlen(VDN);
		
		//3	
		strncpy(CallID, iVDNCallLog[i].iCallID, STRING_SIZE); 					
		CallIDLength= strlen(CallID);							       
		   
		//4
		strncpy(ANI, iVDNCallLog[i].iANI, STRING_SIZE); 					
		ANILength= strlen(ANI);	
		
		//5
		strncpy(DNIS, iVDNCallLog[i].iDNIS, STRING_SIZE); 					
		DNISLength= strlen(DNIS);						     
		
		//6
		strncpy(Enter_Time, iVDNCallLog[i].iEnter_Time, STRING_SIZE); 					
		Enter_TimeLength= strlen(Enter_Time);							     
		
		//7			     
		strncpy(Queue_Time, iVDNCallLog[i].iQueue_Time, STRING_SIZE); 					
		Queue_TimeLength= strlen(Queue_Time);	
		
		//8			     
		strncpy(Deliver_Time, iVDNCallLog[i].iDeliver_Time, STRING_SIZE); 					
		Deliver_TimeLength= strlen(Deliver_Time);	
		
		//9			     
		strncpy(Estab_Time, iVDNCallLog[i].iEstab_Time, STRING_SIZE); 					
		Estab_TimeLength= strlen(Estab_Time);	
		
		//10		     
		strncpy(End_Time, iVDNCallLog[i].iEnd_Time, STRING_SIZE); 					
		End_TimeLength= strlen(End_Time);	
		
		//11			     
		strncpy(Station, iVDNCallLog[i].iStation, STRING_SIZE); 					
		StationLength= strlen(Station);	
		
		//12		     
		strncpy(AgentID, iVDNCallLog[i].iAgentID, STRING_SIZE); 					
		AgentIDLength= strlen(AgentID);	
		
		//13		     
		strncpy(Split, iVDNCallLog[i].iSplit, STRING_SIZE); 					
		SplitLength= strlen(Split);	
		
		//14			     
		strncpy(End_Reason, iVDNCallLog[i].iEnd_Reason, STRING_SIZE); 					
		End_ReasonLength= strlen(End_Reason);	
		
		//15			     				
		VDN_Dur= iVDNCallLog[i].iVDN_Dur;	
		
		//16			      					
		Queue_Dur= iVDNCallLog[i].iQueue_Dur;	
		
		//17			     					
		Ring_Dur= iVDNCallLog[i].iRing_Dur;	
		
		//18			      					
		Talk_Dur= iVDNCallLog[i].iTalk_Dur;
		
		//19			     					
		Hold_Count= iVDNCallLog[i].iHold_Count;	
		
		//20			     					
		Hold_Dur= iVDNCallLog[i].iHold_Dur;	
		
		//21			     					
		TransferCount= iVDNCallLog[i].iTransferCount;	
		
		//22	
		ConferenceCount = iVDNCallLog[i].iConferenceCount;     
	
		//23		     
		CRT_DTIsNull = 1;
		
		//24		     
		strncpy(UCID, iVDNCallLog[i].iUCID, STRING_SIZE); 					
		UCIDLength= strlen(UCID);	
		
		//25		     
		strncpy(From_STR, iVDNCallLog[i].iFrom_STR, STRING_SIZE); 					
		From_STRLength= strlen(From_STR);	
		
		//执行已经完全填好值的SQL语句
		if (ULANE_StmtExecute(sqlStmt)) 
		{ 
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtExecute(), 1 failed");			
		  return -1;
		}		
						     	                            
		iAffectedRows= mysql_stmt_affected_rows(sqlStmt);
		if (iAffectedRows != 1)
	  { 
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "invalid affected rows by databASE");	
		  return -1;
		}
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "total affected rows(insert 1): %lu\n",(unsigned long)iAffectedRows);	
	}
	
	if(ULANE_StmtClose(sqlStmt))	
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "failed while closing the statement");	
	  return -1;
	}	
	*outJson = inJson;
}

//呼出通话明细入库处理
int StationCallLog(char *inJson, char** outJson)
{
	int                iRet          =   0;
	int                jsonLength    =   0;               //json内数组的长度
	int                i             =   0;
	ULANESTMT          sqlStmt;   
	ULANEINT           iParamCount;
	char               iStrData[STRING_SIZE];
	ULANEBINDSTR       iBind[100];
	my_ulonglong       iAffectedRows;
	
	uuid_t             iUuid;
  char               uuidStr[36];
          
  char               ID[STRING_SIZE];
	char               AgentID[STRING_SIZE];
	char						   Station[STRING_SIZE]; 
	char						   UCID[STRING_SIZE];    
	char						   CallID[STRING_SIZE];
	char						   CallDirect[STRING_SIZE]; 
	char						   ANI[STRING_SIZE];
	char               DNIS[STRING_SIZE];
	char               MakeCall_Time[STRING_SIZE];
	char               Deliver_Time[STRING_SIZE];
	char               Establish_time[STRING_SIZE];
	char						   End_Time[STRING_SIZE]; 	
	char						   End_Reason[STRING_SIZE];    
	char						   VDN[STRING_SIZE];
	char						   Split[STRING_SIZE]; 
	int						     Talk_Dur;
	int                Ring_Dur;
	int                Hold_Dur;
	int                HoldCount;
	int 						   ConsultCount; 
	int 						   ConferenceCount;          
	char 						   Next_UCID[STRING_SIZE];
	char 						   Next_CallID[STRING_SIZE]; 
	char  						 From_STR[STRING_SIZE];
	char               CRT_DT[STRING_SIZE];
	     
	char               CRT_DTIsNull;
	char               MakeCall_TimeIsNull;
	char               Establish_timeIsNull;
	char               Deliver_TimeIsNull;
	char               End_TimeIsNull;
	       
	ULANELONG          IDLength;
  ULANELONG          AgentIDLength;
  ULANELONG          StationLength;
  ULANELONG          UCIDLength;
  ULANELONG          CallIDLength;
  ULANELONG          CallDirectLength;
  ULANELONG          ANILength;
  ULANELONG          DNISLength; 
  ULANELONG          MakeCall_TimeLength;
  ULANELONG					 Deliver_TimeLength;
	ULANELONG					 Establish_timeLength;
  ULANELONG          End_TimeLength;
	ULANELONG					 End_ReasonLength;
	ULANELONG					 VDNLength;
	ULANELONG					 SplitLength;
	ULANELONG					 Talk_DurLength;
	ULANELONG					 Ring_DurLength;
	ULANELONG					 Hold_DurLength;
	ULANELONG					 HoldCountLength;
	ULANELONG					 ConsultCountLength;
	ULANELONG					 ConferenceCountLength;   
	ULANELONG					 Next_UCIDLength;
	ULANELONG					 Next_CallIDLength;
	ULANELONG          From_STRLength;
	ULANELONG					 CRT_DTLength;

  //获取json串中的内数组的长度
	jsonLength = GetJsonArrayInnerLen(inJson);
	if(jsonLength == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//json串解析
	iRet = TeloutExtJsonDecode(inJson, iStationCallLog);
	if(iRet == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//初始化预处理环境句柄
  sqlStmt = ULANE_StmtInit(iSql); 
	if(sqlStmt == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"ULANE_StmtInit(), out of memory");
	  return -1;
	}
	
	//向预处理环境句柄添加带(???)的sql语句
	if(ULANE_StmtPrepare(sqlStmt, INSERT_StationCallLog, strlen(INSERT_StationCallLog))) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtPrepare(), INSERT failed");
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "prepare INSERT successful");

	//辅助性函数获取站位付个数
	iParamCount = ULANE_StmtParamCount(sqlStmt);		
	if (iParamCount != 25)
	{ 	
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "invalid parameter count returned by DB");					
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "total parameters in INSERT: %d\n", iParamCount);
	
	 //1
  iBind[0].buffer_type= MYSQL_TYPE_STRING;
	iBind[0].buffer= (char *)&ID;		
	iBind[0].buffer_length= STRING_SIZE;	     
	iBind[0].is_null= 0;  
	iBind[0].length= &IDLength;	
	     
	//2                    
	iBind[1].buffer_type= MYSQL_TYPE_STRING;
	iBind[1].buffer= (char *)AgentID;			     
	iBind[1].buffer_length= STRING_SIZE;
	iBind[1].is_null= 0;
	iBind[1].length= &AgentIDLength;		
		 	
	//3
	iBind[2].buffer_type= MYSQL_TYPE_STRING;
	iBind[2].buffer= (char *)&Station;	
	iBind[2].buffer_length= STRING_SIZE;	     
	iBind[2].is_null= 0;			 		         
	iBind[2].length= &StationLength;    
                 
	//4
	iBind[3].buffer_type= MYSQL_TYPE_STRING;
	iBind[3].buffer= (char *)&UCID;
	iBind[3].buffer_length= STRING_SIZE;		     
	iBind[3].is_null= 0;			 		         
	iBind[3].length= &UCIDLength;
	
	//5
	iBind[4].buffer_type= MYSQL_TYPE_STRING;
	iBind[4].buffer= (char *)&CallID;
	iBind[4].buffer_length= STRING_SIZE;		     
	iBind[4].is_null= 0;			 		         
	iBind[4].length= &CallIDLength;  
	
	//6
	iBind[5].buffer_type= MYSQL_TYPE_STRING;
	iBind[5].buffer= (char *)&CallDirect;
	iBind[5].buffer_length= STRING_SIZE;		     
	iBind[5].is_null= 0;			 		         
	iBind[5].length= &CallDirectLength;

	//7
	iBind[6].buffer_type= MYSQL_TYPE_STRING;
	iBind[6].buffer= (char *)&ANI;	
	iBind[6].buffer_length= STRING_SIZE;	     
	iBind[6].is_null= 0;			 		         
	iBind[6].length= &ANILength;
	
	 //8
  iBind[7].buffer_type= MYSQL_TYPE_STRING;
	iBind[7].buffer= (char *)&DNIS;		
	iBind[7].buffer_length= STRING_SIZE;	     
	iBind[7].is_null= 0;  
	iBind[7].length= &DNISLength;	
	
	//9                    
	iBind[8].buffer_type= MYSQL_TYPE_STRING;
	iBind[8].buffer= (char *)MakeCall_Time;			     
	iBind[8].buffer_length= STRING_SIZE;
	iBind[8].is_null= &MakeCall_TimeIsNull;
	iBind[8].length= 0;		
		 	
	//10
	iBind[9].buffer_type= MYSQL_TYPE_STRING;
	iBind[9].buffer= (char *)&Deliver_Time;	
	iBind[9].buffer_length= STRING_SIZE;	     
	iBind[9].is_null= &Deliver_TimeIsNull;			 		         
	iBind[9].length= 0;    
     
	//11
	iBind[10].buffer_type= MYSQL_TYPE_STRING;
	iBind[10].buffer= (char *)&Establish_time;
	iBind[10].buffer_length= STRING_SIZE;		     
	iBind[10].is_null= &Establish_timeIsNull;			 		         
	iBind[10].length= 0;
	
	//12
	iBind[11].buffer_type= MYSQL_TYPE_STRING;
	iBind[11].buffer= (char *)&End_Time;	
	iBind[11].buffer_length= STRING_SIZE;	     
	iBind[11].is_null= &End_TimeIsNull;			 		         
	iBind[11].length= 0;

	//13
	iBind[12].buffer_type= MYSQL_TYPE_STRING;
	iBind[12].buffer= (char *)&End_Reason;
	iBind[12].buffer_length= STRING_SIZE;		     
	iBind[12].is_null= 0;			 		         
	iBind[12].length= &End_ReasonLength;  
   
	//14
	iBind[13].buffer_type= MYSQL_TYPE_STRING;
	iBind[13].buffer= (char *)&VDN;
	iBind[13].buffer_length= STRING_SIZE;		     
	iBind[13].is_null= 0;			 		         
	iBind[13].length= &VDNLength;
	
	//15
	iBind[14].buffer_type= MYSQL_TYPE_STRING;
	iBind[14].buffer= (char *)&Split;	
	iBind[14].buffer_length= STRING_SIZE;	     
	iBind[14].is_null= 0;			 		         
	iBind[14].length= &SplitLength;


	//16
	iBind[15].buffer_type= MYSQL_TYPE_LONG;
	iBind[15].buffer= (char *)&Talk_Dur;	
	iBind[15].buffer_length= STRING_SIZE;	     
	iBind[15].is_null= 0;			 		         
	iBind[15].length= 0;
	
	//17
	iBind[16].buffer_type= MYSQL_TYPE_LONG;
	iBind[16].buffer= (char *)&Ring_Dur;	
	iBind[16].buffer_length= STRING_SIZE;	     
	iBind[16].is_null= 0;			 		         
	iBind[16].length= 0;
	
	//18
	iBind[17].buffer_type= MYSQL_TYPE_LONG;
	iBind[17].buffer= (char *)&Hold_Dur;	
	iBind[17].buffer_length= STRING_SIZE;	     
	iBind[17].is_null= 0;			 		         
	iBind[17].length= 0;
	
	//19
	iBind[18].buffer_type= MYSQL_TYPE_LONG;
	iBind[18].buffer= (char *)&HoldCount;	
	iBind[18].buffer_length= STRING_SIZE;	     
	iBind[18].is_null= 0;			 		         
	iBind[18].length= 0;
	  
	//20
	iBind[19].buffer_type= MYSQL_TYPE_LONG;
	iBind[19].buffer= (char *)&ConsultCount;	
	iBind[19].buffer_length= STRING_SIZE;	     
	iBind[19].is_null= 0;			 		         
	iBind[19].length= 0;
	
	//21
	iBind[20].buffer_type= MYSQL_TYPE_LONG;
	iBind[20].buffer= (char *)&ConferenceCount;	
	iBind[20].buffer_length= STRING_SIZE;	     
	iBind[20].is_null= 0;			 		         
	iBind[20].length= 0;
	 
	//22
	iBind[21].buffer_type= MYSQL_TYPE_STRING;
	iBind[21].buffer= (char *)&Next_UCID;	
	iBind[21].buffer_length= STRING_SIZE;	     
	iBind[21].is_null= 0;			 		         
	iBind[21].length= &Next_UCIDLength;
	
	//23
	iBind[22].buffer_type= MYSQL_TYPE_STRING;
	iBind[22].buffer= (char *)&Next_CallID;	
	iBind[22].buffer_length= STRING_SIZE;	     
	iBind[22].is_null= 0;			 		         
	iBind[22].length= &Next_CallIDLength;
	
	//24
	iBind[23].buffer_type= MYSQL_TYPE_STRING;
	iBind[23].buffer= (char *)&From_STR;	
	iBind[23].buffer_length= STRING_SIZE;	     
	iBind[23].is_null= 0;			 		         
	iBind[23].length= &From_STRLength;
	
	//25
	iBind[24].buffer_type= MYSQL_TYPE_STRING;
	iBind[24].buffer= (char *)&CRT_DT;	
	iBind[24].buffer_length= STRING_SIZE;	     
	iBind[24].is_null= &CRT_DTIsNull;			 		         
	iBind[24].length= &CRT_DTLength;
	
	//把赋值以后的数组 添加到预处理环境句柄
	if (ULANE_StmtBindParam(sqlStmt,iBind))
	{ 
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "ULANE_StmtBindParam() failed");		
	  return -1;
	}
	
	for(i = 0; i < jsonLength; i++)
	{
		//产生uuid的函数
	  uuid_generate(iUuid);
	  uuid_unparse(iUuid, uuidStr);
		
		//1
		strncpy(ID, uuidStr, STRING_SIZE); 					
		IDLength= strlen(ID);	
      
		//2        							          
		strncpy(AgentID, iStationCallLog[i].iAgentID, STRING_SIZE); 					
		AgentIDLength= strlen(AgentID);
		
		//3	
		strncpy(Station, iStationCallLog[i].iStation, STRING_SIZE); 					
		StationLength= strlen(Station);							       
		   
		//4
		strncpy(UCID, iStationCallLog[i].iUCID, STRING_SIZE); 					
		UCIDLength= strlen(UCID);	
		
		//5
		strncpy(CallID, iStationCallLog[i].iCallID, STRING_SIZE); 					
		CallIDLength= strlen(CallID);						     
		
		//6
		strncpy(CallDirect, iStationCallLog[i].iCallDirect, STRING_SIZE); 					
		CallDirectLength= strlen(CallDirect);							     
		
		//7			     
		strncpy(ANI, iStationCallLog[i].iANI, STRING_SIZE); 					
		ANILength= strlen(ANI);	
		
		//8			     
		strncpy(DNIS, iStationCallLog[i].iDNIS, STRING_SIZE); 					
		DNISLength= strlen(DNIS);	
		
		//9			     
		MakeCall_TimeIsNull = 1;	
		
		//10		     
		Deliver_TimeIsNull = 1;
		
		//11			     
		Establish_timeIsNull = 1;
		
		//12		     
		End_TimeIsNull = 1;
		
		//13		     
		strncpy(End_Reason, iStationCallLog[i].iEnd_Reason, STRING_SIZE); 					
		End_ReasonLength= strlen(End_Reason);	
		
		//14			     
		strncpy(VDN, iStationCallLog[i].iVDN, STRING_SIZE); 					
		VDNLength= strlen(VDN);	
		
		//15			     				
		strncpy(Split, iStationCallLog[i].iSplit, STRING_SIZE); 					
		SplitLength= strlen(Split);	
		
		//16			      					
		Talk_Dur= iStationCallLog[i].iTalk_Dur;	
		
		//17			     					
		Ring_Dur= iStationCallLog[i].iRing_Dur;	
		
		//18			      					
		Hold_Dur= iStationCallLog[i].iHold_Dur;
		
		//19			     					
		HoldCount= iStationCallLog[i].iHoldCount;	
		
		//20			     					
		ConsultCount= iStationCallLog[i].iConsultCount;	
		
		//21			     					
		ConferenceCount= iStationCallLog[i].iConferenceCount;	
		
		//22	
		strncpy(Next_UCID, iStationCallLog[i].iNext_UCID, STRING_SIZE); 					
		Next_UCIDLength= strlen(Next_UCID);     
	
		//23		     
		strncpy(Next_CallID, iStationCallLog[i].iNext_CallID, STRING_SIZE); 					
		Next_CallIDLength= strlen(Next_CallID);	
		
		//24		     
		strncpy(From_STR, iStationCallLog[i].iFrom_STR, STRING_SIZE); 					
		From_STRLength= strlen(From_STR);	
		
		//25
		CRT_DTIsNull = 1;		     
		
		//执行已经完全填好值的SQL语句
		if (ULANE_StmtExecute(sqlStmt)) 
		{ 
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtExecute(), 1 failed");			
		  return -1;
		}		
						     	                            
		iAffectedRows= mysql_stmt_affected_rows(sqlStmt);
		if (iAffectedRows != 1)
	  { 
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "invalid affected rows by databASE");	
		  return -1;
		}
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "total affected rows(insert 1): %lu\n",(unsigned long)iAffectedRows);	
	}
	
	if(ULANE_StmtClose(sqlStmt))	
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "failed while closing the statement");	
	  return -1;
	}	
	*outJson = inJson;
}

//坐席实时状态删除和入库处理
int AgentStateLog(char *inJson, char** outJson)
{
	int                iRet          =   0;
	int                jsonLength    =   0;               //json内数组的长度
	int                i             =   0;
	ULANESTMT          sqlStmt;   
	ULANEINT           iParamCount;
	char               iStrData[STRING_SIZE];
	ULANEBINDSTR       iBind[100];
	my_ulonglong       iAffectedRows;
	
	uuid_t             iUuid;
  char               uuidStr[36];
 
  char               ID[STRING_SIZE];
	char               AgentID[STRING_SIZE];
	char						   Station[STRING_SIZE]; 
	char						   LogonTime[STRING_SIZE];    
	char						   State[STRING_SIZE];
	char						   AUX_Code[STRING_SIZE]; 
	char						   Begintime[STRING_SIZE];
	char               Pre_aux[STRING_SIZE];
	char               pre_state[STRING_SIZE];
	char               StateInfo[STRING_SIZE];
	char               CustPhone[STRING_SIZE];
	char               CallDirect[STRING_SIZE];
	char               Split[STRING_SIZE];
	char               CRT_DT[STRING_SIZE];
	
	char               LogonTimeIsNull;
	char               BeginTimeIsNull;
	char               CRT_DTIsNull;
	
	ULANELONG          IDLength;
  ULANELONG          AgentIDLength;
  ULANELONG          StationLength;
  ULANELONG          LogonTimeLength;
  ULANELONG          StateLength;
  ULANELONG          AUX_CodeLength;
  ULANELONG          BegintimeLength;
  ULANELONG          Pre_auxLength;
  ULANELONG          pre_stateLength;
  ULANELONG          StateInfoLength;
  ULANELONG          CustPhoneLength;
  ULANELONG          CallDirectLength;
  ULANELONG          SplitLength;
  ULANELONG          CRT_DTLength;
   
  //获取json串中的内数组的长度
	jsonLength = GetJsonArrayInnerLen(inJson);
	if(jsonLength == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//json串解析
	iRet = AgentStateLogJsonDecode(inJson, iAgentStateLog);
	if(iRet == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ExtJsonDecode() err");
		return -1;
	}
	
	//删除上一次插入的数据
	if (UlaneDB_ExecSql(iSql, DELETE_AgentStateLog))		
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"delete data from m_state table failed");
	  return -1;
	}
	
	//初始化预处理环境句柄
  sqlStmt = ULANE_StmtInit(iSql); 
	if(sqlStmt == NULL)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt),"ULANE_StmtInit(), out of memory");
	  return -1;
	}
	
	//向预处理环境句柄添加带(???)的sql语句
	if(ULANE_StmtPrepare(sqlStmt, INSERT_AgentStateLog, strlen(INSERT_AgentStateLog))) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtPrepare(), INSERT failed");
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "prepare INSERT successful");

	//辅助性函数获取站位付个数
	iParamCount = ULANE_StmtParamCount(sqlStmt);		
	if (iParamCount != 14)
	{ 	
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "invalid parameter count returned by DB");					
	  return -1;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "total parameters in INSERT: %d\n", iParamCount);
	
  memset(iBind, 0, sizeof(iBind));
  //1
  iBind[0].buffer_type= MYSQL_TYPE_STRING;
	iBind[0].buffer= (char *)&ID;			     
	iBind[0].is_null= 0;  
	iBind[0].length= &IDLength;	
	     
	//2                    
	iBind[1].buffer_type= MYSQL_TYPE_STRING;
	iBind[1].buffer= (char *)AgentID;			     
	iBind[1].buffer_length= STRING_SIZE;
	iBind[1].is_null= 0;
	iBind[1].length= &AgentIDLength;				         
	 
	//3
	iBind[2].buffer_type= MYSQL_TYPE_STRING;
	iBind[2].buffer= (char *)&Station;		     
	iBind[2].is_null= 0;			 		         
	iBind[2].length= &StationLength;

	//4
	iBind[3].buffer_type= MYSQL_TYPE_STRING;
	iBind[3].buffer= (char *)&LogonTime;		     
	iBind[3].is_null= 0;			 		         
	iBind[3].length= &LogonTimeLength;    
        
	//5
	iBind[4].buffer_type= MYSQL_TYPE_STRING;
	iBind[4].buffer= (char *)&State;		     
	iBind[4].is_null= 0;			 		         
	iBind[4].length= &StateLength;
		      
	//6
	iBind[5].buffer_type= MYSQL_TYPE_STRING;
	iBind[5].buffer= (char *)&AUX_Code;		     
	iBind[5].is_null= 0;			 		         
	iBind[5].length= &AUX_CodeLength;  

	//7
	iBind[6].buffer_type= MYSQL_TYPE_STRING;
	iBind[6].buffer= (char *)&Begintime;		     
	iBind[6].is_null= &BeginTimeIsNull;			 		         
	iBind[6].length= 0;
	    
	//8
	iBind[7].buffer_type= MYSQL_TYPE_STRING;
	iBind[7].buffer= (char *)&Pre_aux;		     
	iBind[7].is_null= 0;			 		         
	iBind[7].length= &Pre_auxLength;
	 
	//9
	iBind[8].buffer_type= MYSQL_TYPE_STRING;
	iBind[8].buffer= (char *)&pre_state;		     
	iBind[8].is_null= 0;			 		         
	iBind[8].length= &pre_stateLength;

	//10
	iBind[9].buffer_type= MYSQL_TYPE_STRING;
	iBind[9].buffer= (char *)&StateInfo;		     
	iBind[9].is_null= 0;			 		         
	iBind[9].length= &StateInfoLength;

	//11
	iBind[10].buffer_type= MYSQL_TYPE_STRING;
	iBind[10].buffer= (char *)&CustPhone;		     
	iBind[10].is_null= 0;			 		         
	iBind[10].length= &CustPhoneLength;
	
	//12
	iBind[11].buffer_type= MYSQL_TYPE_STRING;
	iBind[11].buffer= (char *)&CallDirect;		     
	iBind[11].is_null= 0;			 		         
	iBind[11].length= &CallDirectLength;
	
	//13
	iBind[12].buffer_type= MYSQL_TYPE_STRING;
	iBind[12].buffer= (char *)&Split;		     
	iBind[12].is_null= 0;			 		         
	iBind[12].length= &SplitLength;
	
	//14
	iBind[13].buffer_type= MYSQL_TYPE_STRING;
	iBind[13].buffer= (char *)&CRT_DT;		     
	iBind[13].is_null= &CRT_DTIsNull;			 		         
	iBind[13].length= 0;
	
	//把赋值以后的数组 添加到预处理环境句柄
	if (ULANE_StmtBindParam(sqlStmt,iBind))
	{ 
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "ULANE_StmtBindParam() failed");		
	  return -1;
	}
	
	for(i = 0; i < jsonLength; i++)
	{
		//产生uuid的函数
	  uuid_generate(iUuid);
	  uuid_unparse(iUuid, uuidStr);
		
		//1
		strncpy(ID, uuidStr, STRING_SIZE); 					
		IDLength= strlen(ID);	
		    
		//2        							          
		strncpy(AgentID, iAgentStateLog[i].iAgentID, STRING_SIZE); 					
		AgentIDLength= strlen(AgentID);
		
		//3	
		strncpy(Station, iAgentStateLog[i].iStation, STRING_SIZE); 					
		StationLength= strlen(Station);							       
		
		//4
		strncpy(LogonTime, iAgentStateLog[i].iLogonTime, STRING_SIZE); 					
		LogonTimeLength= strlen(LogonTime);							     
	 
		//5
		strncpy(State, iAgentStateLog[i].iState, STRING_SIZE); 					
		StateLength= strlen(State);							     
		
		//6
		strncpy(AUX_Code, iAgentStateLog[i].iAUX_Code, STRING_SIZE); 					
		AUX_CodeLength= strlen(AUX_Code);							     
		
		//7--iBegintime					     
		BeginTimeIsNull = 1;	
		
	  strncpy(pre_state, iAgentStateLog[i].iPre_state, STRING_SIZE); 					
		pre_stateLength= strlen(pre_state);		
		  
		//9
		strncpy(Pre_aux, iAgentStateLog[i].iPre_aux, STRING_SIZE); 					
		Pre_auxLength= strlen(Pre_aux);							      
		
		//10
		strncpy(StateInfo, iAgentStateLog[i].iStateInfo, STRING_SIZE); 					
		StateInfoLength= strlen(StateInfo);							     
		
		//11
		strncpy(CustPhone, iAgentStateLog[i].iCustPhone, STRING_SIZE); 					
		CustPhoneLength= strlen(CustPhone);	
		
		//12--CallDirect[STRING_SIZE]; 
		strncpy(CallDirect, iAgentStateLog[i].iCallDirect, STRING_SIZE); 					
		CallDirectLength= strlen(CallDirect);							     
		
	  //13--Split[STRING_SIZE]; 
		strncpy(Split, iAgentStateLog[i].iSplit, STRING_SIZE); 					
		SplitLength= strlen(Split);	
		
		//14
		CRT_DTIsNull = 1;
		//执行已经完全填好值的SQL语句 
		if (ULANE_StmtExecute(sqlStmt)) 
		{ 
			Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], ULANE_StmtErrno(sqlStmt), "ULANE_StmtExecute(), 1 failed");			
		  return -1;
		}		
						     	                            
		iAffectedRows= mysql_stmt_affected_rows(sqlStmt);
		if (iAffectedRows != 1)
	  { 
	  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "invalid affected rows by databASE");	
		  return -1;
		}
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], -1, "total affected rows(insert 1): %lu\n",(unsigned long)iAffectedRows);	
	}
	
	if(ULANE_StmtClose(sqlStmt))	
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4],  ULANE_StmtErrno(sqlStmt), "failed while closing the statement");	
	  return -1;
	}	
	*outJson = inJson;
	return 0;	
}

//技能组实时排队信息统一处理
int SkillQueueInfo(char *inJson, char** outJson)
{
	return 0;
}