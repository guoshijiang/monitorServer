/*=============================================================
 *文件名称: ulaneStmtMysql.h
 *创建时间: 2017.6.22
 *作    者: 郭世江
 *修改记录:
 *2017.6.22 创建文件
 *功能描述：mysql预处理封装 
 * ===========================================================*/
#ifndef __ULANESTMTMYSQL_H
#define  __ULANESTMTMYSQL_H
#include <stdio.h>
#include <mysql.h>
#include "ulaneMysql.h"

//宏定义
#define _TRUE_    1
#define _FALSE_   0

//错误码定义
#define STMTINITERR         3000                 //预处理初始化失败
#define STMTPREERR          3001                 //向预处理环境句柄添加带占位符的sql语句失败
#define StmtParamCountERR   3002
#define StmtExecuteERR      3003


//数据类型定义
typedef   MYSQL_STMT*     ULANESTMT;
typedef   my_bool         ULANEBOOL;
typedef   MYSQL_BIND*     ULANEBIND;
typedef   MYSQL_BIND      ULANEBINDSTR;

/*=========================================================
 *函数入参: iStmt MYSQL_STMT句柄
 *函数出参: 无
 *返回值:   错误代码值。如果未出现错误，返回0。
 *功能说明: 返回最近调用的语句API函数的错误代码
 *===========================================================*/
ULANEUNSIGNINT ULANE_StmtErrno(ULANESTMT iStmt);

/*=========================================================
 *函数入参: iStmt MYSQL_STMT句柄
 *函数出参: 无
 *返回值:   描述了错误的字符串。如果未出现错误，返回空字符串
 *功能说明: 描述了错误的字符串。如果未出现错误，返回空字符串
 *===========================================================*/
ULANESTR ULANE_StmtError(ULANESTMT iStmt);


/*=========================================================
 *函数入参: uSql数据库连接句柄
 *函数出参: 无
 *返回值:   成功时，返回指向MYSQL_STMT结构的指针。
 						如果内存溢出，返回NULL
 *功能说明: 创建MYSQL_STMT句柄
 *===========================================================*/
ULANESTMT ULANE_StmtInit(ULANESQL uSql); 


/*=========================================================
 *函数入参: iStmtRet MYSQL_STMT句柄
 						uSql     要执行的SQL语句
 						uSqLen   SQL语句字符串的长度
 *函数出参: 无
 *返回值:   成功处理了语句，返回0。如果出现错误，返回非0值
 *功能说明: 向预处理环境句柄添加带占位符的sql语句
 *===========================================================*/
ULANEINT ULANE_StmtPrepare(ULANESTMT iStmtRet, ULANESTR uSql, ULANELONG uSqLen);

/*=========================================================
 *函数入参: iStmtRet MYSQL_STMT句柄
 *函数出参: 无
 *返回值:   表示语句中参数数目的无符号长整数
 *功能说明: 返回预处理语句中参数标记符的数目
 *===========================================================*/ 
ULANELONG ULANE_StmtParamCount(ULANESTMT iStmtRet);

/*=========================================================
 *函数入参: iStmtRet MYSQL_STMT句柄
 						IBind
 *函数出参: 无
 *返回值:   把赋值以后的数组 添加到预处理环境句柄
 *功能说明: 返回预处理语句中参数标记符的数目
 *===========================================================*/
ULANEBOOL ULANE_StmtBindParam(ULANESTMT iStmtRet, ULANEBIND IBind); 

/*=========================================================
 *函数入参: iStmtRet MYSQL_STMT句柄
 *函数出参: 无
 *返回值:   如果执行成功，返回0。如果出现错误，返回非0值
 *功能说明: 执行已经完全填好值的SQL语句
 *===========================================================*/
ULANEINT ULANE_StmtExecute(ULANESTMT iStmtRet);


/*=========================================================
 *函数入参: iStmtRet MYSQL_STMT句柄
 *函数出参: 无
 *返回值:   如果成功释放了语句，返回0。如果出现错误，返回非0值。
 *功能说明: 取消由“stmt”指向的语句句柄分配
 *===========================================================*/
ULANEBOOL ULANE_StmtClose(ULANESTMT iStmtRet);

#endif