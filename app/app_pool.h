#ifndef __APP_POOL_H__
#define __APP_POOL_H__
//任务结构体
typedef struct _task_t{
  int (*task_fun)(void *arg);
  void *arg;
}task;

// 线程池模块初始化
int app_pool_init(int thread_num);
    
// 线程池模块销毁
void app_pool_destroy();

// 注册待执行的任务
int app_pool_registerTask(int (*task_fun)(void *arg), void *arg);
#endif /* __APP_POOL_H__ */