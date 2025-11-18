#ifndef __ota_http_H
#define __ota_http_H

#include <curl/curl.h>
#include <string.h>
#include "log.h"
#include <stdlib.h>

#define OTA_FILEINFO_URL "http://192.168.42.192:8000/fileinfo.json"
#define OTA_DOWNLOAD_URL "http://192.168.42.192:8000/download/gateway"
//#define OTA_LOCAL_FILE "/home/stefanie/gateway.update"
#define OTA_LOCAL_FILE "/root/gateway.update"

/**
 * @brief 请求获取json数据
 * 
 * @param url 请求地址
 * @return char* 返回的json
 */
char *ota_http_getJson(char *url);

/**
 * @brief 请求下载文件
 * 
 * @param url 请求地址
 * @param filepath 保存文件的路径
 * @return int 0：成功，-1：失败
 */
int ota_http_download(char *url, char *filepath);


#endif /* __ota_http_H */
