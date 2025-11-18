#ifndef __app_mqtt_H
#define __app_mqtt_H

#include "MQTTClient.h"
#include "unistd.h"
#include "log.h"
#include "string.h"

#define ADDRESS "tcp://192.168.42.192:1883"
#define CLIENTID "b253ba38-daf6-4b37-984f-5d8fdc6a1cfqw"
#define TOPIC_GTR "gateway_to_console" // 要发送消息的主题
#define TOPIC_RTG "console_to_gateway" // 要订阅的主题
#define QOS 1
#define TIMEOUT 10000L

/**
 * @brief 初始化MQTT模块
 * 
 * @return int 0：成功，-1：失败
 */
int app_mqtt_init(void);

/**
 * @brief 发送消息
 * 
 * @param json 消息数据
 * @return int  0：成功，-1：失败
 */
int app_mqtt_send(char *json);

/**
 * @brief 关闭MQTT模块
 * 
 */
void app_mqtt_close(void);

/**
 * @brief 注册回调函数来处理接收的json数据
 * 
 * @param callback 
 */
void app_mqtt_registerCallback(int (*callback)(char *json));

#endif // __APP_MQTT_H
