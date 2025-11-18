#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
static int isrun=1;
void usesource(void){
    printf("use 资源\n");

}
void freesource(void){
    printf("free 资源\n");
}
void  exit_handler(int signum){
    printf("收到信号 %d\n",signum);
    if (signum == SIGINT)
    {
        freesource();
        exit(EXIT_SUCCESS);
    }else{
        isrun=0;
    }
    
}
int main(int argc, char const *argv[]){
    signal(SIGINT,exit_handler);
    signal(SIGTERM,exit_handler);
    usesource();
    while(isrun){
        sleep(1);
        printf("程序运行中\n");
    }
    freesource();
    return 0;
}