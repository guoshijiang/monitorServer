/*=========================================================
 *文件名称: 			minitorServer.c
 *创建日期：			2017-7-10
 *作    者:       郭世江
 *修改记录：			2017-7-10  首次创建
 *功能描述：      判断传入的字符串是不是JSON串
 *===========================================================*/
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "jsonCheck.h"
#include "ulaneJsonLog.h"

#ifdef IN
#undef IN
#endif

#define true  1
#define false 0
#define __   -1     

enum classes 
{
  C_SPACE,  
  C_WHITE,  
  C_LCURB,  
  C_RCURB,  
  C_LSQRB,  
  C_RSQRB,  
  C_COLON,  
  C_COMMA,  
  C_QUOTE,  
  C_BACKS,  
  C_SLASH,  
  C_PLUS,   
  C_MINUS,  
  C_POINT,  
  C_ZERO ,  
  C_DIGIT,  
  C_LOW_A,  
  C_LOW_B,  
  C_LOW_C,  
  C_LOW_D,  
  C_LOW_E,  
  C_LOW_F,  
  C_LOW_L,  
  C_LOW_N,  
  C_LOW_R,  
  C_LOW_S,  
  C_LOW_T,  
  C_LOW_U,  
  C_ABCDF,  
  C_E,      
  C_ETC,    
  NR_CLASSES
};

static int ascii_class[128] = 
{
  __,      __,      __,      __,      __,      __,      __,      __,
  __,      C_WHITE, C_WHITE, __,      __,      C_WHITE, __,      __,
  __,      __,      __,      __,      __,      __,      __,      __,
  __,      __,      __,      __,      __,      __,      __,      __,

  C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
  C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
  C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,

  C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,

  C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
  C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
  C_ETC,   C_ETC,   C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
};

enum states {
  GO,  
  OK,  
  OB,  
  KE,  
  CO,  
  VA,  
  AR,  
  ST,  
  ES,  
  U1,  
  U2,  
  U3,  
  U4,  
  MI,  
  ZE,  
  IN,  
  FR,  
  E1,  
  E2,  
  E3,  
  T1,  
  T2,  
  T3,  
  F1,  
  F2,  
  F3,  
  F4,  
  N1,  
  N2,  
  N3,  
  NR_STATES
};


