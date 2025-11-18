#ifndef __DAEMON_SUB_PROCESS_H__
#define __DAEMON_SUB_PROCESS_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <signal.h>
#include "log.h"

#define MAX_FAIL_COUNT 5
typedef struct{
    pid_t pid;//子进程id
    char *cmd_parm;//启动子进程命令参数 ota|app
    int fail_count;//子进程失败次数
}SubProcess;
/**
 * @brief 子进程初始化
 * 
 * @param cmd_parm 
 */
SubProcess* daemon_sub_process_init(char *cmd_parm);

/**
 * @brief 子进程启动
 * 
 */
int daemon_sub_process_checkstart(SubProcess *sp);

/**
 * @brief 停止子进程
 * 
 */
void daemon_sub_process_stop(SubProcess *sp);







#endif /* __DAEMON_SUB_PROCESS_H__ */