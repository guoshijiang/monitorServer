/*=========================================================
 *文件名称: 			minitorServer.c
 *创建日期：			2017-6-23
 *作    者:       郭世江
 *修改记录：			2017-6-23  首次创建
 *功能描述：      业务函数主要文件
 *===========================================================*/
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
#include "minitorUlaneServerOp.h"
#include "threadPool.h"
#include "ulaneMysql.h"
#include "ulaneStmtMysql.h"
#include "minitorUlaneServerOp.h"
#include "poolSocket.h"
#include "queueMsg.h"

//宏定义
#define STATIONSTATELOG   1              //分机状态明细
#define VDNCALLLOG        2              //呼入通话明细
#define STATIONCALLLOG    3              //呼出通话明细
#define AGENTSIGNINLOG    4              //签入签出明细
#define AGENTSTATELOG     5              //坐席实时状态
#define SKILLQUEUEINLOG   6              //技能组实时排队信息

//错误码定义
#define DBINITERR         4000
#define GETEVTERR         4001
#define DETACHTHREADERR   4002

//全局变量定义
int                g_serverTag = 0;
//常量定义
char welcome[] = "------Welcome to Our UlaneMonitor Server------\n";

//函数功能：接收/发送客户端消息
void *UlaneServer_Routine(void* arg)
{
  int             iSockfd = *(int *)arg;
  int             outLen = 0;
  fd_set          rSet;
  char            outData[MAXLINE];
	int             iRet;
	int             i;
	char            iLastThreeStr[3];
	
  //分离主线程，以便线程退出时资源立即被收回
  pthread_t tid = pthread_self();
  if (pthread_detach(tid)) 
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], DETACHTHREADERR,"pthread_detach failed");
  }    
  FD_ZERO(&rSet);
  FD_SET(iSockfd, &rSet);
  
  while(1)
  {
    if (select(iSockfd + 1, &rSet, NULL, NULL, NULL) == 0)
    {
    	continue;
    }   
    if (FD_ISSET(iSockfd, &rSet))
    {
    	outLen = read(iSockfd, outData, MAXLINE);	
    	/*
			for(i=0;i<3;i++)
			{
				iLastThreeStr[i]=outData[strlen(outData)-3+i];
			}
			printf("%s\n", iLastThreeStr);
			*/
      if (outLen == 0)
      {
        FD_CLR(iSockfd, &rSet);
        Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], DETACHTHREADERR,"Client logout");
        sem_wait(&sem);  
        MsgData.clients_info[iSockfd].isEmpty = 0;
        sem_post(&sem);
        break;
      }
      else
      {
      	sem_wait(&sem);
      	HandleTransMsg(iSockfd, outData, outLen);
        sem_post(&sem);  
      }
    }
    Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], DETACHTHREADERR,"outData=%s", outData);
  }
  Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], DETACHTHREADERR,"Child Thread:%lu exiting", (unsigned long)tid);
  pthread_exit((void *)&iSockfd);
}

int StartUlaneService()
{
	int 		             iRet = 0;
	int 		             iListenfd 	= 0;
	int 		             iTimeout		= 3;
	int 		             iConnfd = 0;
	//threadpool_t*      iThp;
	pthread_t 	         tid;
	int                  nReady;    
	fd_set               rSet;    
	g_serverTag  = 0;
	
	//信号处理
	if(signal(SIGINT, sigint) == SIG_ERR)
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[1], iRet,"Signal Error");
  	return -1;
  } 
	memset(&iServerInfo, 0, sizeof(iServerInfo));
	//初始化服务器 全局变量
	iRet = Ulane_ServerInfoInit(&iServerInfo);
	if (iRet != 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"func Ulane_ServerInfoInit() err");
		return iRet;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[1], iRet,"Server system init ok");
	
	//服务器端初始化
	iRet = sckServer_init(iServerInfo.iSerPort, &iListenfd);
	if (iRet != 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"func sckServer_init() err");
		return iRet;
	}
	//iThp = threadpool_create(3,100,100);    //线程池里最小3个线程，最大100个，队列最大值100
  memset(&MsgData, 0, sizeof(MsgData));
  MsgData.maxfd = iListenfd;
  FD_ZERO(&rSet);
  FD_SET(iListenfd, &rSet);
  iRet = sem_init(&sem, 0, 1);
	while (1)
	{
		if (g_serverTag ==1 )
		{
			break;
		}
    if ((nReady = select(iListenfd + 1, &rSet, NULL, NULL, NULL)) == 0)
    {
    	continue;
    } 
    if (FD_ISSET(iListenfd, &rSet))
    {
      iRet = sckServer_accept(iListenfd, iTimeout, &iConnfd);
			if (iRet ==Sck_ErrTimeOut)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"func sckServer_accept() Sck_ErrTimeOut");
				continue;
			}
			else if (iRet != 0)
			{
				iRet = 2;
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"fun sckServer_accept() err");
				continue;
			}
      if (iConnfd < FD_SETSIZE )
      {
        sem_wait(&sem) ;
        if (MsgData.clients_info[iConnfd].isEmpty == 0)
        {
          if (iConnfd > MsgData.maxfd)
          {
              MsgData.maxfd = iConnfd;
          }
          MsgData.clients_info[iConnfd].isEmpty = -1;
          sem_post(&sem);
          iRet =  sckServer_send(iConnfd, iTimeout, welcome, strlen(welcome));
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
          Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iConnfd,"Client login");
        }
        else
        {
          sem_post(&sem);
          Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iConnfd,"The Client Socket[%d] Is Online\n", iConnfd);
          continue;
        }
        //threadpool_add(iThp,UlaneServer_Routine,(void*)(iConnfd));
        iRet = pthread_create(&tid, NULL, UlaneServer_Routine, (void *)&iConnfd);
        if (iRet)
        {
        	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Can't Create Thread");
        }
        Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"ThreadID:%ld", (unsigned long)tid);
      }
      else
      {
      	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"Too Many Clients");
      } 
    }
	}
	//服务器端环境释放 
	sckServer_close(iListenfd);
	sckServer_destroy();
	//释放线程池
	//threadpool_destroy(iThp);  
	sleep(1);  
	//数据库资源释放
	if(iSql != NULL)
	{
		UlaneDB_Close(iSql);				
	}
	return 0;
}

