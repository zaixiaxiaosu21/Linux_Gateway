#define _GNU_SOURCE
#include "app_mqtt.h"
#include <unistd.h>

static MQTTClient client; // MQTT客户端句柄
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer; // 连接选项
static MQTTClient_message pubmsg = MQTTClient_message_initializer; // 消息对象
static int (*receive_callback)(char *) = NULL;

// 发送消息完成的回调
static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("发送消息完成");
}

// 收到消息的回调
// 此函数返回值为1表示消息处理成功，返回0表示消息处理失败
static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    log_debug("接收到远程消息（%d）", gettid());
    int result = 0; // 默认消息处理失败/未处理
    if (receive_callback) {
        result = receive_callback((char *)message->payload)==0 ? 1 : 0;
    }
    // 释放内存
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    // 返回结果
    return result; // 1表示消息处理成功，0表示消息处理失败
}

// 连接意外断开的回调
static void connlost(void *context, char *cause)
{
    log_error("连接意外断开，原因：%s", cause);
}

int app_mqtt_init(void)
{
    // 创建MQTT客户端
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS){
        log_error("创建MQTT客户端失败");
        return -1;
    }
    // 设置回调函数
    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS){
        log_error("设置回调函数失败");
        MQTTClient_destroy(&client);
        return -1;
    }
  
    // 连接服务器
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS){
        log_error("连接服务器失败");
        MQTTClient_destroy(&client);
        return -1;
    }

    // 订阅主题
    if (MQTTClient_subscribe(client, TOPIC_RTG, QOS) != MQTTCLIENT_SUCCESS){
        log_error("订阅主题失败");
        MQTTClient_disconnect(client, TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
    }
    log_debug("MQTT模块初始化成功");

    return 0;
}

int app_mqtt_send(char *json)
{
    pubmsg.payload = json;
    pubmsg.payloadlen = strlen(json);
    pubmsg.qos = QOS;

    if (MQTTClient_publishMessage(client, TOPIC_GTR, &pubmsg, NULL) != MQTTCLIENT_SUCCESS){
        log_error("发送消息失败");
        return -1;
    }

    return 0;
}

void app_mqtt_close(void)
{
    // 取消订阅
    MQTTClient_unsubscribe(client, TOPIC_RTG);
    // 断开连接
    MQTTClient_disconnect(client, TIMEOUT);
    // 销毁客户端
    MQTTClient_destroy(&client);
}

void app_mqtt_registerCallback(int (*callback)(char *json))
{
    receive_callback = callback;
}
