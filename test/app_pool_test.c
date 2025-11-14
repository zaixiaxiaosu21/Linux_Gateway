#define _GNU_SOURCE
#include "app_pool.h"
#include "stdio.h"
#include <stdlib.h>  
#include <unistd.h>  
#include "log.h" 
int my_task(void *arg){
    log_debug("线程函数(%d)执行，参数：%s", gettid(), (char *)arg);
}

int main(void){
    app_pool_init(5);
    for (int i = 0; i < 10; i++)
    {
        char * msg=malloc(10);
        sprintf(msg,"msg %d",i);
        app_pool_registerTask(my_task,msg);
    }
    sleep(1);
    app_pool_destroy();
}