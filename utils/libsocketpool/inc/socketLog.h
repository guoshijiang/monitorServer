/*=============================================================
 *文件名称: ulaneLog.h
 *创建时间: 2017.6.23
 *作    者: 郭世江
 *修改记录:
 *2017.6.23 创建文件
 *功能描述：打日志功能
 * ===========================================================*/
#ifndef _ULANELOG_H_
#define _ULANELOG_H_

#ifdef __cplusplus
extern 'C'
{
#endif

//实际使用的Level
extern int  SocketLevel[5];
/*=========================================================
 *函数入参: file:  文件名称
						line:  文件行号
						level: 错误级别
								0- 没有日志
								1- debug级别
								2- info级别
								3- warning级别
								4- err级别
						status:错误码
            fmt:   可变参数
 *函数出参: 无
 *返回值:   无
 *功能说明: 打日函数
 *===========================================================*/
void Socket_Log(const char *file, int line, int level, int status, const char *fmt, ...);

#ifdef __cpluspluse
}
#endif

#endif
