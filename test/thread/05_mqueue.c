#define _GNU_SOURCE  # 声明使用扩展语法
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "mqueue.h"
static  mqd_t mqd=-1; // 全局变量
void *first_thread_func(void *arg){
   // sleep(1);
    printf("开始接收消息\r\n");
    char buf[100];
   int len= mq_receive(mqd,buf,sizeof(buf),NULL);
    printf("接收到的消息是：%*s\r\n",len,buf);
} 
void *second_thread_func(void *arg){
    sleep(3);
    printf("开始发送消息\r\n");
    mq_send(mqd,"hello",sizeof("hello"),0);

}
int main(int argc, char const *argv[]){
    printf("main thread id: %d\n",gettid());
    struct  mq_attr attr={
        .mq_maxmsg = 10, // 最大消息数
    .mq_msgsize = 100, // 每条消息的最大字节数
    };

    mqd= mq_open("/mqtest", O_RDWR|O_CREAT,0644,&attr);
   if (mqd==-1)
   {
    printf("mq_open error\n");
    return -1; 
   }

    // 创建两个线程
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,first_thread_func,NULL);
    pthread_create(&tid2,NULL,second_thread_func,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    printf("main线程（%d）即将结束 \n", gettid());
    mq_close(mqd);
    mq_unlink("/mqtest");
    return 0;
}