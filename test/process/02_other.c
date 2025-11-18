#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("other进程（%d）开始执行 argv[1]=%s,argv[2]=%s\n", getpid(), argv[1], argv[2]);

    return 0;
}