//数据入库处理中心
int QueueMsgServerDataHandleCenter()
{
	int           msgId;
	char          Msg[MSGSIZE];
	char*         iEventType;  //事件类型 
	char*         outExtJson;
	int           iRet;
	
	//数据库初始化
	iSql = Ulane_ServerDBInit("localhost", "root", "123456", "minitor", 0, NULL, 0);
	if (iSql == NULL) 
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[1], iRet,"Ulane_ServerDBInit() exec error");
		return iRet;
	}
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[1], iRet,"Server databases system init ok");
	
	//创建消息队列
	msgId = CreatMsgQueue();
	if(msgId < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], 100,"CreatMsgQueue Error");
		return -1;
	}		
	while(1)
	{
		memset(Msg, 0,sizeof(Msg));
		RecvQueueMsg(msgId, mainThread_Type, Msg);
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], 100,"mainThread_Type:%s\n",Msg);
		printf("fork msg = %s\n", Msg);
		//解析推过来入库的消息
		iEventType = EveNameParse(Msg);
		printf("Fork iEventType = %s\n", iEventType);
		if(iEventType == NULL)
		{
			 continue;	
		}
		else if(strcmp(iEventType, "stationstatelog")==0)
		{
			//分机状态明细统一入库处理  1
			iRet = StationStateLog(Msg, &outExtJson);                        
			if(iRet == -1)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"StationStateLog error");
				continue;
			}
		}
		else if(strcmp(iEventType, "agentsigninlog") == 0 || strcmp(iEventType, "agentsignoutlog") == 0)
		{
			//签入签出明细统一入库处理  2
			printf("Msg = %s\n", Msg);
			iRet = AgentSignInLog(Msg, &outExtJson); 
			if(iRet == -1)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"AgentSignInLog error");
				continue;
			}                        
		}
		else if(strcmp(iEventType, "vdncalllog") == 0)
		{
			//呼入通话明细统一入库处理  3
			iRet = VdnCallLog(Msg, &outExtJson); 
			if(iRet == -1)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"VdnCallLog error");
				continue;
			}                             
		}
		else if(strcmp(iEventType, "stationcalllog") == 0)  
		{
			//呼出通话明细统一入库处理  4 
			iRet = StationCallLog(Msg, &outExtJson);
			if(iRet == -1)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"StationCallLog error");
				continue;
			}                          
		}
		else if(strcmp(iEventType, "agentstatelog") == 0)  
		{
			//坐席实时状态入库处理  5
			iRet = AgentStateLog(Msg, &outExtJson); 
			if(iRet == -1)
			{
				Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], iRet,"AgentStateLog error");
				continue;
			}                         
		}
	}
	DestroyQueue(msgId);
	return 0;
}

//信号处理函数
void(sighandler_t)(int arg)
{
	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[3], arg,"Server already accept the signal");
	g_serverTag = 1;
	return ;		
}

int main()
{
	pid_t                pid;            //进程ID
	int                  iRet  = 0;
	pthread_t            tid;
	//安装一个信号处理函数
	signal(SIGUSR1, sighandler_t);
	//守护进程
	//INIT_DAEMON

	pid = fork();
	if(pid > 0)
	{
		 QueueMsgServerDataHandleCenter();	
	}
	else if(pid == 0)
	{
		sleep(1);
		StartUlaneService();
	}
	else
  {
  	Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], 100,"Fork Error");
  }
	return 0;	
}
