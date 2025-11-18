#include "daemon_sub_process.h"

SubProcess* daemon_sub_process_init(char *cmd_parm)
{
    SubProcess *sp = (SubProcess*)malloc(sizeof(SubProcess));
    sp->pid=-1;
    sp->cmd_parm = cmd_parm;
    sp->fail_count=0;
    return sp;
}

int daemon_sub_process_checkstart(SubProcess *sp)
{    
   int status=0;
   if(sp->pid>0&&waitpid(sp->pid,&status,WNOHANG)==0){//子进程正常
      return 0;
   }
   //如果子进程异常结束
   if(status!=0){
     sp->fail_count++;
     if (sp->fail_count>MAX_FAIL_COUNT)
     {
        reboot(RB_AUTOBOOT);
     }
   }
   //如果子进程未启动
   sp->pid=fork();
   if(sp->pid==0){
       char *path = EXCEL_PATH;
     char *argv[] = {EXCEL_PATH, sp->cmd_parm, NULL};
     log_debug("子进程%d正在运行,启动命令:%s",getpid(),sp->cmd_parm);
     execve(EXCEL_PATH, argv, NULL);
     exit(EXIT_FAILURE);
   }
   return 0;
}

void daemon_sub_process_stop(SubProcess *sp)
{   
     kill(sp->pid, SIGTERM);
     waitpid(sp->pid, NULL,0);
     free(sp);
}
