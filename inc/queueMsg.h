/*=============================================================
 *�ļ�����: queueMsg.h
 *����ʱ��: 2017.7.7
 *��    ��: ������
 *�޸ļ�¼:
 *2017.7.7 �����ļ�
 *����������
 *  ��Ϣ���й���������װ
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

//�궨������
#define FTOKPARAMONE    "/tmp"
#define FTOKPARAMTWO    0x6666
#define  MSGSIZE        10240

//�����붨������
#define PMQFTOKERR        500
#define CMQUEUEMSGERR     501
#define GMQUEUEMSGERR     502
#define SENDQUEUEMSGERR   503
#define RECVQUEUEMSGERR   504
#define FREEQUEUEMSGERR   505

//��������
extern int mainThread_Type;
extern int dataThread_type;

//�ṹ�嶨������
struct msgbuf
{
	long mtype;
	char mtext[MSGSIZE];
};

/*=========================================================
 *�������:  ��
 *��������:  ��
 *����ֵ:   �ɹ����ش�������Ϣ���е�ID,ʧ�ܷ���-1
 *����˵��: ������Ϣ����
 *===========================================================*/
int CreatMsgQueue(); 

/*=========================================================
 *�������:  ��
 *��������:  ��
 *����ֵ:   �ɹ����ش�������Ϣ���е�ID,ʧ�ܷ���-1
 *����˵��: ��ȡ��Ϣ����
 *===========================================================*/
int GetMsgQueue(); 

/*=========================================================
 *�������: msgId     ��Ϣ���е�ID
 						sendType  ���͵���Ϣ����
 						Msg	      Ҫ���͵���Ϣ
 *��������: 
 *����ֵ:   �ɹ�����0��ʧ�ܷ���-1
 *����˵��: ������Ϣ
 *===========================================================*/
int SendQueueMsg(int msgId,int sendType,const char* Msg);

/*=========================================================
 *�������: msgId     ��Ϣ���е�ID   
 						recvType	���յ���Ϣ����
 *��������: msgOut    ���յ�����Ϣ
 *����ֵ:   �ɹ�����0��ʧ�ܷ���-1
 *����˵��: ������Ϣ
 *===========================================================*/
int RecvQueueMsg(int msgId,int recvType,char* msgOut);

/*=========================================================
 *�������: msgId     ��Ϣ���е�ID   
 						recvType	���յ���Ϣ����
 *��������: msgOut    ���յ�����Ϣ
 *����ֵ:   �ɹ�����0��ʧ�ܷ���-1
 *����˵��: ������ʽ������Ϣ
 *===========================================================*/
int RecvQueuePeekMsg(int msgId,int recvType,char* msgOut);

/*=========================================================
 *�������: msgId        
 *��������: ��
 *����ֵ:   �ɹ�����0��ʧ�ܷ���-1  
 *����˵��: ������Ϣ����
 *===========================================================*/
int DestroyQueue(int msgId); 

#ifdef __cplusplus
}
#endif

#endif
