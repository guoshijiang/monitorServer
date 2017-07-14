#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "Socket.h"
#include "cJSON.h"


size_t       rLen;

char* ReadFile(const char *iFName)  
{ 
	FILE         *fp;  
  int          len = 0;
  char *data = NULL;   
	if(iFName == NULL)
	{
		printf("file name iFName is NULL\n");	
		return NULL;
	}
  fp=fopen(iFName,"rb");
  if(fp == NULL) 
  {
  	printf("file descriptor fp is null\n");
  	return NULL;
  } 
  fseek(fp,0,SEEK_END);  
  len=ftell(fp);  
  fseek(fp,0,SEEK_SET);
   
  data=(char*)malloc(len+1);
  if(data == NULL)
  {
  	printf("malloc err\n");
  	return NULL;
  } 
  rLen = fread(data,1,len,fp); 
  if(rLen == 0)
  {
  	printf("read file err\n");
  	return NULL;	
  }
  return data;
  fclose(fp);
}  
/*
char* CreateJsonBuf()
{
		cJSON *json = cJSON_CreateObject();
		cJSON_AddStringToObject(json,"cmdName","agentlogon");
		cJSON_AddStringToObject(json,"msgID","100000001");
		cJSON_AddStringToObject(json,"agentid","agentmonitor1");
		cJSON_AddStringToObject(json,"agenttype","agenttype_3");
	  char *buf = cJSON_Print(json);
	  strcat(buf, "eof");
	  strcat(buf, "\r\n");
		return buf;
}
*/
void *UlaneTestTool(void *arg)
{
	int             iRet, ret;
	char            socket_addr[20] = "192.168.110.131";
  unsigned short  socket_port = 9999;
  size_t          send_len = 1024;
  int             recv_len = 1024;
	char            *send_buf;
	Socket          sendSock;
	int             i  =  0; 
	char*           fname;
	char            *data;
	char            *szData;
	int timeOut     = 3;

	iRet = sendSock.Connect(socket_addr, socket_port);
	if(iRet < 0)
	{
		printf("sendSock.Connect err\n");
		return NULL;	
	}
	for(i = 0; i < 10; i++)
	{
		sleep(3);
		sprintf(fname, "./package_9.json", i);
		data = ReadFile(fname);
		//strcat(data, "\r\n");
		if(data == NULL)
		{
			printf("ReadFile err\n");
			return NULL;
		}
		printf("data = %s\n", data);
		iRet = sendSock.Send(data, send_len, timeOut);
		if(iRet < 0)
		{
			printf("sendSock.Connect err\n");
			return NULL;
		}
		printf("SendData=%s\n", data);
	}
	sendSock.Close();
}


int main()
{
	pthread_t      pid[20];
	int            i;
	
	for (i=0; i< 1; i++)
	{
	 	pthread_create( &pid[i], NULL, UlaneTestTool, NULL);
	}
	
  for (i=0; i< 1; i++)
  {
  	 pthread_join(pid[i], NULL);
  }
	return 0;	
}
