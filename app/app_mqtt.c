#include "app_mqtt.h"

static MQTTClient client;                                                           // mqtt客户端句柄
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer; // mqtt连接参数
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static int (*mqtt_recv_callback)(char *json) = NULL;

static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("消息发送完成");
}

static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int result = 0; // 0: 失败，1：成功
    if (mqtt_recv_callback != NULL)
    {
        result = mqtt_recv_callback(message->payload) == 0 ? 1 : 0;
}

    // 释放内存
    MQTTClient_freeMessage(&message);
MQTTClient_free(topicName);

    return result;
}

static void connlost(void *context, char *cause)
{
    log_debug("Connection lost");
    log_debug("cause: %s", cause);
}

int app_mqtt_init(void)
{
    // 清空mqtt的消息队列
    //  创建mqtt客户端
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_debug("MQTT客户端创建失败");
        return -1;
}

    // 回调函数注册
    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS)
    {
        log_debug("MQTT回调函数注册失败");
        MQTTClient_destroy(&client);
}

    // 连接mqtt服务器
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS)
    {
        log_debug("MQTT连接失败");
        MQTTClient_destroy(&client);
        return -1;
    }
    // 订阅主题
    if (MQTTClient_subscribe(client, TOPIC_PULL, QOS) != MQTTCLIENT_SUCCESS)
    {
        log_debug("MQTT订阅主题失败");
        MQTTClient_disconnect(client, TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
}

    log_debug("MQTT初始化成功");
    return 0;
}

void app_mqtt_close(void)
{
    MQTTClient_unsubscribe(client, TOPIC_PULL);
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

int app_mqtt_send(char *json)
{
    pubmsg.payload = json;
    pubmsg.payloadlen = (int)strlen(json);
    pubmsg.qos = QOS;
    if (MQTTClient_publishMessage(client, TOPIC_PUSH, &pubmsg, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_debug("MQTT发布消息失败");
        return -1;
    }
    return 0;
}

void app_mqtt_registerRecvCallback(int callback(char *json))
{
    mqtt_recv_callback = callback;
}
