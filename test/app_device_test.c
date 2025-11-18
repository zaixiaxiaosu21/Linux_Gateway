#include "app_device.h"

int main(int argc, char const *argv[])
{

    // 初始化设备
    app_device_init("/dev/pts/5");

    // 启动设备
    app_device_start();

    while (1)
    {
        sleep(1);
    }
    
    app_device_free();
    
    return 0;
}
