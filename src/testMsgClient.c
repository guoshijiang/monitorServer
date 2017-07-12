#include"queueMsg.h"

int main()
{
  int msgId = GetMsgQueue();
	char buf[MSGSIZE];
	while(1)
	{
		printf("Please Enter:");
		fflush(stdout);
		ssize_t _s = read(0,buf,sizeof(buf)-1);
		if(_s >0 )
		{
			buf[_s - 1] = '\0';
		}
		SendQueueMsg(msgId,mainThread_Type,buf);
		if(strcasecmp(buf,"quit") == 0)
		{
			break;
		}
	}
	return 0;
}
