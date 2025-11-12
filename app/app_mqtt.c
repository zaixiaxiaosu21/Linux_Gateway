#include "app_mqtt.h"

static MQTTClient client;
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static int (*rec_callback)(char *json)=NULL;
// 发送消息完成的回调
void delivered(void *context, MQTTClient_deliveryToken dt)
{
   log_info("发送消息成功");
}

// 收到消息的回调
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{   
    int res=0;//1表示消息处理成功，0表示消息处理失败
    if (rec_callback)
    {
        res=rec_callback((char *)message->payload)==0?1:0;
    }
    
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;//1表示消息处理成功，0表示消息处理失败
}

// 连接意外断开的回调
void connlost(void *context, char *cause)
{
    log_info("连接意外断开,原因：%s", cause);
}
int app_mqtt_init()
{   
    //创建客户端
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS){
        log_error("MQTTClient_create error");
        return -1;
    }
    //设置回调函数
    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS){
       log_error("MQTTClient_setCallbacks error");
       MQTTClient_destroy(&client);
       return -1;
    }
    //建立连接
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS){
        log_error("MQTTClient_connect error");
        MQTTClient_destroy(&client);
        return -1;
    }
    //订阅
    if (MQTTClient_subscribe(client, TOPIC_PULL, QOS) != MQTTCLIENT_SUCCESS){
        log_error("MQTTClient_subscribe error");
        MQTTClient_disconnect(client,TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
    }
}

void app_mqtt_close()
{   
    MQTTClient_unsubscribe(client, TOPIC_PULL);
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

int app_mqtt_send(char *json)
{   pubmsg.payload = json;
    pubmsg.payloadlen = strlen(json);
    pubmsg.qos = QOS;
    MQTTClient_publishMessage(client, TOPIC_PUSH, &pubmsg, NULL);
    return 0;
}

void app_mqtt_registerRecvCallback(int callback(char *json))
{   
    rec_callback = callback;
}
