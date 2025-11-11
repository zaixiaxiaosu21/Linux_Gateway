#include "app_message.h"
#include <stdlib.h>
#include "log.h"
int main(int argc, char const *argv[])
{
    uint8_t data[]= {0x01, 0x00,0x00, 0x61, 0x45, 0x00, 0x00, 0x70, 0x42};
   char*str= app_message_bytesToJson(data, sizeof(data));
    log_info("str: %s", str);
    free(str);
    char *json2 = "{\"device_id\":1,\"target_angle\": 3600, \"max_speed\": 60}";
    uint8_t bytes_buff[20];
    int datalen= app_message_jsonToBytes(json2, bytes_buff, sizeof(bytes_buff));
    for (uint8_t i = 0; i < datalen; i++)
    {
        printf("%02x", bytes_buff[i]);
    }
    printf("\n");
    return 0;

    
    

}

