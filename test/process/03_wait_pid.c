#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "stdlib.h"

int main()
{
    printf("父(main)进程(%d)开始执行 \n", getpid());
    pid_t pid = fork();
    if (pid == -1) {
        printf("fork失败\n");
    }
    if (pid == 0) {

        printf("子进程(%d)开始执行 \n", getpid()); 
        char * path="/bin/ping";
        char * argv[] = {path,"www.baidu.com","-c","5",NULL};
        execve(path, argv, NULL);
        printf("execve失败\n");
    }else{
         
        printf("父进程(%d)", getpid());
        int status;
        waitpid(pid, &status, 0); //等待子进程结束
        printf("子进程status = %d \n",status);
    }
       printf("进程(%d)执行结束 \n", getpid());
    exit(0);
}
