/*=============================================================
 *�ļ�����: createJson.h
 *����ʱ��: 2017.6.26
 *��    ��: ������
 *�޸ļ�¼:
 *   2017.6.26 �����ļ�
 *����������
 *  ����JSON��
 * ===========================================================*/
#ifndef __CREATE_H
#define __CREATE_H

//�����붨��
#define   CREATEJSONLOGOERR   5000      //����Ӧ���JSON������

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/*=========================================================
 *�������: ��
 *��������: eventName    �¼�����
 						succErrBuf   �ɹ�����ʧ�ܵ��ַ���
 						errMsg       ʧ�ܵĴ����
 						outJson      ������JSON��
 *����ֵ:   �ɹ�����0,ʧ�ܷ���-1
 *����˵��: ����Ӧ���¼��JSON��
 *===========================================================*/
char* CreateAnswerJson(char *eventName, char *succErrBuf, char *errMsg); //char *outJson










#endif