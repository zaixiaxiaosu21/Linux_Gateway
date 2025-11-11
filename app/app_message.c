#include "app_message.h"    
char *app_message_bytesToJson(uint8_t *bytes, int data_len){
    uint8_t device_id = bytes[0];
    char *motor_status = bytes[5] == 0 ? "off" : "on";
    float cur_angle = 0.0;
    memcpy(&cur_angle, bytes + 1, 4);
    char cur_angle_str[9];
    sprintf(cur_angle_str, "%8.1f", cur_angle);
    //数据转为json数据
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "device_id", device_id);
    cJSON_AddStringToObject(root, "motor_status", motor_status);
    cJSON_AddStringToObject(root, "cur_angle", cur_angle_str);
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json;
}
int app_message_jsonToBytes(char *json, uint8_t *bytes_buf, int buf_size){
    // 从json中解析提取数据 {"device_id ": 1 ,"target_angle":3600 ,"max_speed":60}
    //  01 00 00 61 45 00 00 70 42
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        return -1;
    }
    uint8_t device_id = cJSON_GetObjectItem(root, "device_id")->valueint;
    float target_angle = cJSON_GetObjectItem(root, "target_angle")->valuedouble;
    float max_speed = cJSON_GetObjectItem(root, "max_speed")->valuedouble;
    if (buf_size < 9) 
    {
       log_error("buf_size is too small");
       return -1;
    }
    bytes_buf[0]=   device_id;
    memcpy(bytes_buf + 1, &target_angle, 4);
    memcpy(bytes_buf + 5, &max_speed, 4);
    cJSON_Delete(root);
    return 9;

}
