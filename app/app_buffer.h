#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H__
#include <pthread.h>    
typedef struct  {
    unsigned char* ptr;//缓冲区指针
    int size;//缓冲区大小
    int len;//存储数据长度
}SUB_BUFFER;

typedef struct 
{
    SUB_BUFFER *sub[2];//子缓冲区指针
    int read_index;//读指针
    int write_index;//写指针
    pthread_mutex_t read;//读锁
    pthread_mutex_t write;//写锁
}BUFFER;
BUFFER* App_Buffer_Init(int size);
void App_Buffer_destory(BUFFER *buf);
int App_Buffer_Write(BUFFER *buf,char *data,int len);   
int App_Buffer_Read(BUFFER *buf,char *data_buff,int buffer_size);

#endif /* __APP_BUFFER_H__ */