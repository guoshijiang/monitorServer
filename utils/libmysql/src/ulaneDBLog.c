/*=============================================================
 *文件名称: ulaneDBLog.c
 *创建时间: 2017.6.23
 *作    者: 郭世江
 *修改记录:
 *2017.6.23 创建文件
 *功能描述：打日志API
 * ===========================================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ulaneDBLog.h"

//宏定义
#define ULANE_MAX_STRING_LEN 10240

//日志类别
#define ULANE_NO_LOG_LEVEL		0
#define ULANE_DEBUG_LEVEL			1
#define ULANE_INFO_LEVEL			2
#define ULANE_WARNING_LEVEL		3
#define ULANE_ERROR_LEVEL			4

int DBLogLevel[5]=
{ 
	ULANE_NO_LOG_LEVEL, 
	ULANE_DEBUG_LEVEL, 
	ULANE_INFO_LEVEL, 
	ULANE_WARNING_LEVEL, 
	ULANE_ERROR_LEVEL
};

//日志的名称
char ULANELevelName[5][10]=
{
	"NOLOG", 
	"DEBUG", 
	"INFO", 
	"WARNING", 
	"ERROR"
};

//生成日志文件的时间
static int ULANE_LogFileTime(char* fileTime)
{
	struct tm*		  tmTime = NULL;
	size_t			    timeLen = 0;
	time_t			    tTime = 0;	
	tTime =         time(NULL);
	tmTime = localtime(&tTime);
	timeLen = strftime(fileTime, 33, "%Y%m%d", tmTime);
	return timeLen;
}

//日志文件中的时间
static int ULANE_GetCurTime(char* strTime)
{
	struct tm*		  tmTime = NULL;
	size_t			    timeLen = 0;
	time_t			    tTime = 0;	
	tTime =         time(NULL);
	tmTime = localtime(&tTime);
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);
	return timeLen;
}

static int ULANE_OpenFile(int* pFp)
{
	char         str[ULANE_MAX_STRING_LEN];
	char         timeFileName[20];
	char	       fileName[1024];
	int	         strLen = 0;
  int	         tmpStrLen = 0;

	//加入LOG时间
	memset(timeFileName, 0, sizeof(timeFileName));
  tmpStrLen = ULANE_LogFileTime(timeFileName);
  tmpStrLen = sprintf(str, "[%s] ", timeFileName);
  strLen = tmpStrLen;
	
	memset(fileName, 0, sizeof(fileName));
	sprintf(fileName, "%s/log/%s_dbsys.log", "/MonitorForC", timeFileName); 
  *pFp = open(fileName, O_WRONLY|O_CREAT|O_APPEND, 0666);
  if(*pFp < 0)
  {
      return -1;
  }
	return 0;
}

static void ULANE_DBLogCore(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
  char            str[ULANE_MAX_STRING_LEN];
  int	            strLen = 0;
  char            tmpStr[64];
  int	            tmpStrLen = 0;
  int             pf = 0;
  
  //初始化
  memset(str, 0, ULANE_MAX_STRING_LEN);
  memset(tmpStr, 0, 64);
  
  //加入LOG时间
  tmpStrLen = ULANE_GetCurTime(tmpStr);
  tmpStrLen = sprintf(str, "[%s] ", tmpStr);
  strLen = tmpStrLen;

  //加入LOG等级
  tmpStrLen = sprintf(str+strLen, "[%s] ", ULANELevelName[level]);
  strLen += tmpStrLen;
  
  //加入LOG状态
  if (status != 0) 
  {
      tmpStrLen = sprintf(str+strLen, "[ERRNO is %d] ", status);
  }
  else
  {
  	tmpStrLen = sprintf(str+strLen, "[SUCCESS] ");
  }
  strLen += tmpStrLen;

  //加入LOG信息
  tmpStrLen = vsprintf(str+strLen, fmt, args);
  strLen += tmpStrLen;

  //加入LOG发生文件
  tmpStrLen = sprintf(str+strLen, " [%s]", file);
  strLen += tmpStrLen;

  //加入LOG发生行数
  tmpStrLen = sprintf(str+strLen, " [%d]\n", line);
  strLen += tmpStrLen;
    
  //打开LOG文件
  if(ULANE_OpenFile(&pf))
	{
		return ;
	}
  //写入LOG文件
  write(pf, str, strLen);
  close(pf);
  return ;
}

void Ulane_DBWriteLog(const char *file, int line, int level, int status, const char *fmt, ...)
{
  va_list args;
	if(level == ULANE_NO_LOG_LEVEL)
	{
		return ;
	}
  va_start(args, fmt);
  ULANE_DBLogCore(file, line, level, status, fmt, args);
  va_end(args);
  return ;
}
