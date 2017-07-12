#include"queueMsg.h"

int UlaneTestTool()
{
	int msgId = CreatMsgQueue();
	if(msgId <0)
	{
		printf("error");
		return 1;
	}
	char buf[MSGSIZE];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		RecvQueueMsg(msgId, mainThread_Type, buf);
		printf("client:%s\n",buf);
		if(strcasecmp(buf,"quit") == 0)
		{
			break;
		}
	}
	DestroyQueue(msgId);
	return 0;
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
