/*=========================================================
 *文件名称:  ThreadPool.h
 *创建日期：2017-6-15
 *作者：郭世江
 *修改记录：
 *2017-6-15  首次创建
 *功能描述 Thread线程池封装
===========================================================*/

#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

typedef struct threadpool_t threadpool_t;

threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);


int threadpool_add(threadpool_t *pool, void*(*function)(void *arg), void *arg);


int threadpool_destroy(threadpool_t *pool);


int threadpool_all_threadnum(threadpool_t *pool);


int threadpool_busy_threadnum(threadpool_t *pool);

#endif
