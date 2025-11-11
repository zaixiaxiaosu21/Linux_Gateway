#ifndef __APP_MESSAGE_H__
#define __APP_MESSAGE_H__
#include "stdint.h"
#include "cJSON.h"
#include  "log.h"
#include "string.h"
/**
 * @brief 字节数组转json
 * 
 * @param bytes 
 * @param data_len 
 * @return char* 
 */
char *app_message_bytesToJson(uint8_t *bytes, int data_len);
/**
 * @brief json转字节数组
 * 
 * @param json 
 * @param bytes_buf 
 * @param buf_size 
 * @return int 
 */
int app_message_jsonToBytes(char *json, uint8_t *bytes_buf, int buf_size);
#endif /* __APP_MESSAGE_H__ */