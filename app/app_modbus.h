// #ifndef __app_modbus_H
// #define __app_modbus_H
// #define _GNU_SOURCE
// #include <modbus/modbus.h>
// #include "log.h"
// #include <errno.h>
// #include <unistd.h>

// /**
//  * @brief 初始化modbus
//  * 
//  * @param filename 串口文件
//  * @return int 0：成功，-1：失败
//  */
// int app_modbus_init(char *filename);

// /**
//  * @brief 释放modbus
//  * 
//  */
// void app_modbus_free(void);


// /**
//  * @brief 写多个保持寄存器
//  * 
//  * @param device_id 从设备地址/id
//  * @param addr 写入的起始地址
//  * @param reg_num 寄存器数量
//  * @param reg_data 要写入寄存器数据
//  * @return int 0:成功，-1：失败
//  */
// int app_modbus_writeHoldReg(int device_id, int addr, int reg_num, uint16_t *reg_data);

// // 读多个输入寄存器
// int app_modbus_readInputReg(int device_id, int addr, int reg_num, uint16_t *reg_data_buf);

// // 写多个线圈
// int app_modbus_writeCoil(int device_id, int addr, int reg_num, uint8_t *coil_data);

// // 读多个线圈
// int app_modbus_readCoil(int device_id, int addr, int reg_num, uint8_t *coil_data_buf);


// #endif /* __app_modbus_H */
#ifndef __app_modbus_H
#define __app_modbus_H
#define _GNU_SOURCE
#include <modbus/modbus.h>
#include "log.h"
#include <errno.h>
#include <unistd.h>

/**
 * @brief 初始化modbus
 * 
 * @param filename 串口文件䠘
 * @return int 0：成功，-1：失败
 */
int app_modbus_init(char *filename);

/**
 * @brief 释放modbus
 * 
 */
void app_modbus_free(void);


/**
 * @brief 写多个保持寄存器
 * 
 * @param device_id 从设备地址/id
 * @param addr 写入的起始地址
 * @param reg_num 寄存器数量
 * @param reg_data 要写入寄存器数据
 * @return int 0:成功，-1：失败
 */
int app_modbus_writeHoldReg(int device_id, int addr, int reg_num, uint16_t *reg_data);

// 读多个输入寄存器
int app_modbus_readInputReg(int device_id, int addr, int reg_num, uint16_t *reg_data_buf);

// 写多个线圈
int app_modbus_writeCoil(int device_id, int addr, int reg_num, uint8_t *coil_data);

// 读多个线圈
int app_modbus_readCoil(int device_id, int addr, int reg_num, uint8_t *coil_data_buf);


#endif /* __app_modbus_H */
