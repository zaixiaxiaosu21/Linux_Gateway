#include "app_modbus.h"
#include <string.h>

int main(int argc, char const *argv[])
{
     // 初始化modbus模块
    app_modbus_init("/dev/pts/5");

    // 写线圈  '\x01\x0F\x00\x02\x00\x01\x35\xCB'
    uint8_t coil_data = 1;
    app_modbus_writeCoil(1, 2, 1, &coil_data);

    // 读线圈 '\x01\x01\x01\x01\x90\x48'
    uint8_t coil_data_buf;
    app_modbus_readCoil(1, 2, 1, &coil_data_buf);
    log_debug("读取线圈数据：%d", coil_data_buf);

    // 写保持寄存器 '\x01\x10\x00\x00\x00\x04\xC1\xCA'
    // target_angle 3600 max_speed 60
    float target_angle = 3600;
    float max_speed = 60;
    uint16_t reg_data[4];
    memcpy(reg_data, &target_angle, 4);
    memcpy(reg_data+2, &max_speed, 4);

    app_modbus_writeHoldReg(1, 0, 4, reg_data);

    // 读输入寄存器 '\x01\x04\x06\x00\x00\x45\x61\x00\x01\xE5\x81'
    uint16_t reg_data_buf[3]; // x00\x00\x45\x61\x00\x01
    app_modbus_readInputReg(1, 0, 3, reg_data_buf);
    float cur_angle;
    uint8_t motor_status;
    memcpy(&cur_angle, reg_data_buf, 4);
    // motor_status = (uint8_t)reg_data_buf[2];
    memcpy(&motor_status, reg_data_buf+2, 1);

    log_debug("读取角度：%f, 电机状态：%d", cur_angle, motor_status);


    return 0;
}
