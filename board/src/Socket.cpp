/*=========================================================
 *文件名称:  Socket.cpp
 *创建日期：2017-6-15
 *作者：郭世江
 *修改记录：
 *2017-6-15  首次创建
 *功能描述 Socket封装
===========================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "Socket.h"

Socket::Socket()
{
  m_iSock = 0;
}

Socket::Socket(int hSock)
{
  m_iSock = hSock;
}

Socket::~Socket()
{
}
    
int Socket::Connect(const char *szIp, int iPort)
{
  struct sockaddr_in  addr;
  int iRet;
  
  m_iSock = socket( AF_INET, SOCK_STREAM, 0 ); 
  if(m_iSock < 0)
  {
    printf("Socket:socket1 failure!\n");
    return -1;
  }
  
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(szIp);
	addr.sin_port = htons( iPort ); 
	
	iRet = connect( m_iSock,(struct sockaddr*)&addr,sizeof(addr));
	if(iRet < 0)
  {
    printf("CSock:connect failure\n");
  }
	return iRet;
}

int Socket::Bind(const char *szIp, int iPort)
{
  struct sockaddr_in  addr;
  int iRet;
  
  m_iSock = socket( AF_INET, SOCK_STREAM, 0 ); 
  if(m_iSock < 0)
  {
    printf("Socket:socket2 failure!\n");
    return -1;
  }
  
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(szIp);
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons( iPort ); 
	
	iRet = bind( m_iSock,(struct sockaddr *)&addr,sizeof(addr));
	if(iRet < 0)
  {
	  printf("Socket:bind failure\n");
  }
	return iRet;
}

int Socket::Listen(int iNum)
{
  return listen(m_iSock, iNum);
}

int Socket::Accept(Socket &conSock)
{
	struct sockaddr_in  addr;
	socklen_t iLen;
  int cSock = accept(m_iSock, (struct sockaddr *)&addr, &iLen);
  conSock.Attach(cSock); 
  return cSock;
}

int Socket::Close()
{
  return close(m_iSock);
}

int Socket::Send(const char *szData, int iDataLen, int iTimeOut)
{
  int rtnVal, counter, wantLen;
	counter = 0;
	wantLen = iDataLen ;
	while ( counter != iDataLen ) 
	{
		if(IsReadyToWrite(iTimeOut)) 
		{
			rtnVal = write(m_iSock, szData + counter, wantLen);
			if ( rtnVal == -1 ) 
			{
				if(errno == EPIPE)
					return 0;
				else
					return -1;
			}
			counter += rtnVal;
			wantLen = iDataLen - counter;
		}
		else
			return -1;
	}
	return (counter);  
}

int Socket::Recv(char *szData, int iDataLen, int iTimeOut)
{
  int rtnVal, counter, wantLen;

	counter = 0;
	wantLen = iDataLen ;

	while ( counter != iDataLen ) 
	{
		if(IsReadyToRead(iTimeOut)) 
		{
			rtnVal = read(m_iSock, szData + counter,wantLen);
			if ( rtnVal <= 0 ) 
			{
				 return rtnVal;
			}
			counter += rtnVal;
			wantLen = iDataLen - counter;
		}
		else
		{
			return -1;
		}
	}
	return (counter);  
}

int Socket::Attach(int iSock)
{
  m_iSock = iSock;
  return 0;
}

int Socket::IsReadyToRead(int iTimeOut)
{
  struct 	timeval tv;
  struct 	timeval * ptv;
  fd_set 	ready;

  int 	rtnVal, len;

  if ( iTimeOut != TIME_INFINITE ) 
  {
    tv.tv_sec  = iTimeOut/1000;
    tv.tv_usec = (iTimeOut%1000)*1000;
    ptv = &tv;
  }
	else 
  {
    ptv = 0;
	}
	
	FD_ZERO(&ready);
	FD_SET(m_iSock, &ready);
	rtnVal = select(m_iSock + 1, &ready, 0, 0, ptv);
	switch ( rtnVal ) 
  {
    case 0: 
      return _FALSE_;  
    case -1:
      return _FALSE_; 
  }

  if( FD_ISSET(m_iSock,&ready))
    return _TRUE_;
  return _FALSE_;
}

int Socket::IsReadyToWrite(int iTimeOut)
{
	struct 	timeval tv;
	struct 	timeval * ptv;

	fd_set 	ready;

	int 	rtnVal, len;

	if ( iTimeOut != TIME_INFINITE ) 
	{
		tv.tv_sec  = iTimeOut/1000;
		tv.tv_usec = (iTimeOut%1000)*1000;
		ptv = &tv;
	}
	else 
	{
		ptv = 0;
	}

	FD_ZERO(&ready);
	FD_SET(m_iSock, &ready);
	rtnVal = select(m_iSock + 1, 0,&ready,  0, ptv);

	switch ( rtnVal ) {
		case 0: 
			return _FALSE_;  
		case -1:
			return _FALSE_; 
	}
	if ( FD_ISSET(m_iSock,&ready) )
		return _TRUE_;
	return _FALSE_;
}

int Socket::GetHandle()
{
	return m_iSock;
}
