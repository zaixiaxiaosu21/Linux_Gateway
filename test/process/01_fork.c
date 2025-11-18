#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    printf("父(main)进程(%d)开始执行 \n", getpid());
    int val = 123;
    pid_t pid = fork();
    if (pid == -1) {
        printf("fork失败\n");
    }
    if (pid == 0) {
        val++;
        printf("子进程(%d)开始执行 \n", getpid());
        printf("子进程(%d)的val = %d\n", getpid(), val);
        printf("父进程为%d\n",getppid());
        
    }else{
        val--;
        printf("父进程(%d)的val = %d,子进程为%d\n", getpid(), val,pid);
        wait(0);
    }
       printf("进程(%d)执行结束 \n", getpid());
    return 0;
}
