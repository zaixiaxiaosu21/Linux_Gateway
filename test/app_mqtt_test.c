#include "app_mqtt.h"
int callback(char *json){
    printf("rec_callback:%s\n",json);
    return 0;
}
int main(int argc, char const *argv[])
{
  app_mqtt_init();
  app_mqtt_registerRecvCallback(callback);
  app_mqtt_send("{\"device_id\":1,\"cur_angle\":3600,\"motor_status\":\"on\"}");
  sleep(100);
  app_mqtt_close();
}
