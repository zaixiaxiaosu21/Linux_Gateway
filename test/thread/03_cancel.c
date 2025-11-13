#define _GNU_SOURCE  # 声明使用扩展语法
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void *task(void *arg){
    printf("task thread id: %d\n",gettid());
    printf("task线程工作中...\n");
    sleep(3);
    printf("task线程sleep之后执行结束\n");
    return NULL;
}

int main(int argc, char const *argv[]){
    printf("main thread id: %d\n",gettid());
    pthread_t qwq;
    pthread_create(&qwq,NULL,task,NULL);
    sleep(1);
    pthread_cancel(qwq);
    void *res;
    pthread_join(qwq,&res);
    if (res == PTHREAD_CANCELED)
    {
        printf("线程被取消\n");
    }
    else{
        printf("线程正常结束\n");
    }
    
    printf("main线程（%d）即将结束 \n", gettid());

    return 0;
}