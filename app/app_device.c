#define _GNU_SOURCE
#include "app_device.h"
#include <unistd.h>

static BUFFER *up_buffer;     // 上行缓冲区
static BUFFER *down_buffer;   // 下行缓冲区
static pthread_t read_thread; // 读取下位机数据的线程标识

static int device_ids[DEVICE_SIZE]; // 设备id列表
static int device_num = 0;          // 当前存储设备的数量

// mqtt pool upbuffer downbuffer modbus
int app_device_init(char *filename)
{
    up_buffer = App_Buffer_Init(BUFFER_SIZE);
    down_buffer = App_Buffer_Init(BUFFER_SIZE);
    app_modbus_init(filename);
    app_mqtt_init();
    app_pool_init(POOL_SIZE);
    return 0;
}

// 将设备id添加到设备列表中
static void add_device_id(int device_id)
{ 
     // 遍历设备列表，如果已存在不需要添加
    for (int i = 0; i < device_num; i++) {
        if (device_ids[i] == device_id) {
            return;
        }
    }

    // 添加到设备列表中
    device_ids[device_num++] = device_id;
    
    // 显示设备id列表
    printf("device id list: ");
    for (int i = 0; i < device_num; i++) {
        printf("%d ", device_ids[i]);
    }
    printf("\n");
}

// 将数据写给下位机的任务函数，由线程池中某个线程执行
static int write_task_func(void *arg)
{          
    // 从下行缓冲区读取一个字节数组消息
    char data_buf[128];  // device_id/target_angle/max_speed
    int len = App_Buffer_Read(down_buffer, data_buf, sizeof(data_buf));

    // 根据字节数组生成modbus寄存器数据
    int device_id = data_buf[0];
    uint16_t reg_data[(len-1)/2];
    memcpy(reg_data, data_buf+1, len-1);

    // 写多个保持寄存器（将数据发给下游设备）
    app_modbus_writeHoldReg(device_id, 0, (len-1)/2, reg_data);

    // 将设备id添加到设备列表中
    add_device_id(device_id);

    return 0;
}

// 接收远程消息的回调函数
static int receive_msg_callback(char *json)
{ 
    uint8_t bytes_buff[128]={0};
    log_info("收到MQTT消息: %s", json);
    int len= app_message_jsonToBytes(json,bytes_buff,sizeof(bytes_buff));
    
    // 检查JSON解析是否成功
    if (len < 0) {
        log_error("JSON解析失败或缓冲区大小不足");
        return -1;
    }
    
    // 将数据写入正确的缓冲区
    App_Buffer_Write(down_buffer,(char *)bytes_buff,len);
    log_info("数据已写入down_buffer，长度: %d", len);
    app_pool_registerTask(write_task_func,NULL);
    return 0;
}

static void remove_device_id(int device_id)
{
       // 遍历设备列表，如果存在则删除
    for (int i = 0; i < device_num; i++)
    {
        if (device_ids[i] == device_id) {
           memmove(device_ids + i, device_ids + i + 1, (device_num - i - 1) * sizeof(int));
            device_num--;
            break;
        }
    }

    // 显示设备id列表
    printf("device id list: ");
    for (int i = 0; i < device_num; i++) {
        printf("%d ", device_ids[i]);
    }
    printf("\n");
}

// 发送消息到远程的任务函数
static int send_task_func(void *arg)
{
    char data_buf[128];  // device_id/cur_angle/motor_status
    int len = App_Buffer_Read(up_buffer, data_buf, sizeof(data_buf));
    
    // 检查读取是否成功，缓冲区为空时直接返回
    if (len < 0) {
        log_info("缓冲区没有数据，任务结束");
        return -1;
    }
   
    char* json= app_message_bytesToJson((uint8_t *)data_buf,len);
    app_mqtt_send(json);

    // 如果当前设备的状态是已停止0，则从设备列表中删除
    int device_id = data_buf[0];
    uint8_t status;
    memcpy(&status, data_buf+5, 1);
    if (status == 0) {
        remove_device_id(device_id);
    }
    return 0;
    
    

}

// 从指定id的下位机读取数据
static int read_device_data(int device_id)
{    
    // 读取多个输入寄存器数据
    uint16_t reg_data_buf[3];
    app_modbus_readInputReg(device_id, 0, 3, reg_data_buf);
    uint8_t datas_buff[7]={0};
    datas_buff[0]=device_id;
    memcpy(datas_buff+1,reg_data_buf,6);
    // 数据应该写入up_buffer（上行缓冲区），而不是down_buffer
    App_Buffer_Write(up_buffer,(char *)datas_buff,7);
    app_pool_registerTask(send_task_func,NULL);
    return 0;
}

// 不断读取所有下位机数据的线程函数
static void *read_thread_func(void *arg)
{
     while(1) {
        for (int i = 0; i < device_num; i++) {
            int device_id = device_ids[i];
            read_device_data(device_id);
            usleep(5*1000); // 两个设备之间间隔5ms
        }
        sleep(1); // 每隔1秒遍历一次
    }


    return NULL;
}

/*
 *   注册接收到远程消息的回调函数
 *   启动读取所有下位机的线程
 */
void app_device_start(void)
{
    
    app_mqtt_registerRecvCallback(receive_msg_callback);
    
    pthread_create(&read_thread, NULL, read_thread_func, NULL);
   
   

}

// mqtt pool upbuffer downbuffer modbus
void app_device_free()
{
    app_mqtt_close();
    app_pool_destroy();
    App_Buffer_destory(up_buffer);
    App_Buffer_destory(down_buffer);
    app_modbus_free();
    pthread_cancel(read_thread);
    pthread_join(read_thread, NULL);
}
