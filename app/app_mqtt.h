#ifndef __app_mqtt_H
#define __app_mqtt_H

#include "MQTTClient.h"
#include "unistd.h"
#include "log.h"
#include "string.h"

#define ADDRESS "tcp://192.168.60.1:1883"
#define CLIENTID "b253ba38-daf6-4b37-984f-5d8fdc6a1cfb"
#define TOPIC_PULL "console_to_gateway" // 订阅的主题
#define TOPIC_PUSH "gateway_to_console" // 发送消息的主题
#define QOS 1 // 至少一次
#define TIMEOUT 10000L

/**
 * @brief 初始化mqtt客户端
 */
int app_mqtt_init();

/**
 * @brief 关闭mqtt客户端
 */
void app_mqtt_close();

/**
 * @brief 发送消息
 */
int app_mqtt_send(char *json);

/**
 * @brief 注册接收处理接收到的消息的回调函数
 */
void app_mqtt_registerRecvCallback(int callback(char *json));

#endif // __APP_MQTT_H
