#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    printf("父(main)进程(%d)开始执行 \n", getpid());
    pid_t pid = fork();
    if (pid == -1) {
        printf("fork失败\n");
    }
    if (pid == 0) {

        printf("子进程(%d)开始执行 \n", getpid()); 
        char * path="other";
        char * argv[] = {path,"hello","stefanie",NULL};
        execve(path, argv, NULL);
        printf("execve失败\n");
    }else{
         
        printf("父进程(%d)", getpid());
        wait(0);
    }
       printf("进程(%d)执行结束 \n", getpid());
    return 0;
}
