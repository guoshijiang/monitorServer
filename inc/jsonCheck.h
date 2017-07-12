/*=========================================================
 *文件名称: 			minitorServer.c
 *创建日期：			2017-7-10
 *作    者:       郭世江
 *修改记录：			2017-7-10  首次创建
 *功能描述：      判断传入的字符串是不是JSON串
 *===========================================================*/
#ifndef _JSON_CHECKER_H_
#define _JSON_CHECKER_H_

#include "ulaneJsonLog.h"

typedef struct JSON_checker_struct
{
    int state;
    int depth;
    int top;
    int* stack;
}*JSON_checker;


JSON_checker new_JSON_checker(int depth);
int  JSON_checker_char(JSON_checker jc, int next_char);
int  JSON_checker_done(JSON_checker jc);
int json_checker(const char *json_str);

#endif