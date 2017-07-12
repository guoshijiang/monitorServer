/*=============================================================
 *�ļ�����: ulaneStmtMysql.c
 *����ʱ��: 2017.6.22
 *��    ��: ������
 *�޸ļ�¼:
 *2017.6.22 �����ļ�
 *����������mysqlԤ�����װ 
 * ===========================================================*/
#include <stdlib.h>
#include <unistd.h>
#include "ulaneMysql.h"
#include "ulaneStmtMysql.h"
#include "ulaneDBLog.h"

//����������õ����API�����Ĵ������
ULANEUNSIGNINT ULANE_StmtErrno(ULANESTMT iStmt)
{
	return mysql_stmt_errno(iStmt); 
}

//�����˴�����ַ��������δ���ִ��󣬷��ؿ��ַ���
ULANESTR ULANE_StmtError(ULANESTMT iStmt)
{
	return mysql_stmt_error(iStmt); 
}

//����MYSQL_STMT���
ULANESTMT ULANE_StmtInit(ULANESQL uSql)
{
	ULANESTMT  iStmtRet;
	if(uSql == NULL)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], STMTINITERR,"uSql is NULL");
		exit(0);	
	}
	iStmtRet = mysql_stmt_init(uSql);
	if(uSql == NULL)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], STMTINITERR,"CR_OUT_OF_MEMORY");
		exit(0);	
	}
	return iStmtRet;
}

//��Ԥ�����������Ӵ�ռλ����sql���
ULANEINT ULANE_StmtPrepare(ULANESTMT iStmtRet, ULANESTR uSql, ULANELONG uSqLen)
{
	ULANEINT    iRet;
	if(iStmtRet == NULL || uSql == NULL || uSqLen == 0)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], STMTPREERR,"iStmtRet, uSql and uSqLen is NULL");
		return ULANE_StmtErrno(iStmtRet);
	}
	iRet = mysql_stmt_prepare(iStmtRet, uSql, uSqLen);
	if(iRet != 0)
  {
  	Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], STMTPREERR,"stmt prepare error");
	  return ULANE_StmtErrno(iStmtRet);
	}
	return iRet;
}

//����Ԥ��������в�����Ƿ�����Ŀ
ULANELONG ULANE_StmtParamCount(ULANESTMT iStmtRet)
{
	ULANELONG       iParamCount;
	if(iStmtRet == NULL)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], StmtParamCountERR,"iStmtRet is NULL");
		exit(0);
	}
	iParamCount = mysql_stmt_param_count(iStmtRet);
	if(iParamCount == 0)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], StmtParamCountERR,"iParamCount == 0");
		exit(0);
	}
	return iParamCount;
}

//�Ѹ�ֵ�Ժ������ ��ӵ�Ԥ���������
ULANEBOOL ULANE_StmtBindParam(ULANESTMT iStmtRet, ULANEBIND IBind)
{
	ULANEBOOL  iBool;
	if(iStmtRet == NULL || IBind == NULL)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], ULANE_StmtErrno(iStmtRet),"iStmtRet and IBind is NULL");
		return _FALSE_;
	}
	iBool = mysql_stmt_bind_param(iStmtRet, IBind);
	if(iBool)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], ULANE_StmtErrno(iStmtRet),"bind param failed");
		return _FALSE_;
	}
	return iBool;
}

//ִ���Ѿ���ȫ���ֵ��SQL���
ULANEINT ULANE_StmtExecute(ULANESTMT iStmtRet)
{
	ULANEINT  iRet;
	if(iStmtRet == NULL)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], StmtExecuteERR,"iStmtRet is NULL");
		return -1;
	}
	iRet = mysql_stmt_execute(iStmtRet);
	if(iRet != 0)
	{
		Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[4], StmtExecuteERR,"ULANE_StmtExecute err");
		return -1;
	}
	return iRet;
}

//ȡ����"stmt"ָ������������
ULANEBOOL ULANE_StmtClose(ULANESTMT iStmtRet)
{
	return mysql_stmt_close(iStmtRet);
}
