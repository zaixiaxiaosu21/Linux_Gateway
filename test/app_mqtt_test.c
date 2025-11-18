#define _GNU_SOURCE
#include "app_mqtt.h"
#include <unistd.h>

int receive_callback(char *json) {
    log_debug("接收到json: %s", json);

    return 0;
}

int main(int argc, char const *argv[])
{
    log_debug("main线程（%d）执行", gettid());
    // 初始化mqtt模块
    app_mqtt_init();
    // 注册回调函数
    app_mqtt_registerCallback(receive_callback);

    // 发送消息
    app_mqtt_send("{\"device_id\":1,\"cur_angle\":3600,\"motor_status\":\"on\"}");

    // 休眠100秒  =》用于测试接收远程发送的消息
    sleep(100);

    return 0;
}
