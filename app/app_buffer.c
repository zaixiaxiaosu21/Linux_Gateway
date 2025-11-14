#include "app_buffer.h"
#include "log.h"
#include <stdlib.h>
#include "string.h"
static SUB_BUFFER* sub_buffer_init(int size){
      SUB_BUFFER *buf = (SUB_BUFFER *)malloc(sizeof(SUB_BUFFER));
      buf->size = size;
      buf->ptr = (char *)malloc(size);
      buf->len=0;
      return  buf;
}
BUFFER* App_Buffer_Init( int size)
{ 
       BUFFER *buffer = (BUFFER *)malloc(sizeof(BUFFER));//分配内存空间
       buffer->sub[0] = sub_buffer_init(size);
       buffer->sub[1] = sub_buffer_init(size);
       
       buffer->read_index=0;
       buffer->write_index=1;
       //初始化读写锁
       pthread_mutex_init(&buffer->read,NULL);
       pthread_mutex_init(&buffer->write,NULL);
       return buffer;

}

void App_Buffer_destory(BUFFER *buf)
{    
      free(buf->sub[0]->ptr);
      free(buf->sub[0]);
      free(buf->sub[1]->ptr);
      free(buf->sub[1]);
      free(buf);
}

int App_Buffer_Write(BUFFER *buf, char *data, int len)
{
    if (len>255)
    {
      log_error("数据长度过长");
      return -1;  
    }
    pthread_mutex_lock(&buf->write);
    SUB_BUFFER *subbuff= buf->sub[buf->write_index];
    if (subbuff->size-subbuff->len<len+1)
    {
        log_error("空间不足");
        //开锁
        pthread_mutex_unlock(&buf->write);
        return -1;
    }
    subbuff->ptr[subbuff->len]=len;//此位存放数据长度
    memcpy(subbuff->ptr+subbuff->len+1,data,len);//复制数据
    
    subbuff->len+=len+1;
    pthread_mutex_unlock(&buf->write);

    return 0;
}

void swap(BUFFER *buf){
    //进入写锁
    pthread_mutex_lock(&buf->write);
    buf->read_index = buf->read_index^ buf->write_index ;
    buf->write_index= buf->read_index^ buf->write_index ;
     buf->read_index = buf->read_index^ buf->write_index ;
    //退出写锁
    pthread_mutex_unlock(&buf->write);

}
int App_Buffer_Read(BUFFER *buf, char *data_buff, int buffer_size)
{   
    pthread_mutex_lock(&buf->read) ;
    SUB_BUFFER *subbuff= buf->sub[buf->read_index];
    if (subbuff->len==0){
        //交换读写区域
        swap(buf);
        subbuff= buf->sub[buf->read_index];
        if (subbuff->len==0){
            log_error("没有数据");
            pthread_mutex_unlock(&buf->read);
            return -1;
        }
    }
    int len = subbuff->ptr[0];
    if (len>buffer_size){
        log_error("空间不足");
        pthread_mutex_unlock(&buf->read);
        return -1;
    }
    memcpy(data_buff,subbuff->ptr+1,len);
    memmove(subbuff->ptr,subbuff->ptr+len+1,subbuff->size-len-1);
    subbuff->len-=len+1;
    pthread_mutex_unlock(&buf->read);
    return len;
}
