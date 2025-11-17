#define _GNU_SOURCE  # 声明使用扩展语法
#include "app_pool.h"
#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "mqueue.h"
#include "log.h"
#include  "stdlib.h"
pthread_t *thread_pool;//管理线程id
mqd_t mq_id=-1;//消息队列id
int poolsize=0;//线程池大小
char* mqueue_name="/test_mq";//消息队列名称
void *thread_fun(void *arg){
    task task_info;
    while (1)
    {
      int len=  mq_receive(mq_id,(char*)&task_info,sizeof(task_info)+100,NULL);
      if (len==sizeof(task_info))
      {
        task_info.task_fun(task_info.arg);
      }
      
    }
    
}
int app_pool_init(int thread_num)
{   struct  mq_attr attr={
        .mq_maxmsg = 10, // 最大消息数
    .mq_msgsize = sizeof(task)+100, // 每条消息的最大字节数
    };
   mq_id =mq_open(mqueue_name, O_CREAT | O_RDWR, 0666, &attr);
   if (mq_id == -1)
   {
    log_error("mq_open error\r\n");
   }
   poolsize=thread_num;
   thread_pool=(pthread_t*)malloc(sizeof(pthread_t)*poolsize);
   for(int i=0;i<poolsize;i++){
    pthread_create(&thread_pool[i],NULL,thread_fun,NULL);
   }
   
   return 0;
}

void app_pool_destroy()
{    
    //关闭删除消息队列
    mq_close(mq_id);
    mq_unlink(mqueue_name);
    //关闭线程
    for(int i=0;i<poolsize;i++){
        pthread_cancel(thread_pool[i]);
    }
    //释放线程池内存
    free(thread_pool);
}

int app_pool_registerTask(int (*task_fun)(void *arg), void *arg)
{
     task task_info={
        .task_fun=task_fun,
        .arg=arg,
     };
     mq_send(mq_id,(char *)&task_info,sizeof(task_info),0);
     return 0;
}