static int state_transition_table[NR_STATES][NR_CLASSES] = 
{
 {GO,GO,-6,__,-5,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {OB,OB,__,-9,__,__,__,__,ST,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {KE,KE,__,__,__,__,__,__,ST,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {CO,CO,__,__,__,__,-2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {VA,VA,-6,__,-5,__,__,__,ST,__,__,__,MI,__,ZE,IN,__,__,__,__,__,F1,__,N1,__,__,T1,__,__,__,__},
 {AR,AR,-6,__,-5,-7,__,__,ST,__,__,__,MI,__,ZE,IN,__,__,__,__,__,F1,__,N1,__,__,T1,__,__,__,__},
 {ST,__,ST,ST,ST,ST,ST,ST,-4,ES,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST},
 {__,__,__,__,__,__,__,__,ST,ST,ST,__,__,__,__,__,__,ST,__,__,__,ST,__,ST,ST,__,ST,U1,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U2,U2,U2,U2,U2,U2,U2,U2,__,__,__,__,__,__,U2,U2,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U3,U3,U3,U3,U3,U3,U3,U3,__,__,__,__,__,__,U3,U3,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U4,U4,U4,U4,U4,U4,U4,U4,__,__,__,__,__,__,U4,U4,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,ST,ST,ST,ST,ST,ST,ST,ST,__,__,__,__,__,__,ST,ST,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,ZE,IN,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,FR,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,FR,IN,IN,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__},
 {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,FR,FR,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__},
 {__,__,__,__,__,__,__,__,__,__,__,E2,E2,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T2,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T3,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F2,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F3,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F4,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N2,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N3,__,__,__,__,__,__,__,__},
 {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__},
};

enum modes 
{
  MODE_ARRAY, 
  MODE_DONE,  
  MODE_KEY,   
  MODE_OBJECT,
};

static int reject(JSON_checker jc)
{
  free((void*)jc->stack);
  free((void*)jc);
  return false;
}


static int push(JSON_checker jc, int mode)
{
  jc->top += 1;
  if (jc->top >= jc->depth)
  {
      return false;
  }
  jc->stack[jc->top] = mode;
  return true;
}

static int pop(JSON_checker jc, int mode)
{
  if (jc->top < 0 || jc->stack[jc->top] != mode) 
  {
    return false;
  }
  jc->top -= 1;
  return true;
}

JSON_checker new_JSON_checker(int depth)
{
  JSON_checker jc = (JSON_checker)malloc(sizeof(struct JSON_checker_struct));
  jc->state = GO;
  jc->depth = depth;
  jc->top = -1;
  jc->stack = (int*)calloc(depth, sizeof(int));
  push(jc, MODE_DONE);
  return jc;
}


int JSON_checker_char(JSON_checker jc, int next_char)
{
  int next_class, next_state;
  if (next_char < 0) 
  {
      return reject(jc);
  }
  if (next_char >= 128) 
  {
    next_class = C_ETC;
  }
  else 
  {
    next_class = ascii_class[next_char];
    if (next_class <= __) 
    {
      return reject(jc);
    }
  }
  next_state = state_transition_table[jc->state][next_class];
  if (next_state >= 0) 
  {

  	jc->state = next_state;
  } 
  else 
  {
		switch (next_state)
		{
			case -9:
		    if (!pop(jc, MODE_KEY)) 
		    {
		        return reject(jc);
		    }
		    jc->state = OK;
		    break;
			
			case -8:
		    if (!pop(jc, MODE_OBJECT)) 
		    {
		        return reject(jc);
		    }
		    jc->state = OK;
		    break;
			
			case -7:
		    if (!pop(jc, MODE_ARRAY)) 
		    {
		        return reject(jc);
		    }
		    jc->state = OK;
		    break;
			
			case -6:
		    if (!push(jc, MODE_KEY)) 
		    {
		        return reject(jc);
		    }
		    jc->state = OB;
		    break;
			
			case -5:
		    if (!push(jc, MODE_ARRAY)) 
		    {
		        return reject(jc);
		    }
		    jc->state = AR;
		    break;
			
			case -4:
		    switch (jc->stack[jc->top]) 
		   	{
			    case MODE_KEY:
		        jc->state = CO;
		        break;
			    case MODE_ARRAY:
			    case MODE_OBJECT:
		        jc->state = OK;
		        break;
			    default:
			      return reject(jc);
		    }
		    break;
			case -3:
		    switch (jc->stack[jc->top]) 
		    {
		    	case MODE_OBJECT:
		        if (!pop(jc, MODE_OBJECT) || !push(jc, MODE_KEY)) 
		        {
		          return reject(jc);
		        }
		        jc->state = KE;
		        break;
		    	case MODE_ARRAY:
		        jc->state = VA;
		        break;
		    	default:
		        return reject(jc);
		    }
		    break;
			case -2:			
		    if (!pop(jc, MODE_KEY) || !push(jc, MODE_OBJECT)) 
		    {
		        return reject(jc);
		    }
		    jc->state = VA;
		    break;

     default:
       return reject(jc);
    }
  }
    return true;
}


int JSON_checker_done(JSON_checker jc)
{
  int result = jc->state == OK && pop(jc, MODE_DONE);
  reject(jc);
  return result;
}

int json_checker(const char *json_str) 
{
  JSON_checker jc = new_JSON_checker(20);
	int len = strlen(json_str);
	int tmp_i = 0;
  for (tmp_i = 0; tmp_i < len; tmp_i++) 
	{
    int next_char = json_str[tmp_i];
    if (next_char <= 0) 
		{
      break;
    }
    if(0 == JSON_checker_char(jc, next_char)) 
		{
			Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 0,"JSON_checker_char: syntax error");
      fprintf(stderr, "\n");
			return -1;   
    }
  }
  if (0 == JSON_checker_done(jc)) 
	{
		Ulane_JsonWriteLog(__FILE__, __LINE__, Json_LogLevel[2], 0,"JSON_checker_end: syntax error");
		return -1;
  }
	return 0;
}