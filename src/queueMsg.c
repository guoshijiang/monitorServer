/*=============================================================
 *文件名称: queueMsg.c
 *创建时间: 2017.7.7
 *作    者: 郭世江
 *修改记录:
 *2017.7.7 创建文件
 *功能描述：
 *  消息队列公共函数封装
 * ===========================================================*/
#include "queueMsg.h"

int mainThread_Type = 1;
int dataThread_type = 2;

//生成新消息队列或者取得已存在的消息队列
static int PublicMsgQueue(int iFlags) 
{
	key_t      iKey;
	int        MsgId;
	iKey = ftok(FTOKPARAMONE, FTOKPARAMTWO); 
	if(iKey < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], PMQFTOKERR,"ftok Error");
		return -1;
	}
	MsgId = msgget(iKey, iFlags);
	return MsgId;
}

//创建消息队列
int CreatMsgQueue()
{
	int iFlags = IPC_CREAT | IPC_EXCL | 0666;
	int MsgId = PublicMsgQueue(iFlags);
	if(MsgId == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], CMQUEUEMSGERR,"PublicMsgQueue Error");
		return -1;
	}
	return MsgId;
}

//获取消息队列
int GetMsgQueue()
{
	int iFlags = IPC_CREAT;
	int MsgId = PublicMsgQueue(iFlags);
	if(MsgId == -1)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], GMQUEUEMSGERR,"PublicMsgQueue Error");
		return -1;
	}
	return MsgId;
}

//发送消息
int SendQueueMsg(int msgId,int sendType,const char* Msg)
{
	struct msgbuf    sendBuf;
	sendBuf.mtype = sendType;
	strncpy(sendBuf.mtext,Msg,strlen(Msg)+1);
	if(msgsnd(msgId,&sendBuf,sizeof(sendBuf.mtext),0) < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], SENDQUEUEMSGERR,"msgsnd Error");
	  return -1;
	}
	return 0;
}

//接收消息
int RecvQueueMsg(int msgId,int recvType,char* msgOut)
{
	struct msgbuf recvBuf;
	recvBuf.mtype = 0;
	memset(recvBuf.mtext, 0x00, sizeof(recvBuf.mtext));

	if(msgrcv(msgId,&recvBuf,sizeof(recvBuf.mtext),recvType,0) < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], RECVQUEUEMSGERR,"msgrcv Error");
	  return -1;
	}
	strcpy(msgOut, recvBuf.mtext);
	return 0;	
}

//非阻塞式接收消息
int RecvQueuePeekMsg(int msgId,int recvType,char* msgOut)
{
	struct msgbuf recvBuf;
	recvBuf.mtype = 0;
	memset(recvBuf.mtext, 0x00, sizeof(recvBuf.mtext));

	if(msgrcv(msgId,&recvBuf,sizeof(recvBuf.mtext),recvType,IPC_NOWAIT) < 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], RECVQUEUEMSGERR,"msgrcv Error");
	  return -1;
	}
	strcpy(msgOut, recvBuf.mtext);
	return 0;	
}

//销毁消息队列
int DestroyQueue(int msgId)
{
	if(msgctl(msgId,IPC_RMID,NULL) != 0)
	{
		Ulane_WriteLog(__FILE__, __LINE__, LogLevel[4], FREEQUEUEMSGERR,"msgctl Error");
		return -1;
	}
	return 0;	
}
