/*=========================================================
 *文件名称: 			Config.c
 *创建日期：			2017-6-23
 *作    者:       郭世江
 *修改记录：			2017-6-23  首次创建
 *功能描述：     读配置文件公共函数
 *===========================================================*/
#include"Config.h"  

static char *substr(char *linebuf,char *pKey)  
{  
	char *pTmp = NULL, *pRv = NULL;  
	int lenKey = 0,len = 0;
	if (linebuf==NULL || pKey==NULL)  
	{
		printf("参数为空出错(str==NULL || substr==NULL)\n");  
		return pRv;  
	}  
	pRv = strstr(linebuf, pKey);  
	if(pRv == NULL)  
	{  
		return pRv;  
	}  
	pTmp = pRv;  
	lenKey = strlen(pKey);  
	while(*pTmp != '\0' && *pTmp != ' '&& *pTmp != '=' && *pTmp != '\n')  
	{  
		len++;  
		pTmp++;  
		if(len>lenKey)  
		{  
			break;  
		}  
	}  
	if (lenKey != len)  
	{  
		return NULL;  
	}  
	return pRv;  
}  
 
int GetCfItem(const char *pFileName, char *pKey, char * pValue, int * pValueLen )  
{  
	int rv = 0; 
	FILE *fp = NULL;  
	char linebuf[LineMaxLen];  
	char *pTmp = NULL, *pBegin = NULL, *pEnd = NULL;  
	if (pFileName==NULL||pKey==NULL||pValue==NULL||pValueLen==NULL)  
	{  
		rv = -1;
		printf("获取参数失败!\n");   
		goto End;  
	}  
	fp = fopen(pFileName,"r"); 
	if (fp==NULL)  
	{  
		rv = -2; 
		printf("打开文件失败\n");  
		goto End;  
	}  
	while(!feof(fp))  
	{  
		memset(linebuf,0,LineMaxLen);  
		pTmp = fgets(linebuf, LineMaxLen, fp);
		if (pTmp==NULL)  
		{  
			break;  
		}  
		pTmp = substr(linebuf, pKey);
		if (pTmp==NULL)  
		{  
			continue;  
		} 
		pTmp = strchr(linebuf, '='); 
		if (pTmp==NULL)  
		{  
			continue;  
		}  
		pTmp=pTmp+1;

		while (1) 
		{  
			if(*pTmp==' ')  
			{  
				pTmp++;  
			}  
			else  
			{  
				pBegin = pTmp; 
				if(*pBegin == '\n'||*pBegin=='\0')  
				{  
					rv = -3; 
					printf("对应的键没有这个值\n");     
				}  
				break;  
			}  
		}  
		while (1) 
		{  
			if(*pTmp==' '||*pTmp=='\n'||*pTmp=='\0')  
			{  
				break;  
			}  
			else  
			{  
				pTmp++;  
			}  
		}  
		pEnd = pTmp;  
		*pValueLen = pEnd-pBegin;  
		memcpy(pValue,pBegin,*pValueLen); 
		*(pValue+*pValueLen)='\0';  
		break;  
	} 
	if(pBegin==NULL)   
	{  
		printf("不能找到键\n");     
		rv = -4;  
	}  
End:  
	if (fp!=NULL)  
	{  
		fclose(fp);  
	}  
	return rv;  
} 

char *ReadConItem(char *key)
{
	char fname[128];
	memset(fname, 0, sizeof(fname));
	sprintf(fname, "%s/log/config.ini",getenv("HOME"));
	char *pFileName= fname;
  int len = 0, ret = 0; 
  memset(value, 0, sizeof(value));   
  ret = GetCfItem(pFileName,key,value,&len);  
  if(ret!=0)  
  {  
    printf("获取键值错误\n");  
    return NULL;  
  }
  return value;
} 