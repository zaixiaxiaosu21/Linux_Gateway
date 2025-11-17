#ifndef __app_device_H
#define __app_device_H

#include "app_mqtt.h"
#include "app_modbus.h"
#include "app_buffer.h"
#include "app_pool.h"
#include "app_message.h"
#include "log.h"

#define POOL_SIZE 5
#define BUFFER_SIZE 1024
#define DEVICE_SIZE 50

/**
 * @brief 初始化设备模块
 *  mqtt pool upbuffer downbuffer modbus
 * 
 * @param filename 
 * @return int 
 */
int app_device_init(char *filename);

/**
 * @brief 启动设备
 *   注册接收到远程消息的回调函数
 *   启动读取所有下游设备的线程
 */
void app_device_start(void);

/**
 * @brief 释放设备模块
 * 
 */
void app_device_free();

#endif /* __app_device_H */
