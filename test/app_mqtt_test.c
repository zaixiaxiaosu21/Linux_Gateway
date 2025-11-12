#include "app_mqtt.h"

int handle_callback(char *json)
{
    log_debug("处理接收数据：%s", json);
    return 0;
}

int main(int argc, char const *argv[])
{
    // 初始化
    app_mqtt_init();
    // 注册接收的回调函数
    app_mqtt_registerRecvCallback(handle_callback);
    // 发送数据
    app_mqtt_send("{\"name\":\"tom\", \"age\":18}");

    // 休眠100秒，让当前运行的程序不立即结束 =》 为了能接受消息
sleep(100);

    // 关闭
app_mqtt_close();

    return 0;
}
