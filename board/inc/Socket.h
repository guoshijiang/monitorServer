/*=========================================================
 *文件名称:  Socket.h
 *创建日期：2017-6-15
 *作者：郭世江
 *修改记录：
 *2017-6-15  首次创建
 *功能描述 Socket封装
===========================================================*/
#ifndef __SOCKET_H
#define __SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define TIME_INFINITE -1
#define _TRUE_    1
#define _FALSE_   0
  
class Socket
{
public:
  Socket();
  Socket(int hSock);
  ~Socket();
    
  int Connect(const char *szIp, int iPort);
  int Bind(const char *szIp, int iPort);
  int Listen(int iNum = 3);
  int Accept(Socket &conSock);
  int Close();
  int Send(const char *szData, int iDataLen, int iTimeOut = 10);
  int Recv(char *szData, int iDataLen, int iTimeOut = 10);
  int GetHandle();  
private:
  int Attach(int iSock);
  int IsReadyToRead(int iTimeOut);
  int IsReadyToWrite(int iTimeOut);
  
private:
  int m_iSock;
};

#endif
