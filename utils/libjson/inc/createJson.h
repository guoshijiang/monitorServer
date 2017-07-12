/*=============================================================
 *文件名称: createJson.h
 *创建时间: 2017.6.26
 *作    者: 郭世江
 *修改记录:
 *   2017.6.26 创建文件
 *功能描述：
 *  创建JSON串
 * ===========================================================*/
#ifndef __CREATE_H
#define __CREATE_H

//错误码定义
#define   CREATEJSONLOGOERR   5000      //创建应答的JSON串错误

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/*=========================================================
 *函数入参: 无
 *函数出参: eventName    事件名称
 						succErrBuf   成功或者失败的字符串
 						errMsg       失败的错误号
 						outJson      传出的JSON串
 *返回值:   成功返回0,失败返回-1
 *功能说明: 创建应答登录的JSON串
 *===========================================================*/
char* CreateAnswerJson(char *eventName, char *succErrBuf, char *errMsg); //char *outJson










#endif