/*=============================================================
 *�ļ�����: ulaneMysql.h
 *����ʱ��: 2017.6.20
 *��    ��: ������
 *�޸ļ�¼:
 *2017.6.20 �����ļ�
 *����������
 *  mysql
 * ===========================================================*/
#ifndef __ULANEMYSQL_H
#define  __ULANEMYSQL_H
#include <stdio.h>
#include <mysql.h>

//�궨��
#define SQLINITERR        2000                    //��ʼ�����ݿ�ʧ��
#define SQLCONNECTERR     2001                    //�������ݿ�ʧ��
#define NOSELECTSQLERR    2002                    //ִ�зǲ�ѯSQL���ʧ��
#define SELECTSQLERR      2003                    //ִ�в�ѯSQL���ʧ��
#define GETSQLCOUNT       2004                    //��ȡcountʧ��
#define GETFILESERR       2005                    //��ȡFILEDSʧ��
#define FETCHROWERR       2006                    //��ȡROWʧ��
#define STARTTRANSERR     2007                    //��������ʧ��
#define OPERATETRANSERR   2008                    //�����ֶ������ύʧ��
#define AUTOTRANERR       2009                    //�����Զ������ύʧ��
#define COMMITERR         2010                    //�ύʧ��
#define RollbackERR       2011                    //�ع�����ʧ��

#define SET_TRAN	        "SET AUTOCOMMIT=0"  		//�ֶ��ύ����
#define UNSET_TRAN	      "SET AUTOCOMMIT=1"      //�Զ��ύ����

//�������͵Ķ���
typedef MYSQL*             ULANESQL;
typedef const char*        ULANESTR;
typedef int                ULANEINT;
typedef unsigned int       ULANEUNSIGNINT;
typedef unsigned long      ULANELONG;
typedef MYSQL_RES*         ULANESQLRES;
typedef MYSQL_ROW          ULANESQLROW;
typedef MYSQL_FIELD*       ULANEFIELD;

/*=========================================================
 *�������: uSql   ���ݿ����Ӿ��
 *��������: ��
 *����ֵ:   �ɹ���������ִ�к�������Ĵ�����,ʧ�ܷ���NULL
 *����˵��: ��ȡ���ݿ�Ĵ����
 *===========================================================*/
ULANEUNSIGNINT UlaneDB_Erron(ULANESQL uSql);
 
/*=========================================================
 *�������: ��
 *��������: ��
 *����ֵ:   �ɹ������¼�����,ʧ�ܷ���NULL
 *����˵��: ��ʼ�����ݿ�
 *===========================================================*/
ULANESQL UlaneDB_init(ULANESQL uSql);

/*=========================================================
 *�������: uSql ���ݿ���
 						iHost���ӵ����ݿ�����
 						iUser���ӵ����ݿ��û���
 						iPasswd���ӵ����ݿ�����
 						iDbName���ӵ����ݿ����� 
 						iPort���ӵ�TCP/IP�˿ں� 
 						iUnixSock�׽���
 						iClientFlag��־λ
 *��������: ��
 *����ֵ:   �ɹ��������ݿ���,ʧ�ܷ���NULL
 *����˵��: �������ݿ�
 *===========================================================*/
ULANESQL UlaneDB_Connect(ULANESQL uSql, ULANESTR iHost, ULANESTR iUser, ULANESTR iPasswd, ULANESTR iDbName, ULANEUNSIGNINT iPort, ULANESTR iUnixSock, ULANELONG iClientFlag);

/*=========================================================
 *�������: uSql ���ݿ���
 *��������: ��
 *����ֵ:   �ɹ�����0�� ʧ�ܷ��ش�����
 *����˵��: ִ�зǲ�ѯ��SQL���
 *===========================================================*/
int UlaneDB_ExecSql(ULANESQL uSql, ULANESTR iSql);

/*=========================================================
 *�������: uSql ���ݿ��� 
 						iSql Ҫ�����SQL���
 *��������: ��
 *����ֵ:   �ɹ�����0�� ʧ�ܷ���NULL
 *����˵��: ִ��û�б�ͷ�Ĳ�ѯ��SQL���
 *===========================================================*/
ULANESQLRES UlaneDB_ExecQuerySqlRes(ULANESQL uSql, ULANESTR iSql);

/*=========================================================
 *�������: uSql���ݿ���
 *��������: ��
 *����ֵ:   
 *����˵��: ��ȡ����
 *===========================================================*/
ULANEUNSIGNINT UlaneDB_GetSqlCount(ULANESQL uSql);

/*=========================================================
 *�������: iResult �����
 *��������: ��
 *����ֵ:   �ɹ����ر�ͷ��ʧ�ܷ���-1
 *����˵��: ��ӡ��ͷ
 *===========================================================*/
ULANEFIELD UlaneDB_iFields(ULANESQLRES iResult);

/*=========================================================
 *�������: iResult �����
 *��������: ��
 *����ֵ:   
 *����˵��: ���������
 *===========================================================*/
ULANESQLROW UlaneDB_FetchRow(ULANESQLRES iResult);

/*=========================================================
 *�������: iResult �����
 *��������: ��
 *����ֵ:   �޷���ֵ
 *����˵��: �ͷŽ����
 *===========================================================*/
void UlaneFree_DBRes(ULANESQLRES iResult);
 
/*=========================================================
 *�������: uSql ���ݿ���
 *��������: ��
 *����ֵ:   �޷���ֵ
 *����˵��: �ر����ݿ�
 *===========================================================*/
void UlaneDB_Close(ULANESQL uSql);

/*=========================================================
 *�������: uSql ���ݿ���
 *��������: 
 *����ֵ:   
 *����˵��: ��������
 *===========================================================*/
ULANEINT UlaneDB_StartTrans(ULANESQL uSql);  			

/*=========================================================
 *�������: uSql ���ݿ���
 *��������: 
 *����ֵ:   
 *����˵��: ��������Ϊ�ֶ��ύ
 *===========================================================*/
ULANEINT UlaneDB_OperationTran(ULANESQL uSql);

/*=========================================================
 *�������: uSql ���ݿ���
 *��������: 
 *����ֵ:   
 *����˵��: ��������Ϊ�Զ��ύ
 *===========================================================*/
ULANEINT UlaneDB_AutoTran(ULANESQL uSql);	
	
/*=========================================================
 *�������: uSql ���ݿ���
 *��������: 
 *����ֵ:   
 *����˵��: ִ��commit���ֶ��ύ����
 *===========================================================*/
ULANEINT UlaneDB_Commit(ULANESQL uSql);

/*=========================================================
 *�������: uSql ���ݿ���
 *��������: 
 *����ֵ:   
 *����˵��: ִ��rollback���ع�����
 *===========================================================*/	
ULANEINT UlaneDB_Rollback(ULANESQL uSql);	
	
	
#endif