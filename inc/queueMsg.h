/*=============================================================
 *文件名称: queueMsg.h
 *创建时间: 2017.7.7
 *作    者: 郭世江
 *修改记录:
 *2017.7.7 创建文件
 *功能描述：
 *  消息队列公共函数封装
 * ===========================================================*/
#ifndef __QUEUEMSG_H
#define __QUEUEMSG_H

#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include "ulaneLog.h"

#ifdef __cplusplus
extern "C" {
#endif

//宏定义区域
#define FTOKPARAMONE    "/tmp"
#define FTOKPARAMTWO    0x6666
#define  MSGSIZE        10240

//错误码定义区域
#define PMQFTOKERR        500
#define CMQUEUEMSGERR     501
#define GMQUEUEMSGERR     502
#define SENDQUEUEMSGERR   503
#define RECVQUEUEMSGERR   504
#define FREEQUEUEMSGERR   505

//声明区域
extern int mainThread_Type;
extern int dataThread_type;

//结构体定义区域
struct msgbuf
{
	long mtype;
	char mtext[MSGSIZE];
};

/*=========================================================
 *函数入参:  无
 *函数出参:  无
 *返回值:   成功返回创建的消息队列的ID,失败返回-1
 *功能说明: 创建消息队列
 *===========================================================*/
int CreatMsgQueue(); 

/*=========================================================
 *函数入参:  无
 *函数出参:  无
 *返回值:   成功返回创建的消息队列的ID,失败返回-1
 *功能说明: 获取消息队列
 *===========================================================*/
int GetMsgQueue(); 

/*=========================================================
 *函数入参: msgId     消息队列的ID
 						sendType  发送的消息类型
 						Msg	      要发送的消息
 *函数出参: 
 *返回值:   成功返回0，失败返回-1
 *功能说明: 发送消息
 *===========================================================*/
int SendQueueMsg(int msgId,int sendType,const char* Msg);

/*=========================================================
 *函数入参: msgId     消息队列的ID   
 						recvType	接收的消息类型
 *函数出参: msgOut    接收到的消息
 *返回值:   成功返回0，失败返回-1
 *功能说明: 接收消息
 *===========================================================*/
int RecvQueueMsg(int msgId,int recvType,char* msgOut);

/*=========================================================
 *函数入参: msgId     消息队列的ID   
 						recvType	接收的消息类型
 *函数出参: msgOut    接收到的消息
 *返回值:   成功返回0，失败返回-1
 *功能说明: 非阻塞式接收消息
 *===========================================================*/
int RecvQueuePeekMsg(int msgId,int recvType,char* msgOut);

/*=========================================================
 *函数入参: msgId        
 *函数出参: 无
 *返回值:   成功返回0，失败返回-1  
 *功能说明: 销毁消息队列
 *===========================================================*/
int DestroyQueue(int msgId); 

#ifdef __cplusplus
}
#endif

#endif
