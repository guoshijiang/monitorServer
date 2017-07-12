/*=============================================================
 *文件名称: ulaneMysql.h
 *创建时间: 2017.6.20
 *作    者: 郭世江
 *修改记录:
 *2017.6.20 创建文件
 *功能描述：
 *  mysql
 * ===========================================================*/
#ifndef __ULANEMYSQL_H
#define  __ULANEMYSQL_H
#include <stdio.h>
#include <mysql.h>

//宏定义
#define SQLINITERR        2000                    //初始化数据库失败
#define SQLCONNECTERR     2001                    //连接数据库失败
#define NOSELECTSQLERR    2002                    //执行非查询SQL语句失败
#define SELECTSQLERR      2003                    //执行查询SQL语句失败
#define GETSQLCOUNT       2004                    //获取count失败
#define GETFILESERR       2005                    //获取FILEDS失败
#define FETCHROWERR       2006                    //获取ROW失败
#define STARTTRANSERR     2007                    //开启事务失败
#define OPERATETRANSERR   2008                    //设置手动事务提交失败
#define AUTOTRANERR       2009                    //设置自动事务提交失败
#define COMMITERR         2010                    //提交失败
#define RollbackERR       2011                    //回滚事务失败

#define SET_TRAN	        "SET AUTOCOMMIT=0"  		//手动提交事务
#define UNSET_TRAN	      "SET AUTOCOMMIT=1"      //自动提交事务

//数据类型的定义
typedef MYSQL*             ULANESQL;
typedef const char*        ULANESTR;
typedef int                ULANEINT;
typedef unsigned int       ULANEUNSIGNINT;
typedef unsigned long      ULANELONG;
typedef MYSQL_RES*         ULANESQLRES;
typedef MYSQL_ROW          ULANESQLROW;
typedef MYSQL_FIELD*       ULANEFIELD;

/*=========================================================
 *函数入参: uSql   数据库连接句柄
 *函数出参: 无
 *返回值:   成功返回数据执行函数错误的错误码,失败返回NULL
 *功能说明: 获取数据库的错误号
 *===========================================================*/
ULANEUNSIGNINT UlaneDB_Erron(ULANESQL uSql);
 
/*=========================================================
 *函数入参: 无
 *函数出参: 无
 *返回值:   成功返回事件类型,失败返回NULL
 *功能说明: 初始化数据库
 *===========================================================*/
ULANESQL UlaneDB_init(ULANESQL uSql);

/*=========================================================
 *函数入参: uSql 数据库句柄
 						iHost连接的数据库主机
 						iUser连接的数据库用户名
 						iPasswd连接的数据库密码
 						iDbName连接的数据库名字 
 						iPort连接的TCP/IP端口号 
 						iUnixSock套接字
 						iClientFlag标志位
 *函数出参: 无
 *返回值:   成功返回数据库句柄,失败返回NULL
 *功能说明: 连接数据库
 *===========================================================*/
ULANESQL UlaneDB_Connect(ULANESQL uSql, ULANESTR iHost, ULANESTR iUser, ULANESTR iPasswd, ULANESTR iDbName, ULANEUNSIGNINT iPort, ULANESTR iUnixSock, ULANELONG iClientFlag);

/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 无
 *返回值:   成功返回0； 失败返回错误码
 *功能说明: 执行非查询的SQL语句
 *===========================================================*/
int UlaneDB_ExecSql(ULANESQL uSql, ULANESTR iSql);

/*=========================================================
 *函数入参: uSql 数据库句柄 
 						iSql 要输入的SQL语句
 *函数出参: 无
 *返回值:   成功返回0； 失败返回NULL
 *功能说明: 执行没有表头的查询的SQL语句
 *===========================================================*/
ULANESQLRES UlaneDB_ExecQuerySqlRes(ULANESQL uSql, ULANESTR iSql);

/*=========================================================
 *函数入参: uSql数据库句柄
 *函数出参: 无
 *返回值:   
 *功能说明: 获取列数
 *===========================================================*/
ULANEUNSIGNINT UlaneDB_GetSqlCount(ULANESQL uSql);

/*=========================================================
 *函数入参: iResult 结果集
 *函数出参: 无
 *返回值:   成功返回表头，失败返回-1
 *功能说明: 打印表头
 *===========================================================*/
ULANEFIELD UlaneDB_iFields(ULANESQLRES iResult);

/*=========================================================
 *函数入参: iResult 结果集
 *函数出参: 无
 *返回值:   
 *功能说明: 解析结果集
 *===========================================================*/
ULANESQLROW UlaneDB_FetchRow(ULANESQLRES iResult);

/*=========================================================
 *函数入参: iResult 结果集
 *函数出参: 无
 *返回值:   无返回值
 *功能说明: 释放结果集
 *===========================================================*/
void UlaneFree_DBRes(ULANESQLRES iResult);
 
/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 无
 *返回值:   无返回值
 *功能说明: 关闭数据库
 *===========================================================*/
void UlaneDB_Close(ULANESQL uSql);

/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 
 *返回值:   
 *功能说明: 开启事务
 *===========================================================*/
ULANEINT UlaneDB_StartTrans(ULANESQL uSql);  			

/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 
 *返回值:   
 *功能说明: 设置事务为手动提交
 *===========================================================*/
ULANEINT UlaneDB_OperationTran(ULANESQL uSql);

/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 
 *返回值:   
 *功能说明: 设置事务为自动提交
 *===========================================================*/
ULANEINT UlaneDB_AutoTran(ULANESQL uSql);	
	
/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 
 *返回值:   
 *功能说明: 执行commit，手动提交事务
 *===========================================================*/
ULANEINT UlaneDB_Commit(ULANESQL uSql);

/*=========================================================
 *函数入参: uSql 数据库句柄
 *函数出参: 
 *返回值:   
 *功能说明: 执行rollback，回滚事务
 *===========================================================*/	
ULANEINT UlaneDB_Rollback(ULANESQL uSql);	
	
	
#endif