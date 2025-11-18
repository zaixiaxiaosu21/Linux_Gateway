#include "daemon_runner.h"
#include "daemon_sub_process.h"
static SubProcess *sub_process_list[SUB_PROCESS_COUNT];
static char *cmd_params[SUB_PROCESS_COUNT] = {"app", "ota"};
static int isrun=1;
static void exit_handler(int sig){
        isrun=0;
}
int daemon_runner_run(void)
{
        //注册退出信号
        signal(SIGINT, exit_handler);
        signal(SIGTERM, exit_handler);

        for (int i = 0; i < SUB_PROCESS_COUNT; i++) {
                       sub_process_list[i]=daemon_sub_process_init(cmd_params[i]);
        }
        while(isrun){
                for (int i = 0; i < SUB_PROCESS_COUNT; i++) {
                        daemon_sub_process_checkstart(sub_process_list[i]);
                }
                sleep(1);
        }
        for (int i = 0; i < SUB_PROCESS_COUNT; i++)
        {
            daemon_sub_process_stop(sub_process_list[i]);
        }
}