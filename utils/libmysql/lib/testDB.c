#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ulaneMysql.h"
#include "ulaneDBLog.h"

int main()
{
	int ret =100; 
	int num, i;
	printf("start main function\n");
	
	Ulane_DBWriteLog(__FILE__, __LINE__, DBLogLevel[3], ret,"start main function");
	
	MYSQL* sql;
	sql = UlaneDB_init(NULL);
	if (sql == NULL) 
	{
		printf("UlaneDB_init error: %d\n", ret);
		return ret;
	}
	printf("UlaneDB_init ok...\n");
	
	sql = UlaneDB_Connect(sql, "localhost", "root", "", "mydb1", 0, NULL, 0);
	if(sql == NULL) 
	{
		printf("UlaneDB_Connect error: %d\n", ret);
		return ret;
	}
	printf("connect ok...\n");
	
	char* sqlSec = "insert into t1 values(2, 'bb')";
	ret = UlaneDB_ExecSql(sql, sqlSec);
	if (ret != 0)
	{
		printf("UlaneDB_ExecSql err\n");
		return ret;
	}
	printf("exec insert sql ok...\n");
	
	char* sqlUpd = "update t1 set tname ='dssa' where tid=2;";
	ret = UlaneDB_ExecSql(sql, sqlUpd);
	if (ret != 0)
	{
		printf("UlaneDB_ExecSql err\n");
		return ret;
	}
	printf("exec update sql ok...\n");
	
	char* sqlQury = "select * from t1";
	ULANESQLRES iResult = UlaneDB_ExecQuerySqlRes(sql, sqlQury);
	if (ret != 0)
	{
		printf("UlaneDB_ExecSql err\n");
		return ret;
	}
	
  ULANESQLROW        iRow;
	ULANEINT           iCount;
	
	printf("exec query sql ok...\n");
	iCount = UlaneDB_GetSqlCount(sql);
	if(iCount < 0)
	{
		printf("there is no query result in this select\n");
		return NULL;
	}
	
	while((iRow = UlaneDB_FetchRow(iResult))!= NULL)
	{
		for (i = 0; i < iCount; i++) 
		{
				printf("%s\t", iRow[i]);
		}
		printf("\n");
	}
	
	UlaneDB_Close(sql);
	printf("close database ok...\n");
	printf("hello world!!\n");	
}