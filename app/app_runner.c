#include "app_runner.h"
static int isrun=1;
void exit_handler(int signum){
        isrun=0;
}
int main(int argc, char const *argv[])
{
     signal(SIGINT, exit_handler);
     signal(SIGTERM, exit_handler);
    // 初始化设备
    app_device_init(DEVICE_FILE);
    // 启动设备
    app_device_start();
    while (isrun)
    {
        sleep(1);
    }
    app_device_free();
    return 0;
}