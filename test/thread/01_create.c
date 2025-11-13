#define _GNU_SOURCE  # 声明使用扩展语法
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void *first_thread_func(void *arg){
    printf("first thread id: %d\n",gettid());
}
void *second_thread_func(void *arg){
    printf("second thread id: %d\n",gettid());
}
int main(int argc, char const *argv[]){
    printf("main thread id: %d\n",gettid());
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,first_thread_func,NULL);
    pthread_create(&tid2,NULL,second_thread_func,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    printf("main线程（%d）即将结束 \n", gettid());

    return 0;
}