#include "ota_version.h"
#include "ota_http.h"
#include "cJSON.h"
#include "unistd.h"
#include "stdlib.h"
#include "openssl/sha.h"
#include  "string.h"
#include <sys/reboot.h>
/**
 * 获取文件的SHA1哈希值（40位16进制字符串）
 * 相同文件返回相同的哈希值，可用于判断文件是否相同
 * linux命令生成：sha1sum 文件名
 * 此函数可以利用AI工具帮我们生成
 */
static char *get_file_sha(char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return NULL;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);

    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    if (!buffer)
    {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    int bytesRead;
    while ((bytesRead = fread(buffer, 1, bufSize, file)) > 0)
    {
        SHA1_Update(&sha1, buffer, bytesRead);
    }

    SHA1_Final(hash, &sha1);
    fclose(file);
    free(buffer);

    char *outputBuffer = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    if (!outputBuffer)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    return outputBuffer;
}
int ota_version_checkUpdate()
{   
    char *json= ota_http_getJson(OTA_FILEINFO_URL);
    cJSON *root = cJSON_Parse(json);
    int major = cJSON_GetObjectItem(root, "major")->valueint;
    int minor = cJSON_GetObjectItem(root, "minor")->valueint;
    int patch = cJSON_GetObjectItem(root, "patch")->valueint;
     log_debug("--远程版本号：%d.%d.%d", major, minor, patch);
    ota_version_printVersion();
    if ((VERSION_MAJOR>major)||(VERSION_MAJOR==major&&VERSION_MINOR>minor)||(VERSION_MAJOR==major&&VERSION_MINOR==minor&&VERSION_PATCH>=patch))
    {
       log_info("当前版本已为最新,不需要更改");
       //释放资源
       cJSON_Delete(root);
       free(json);
       return 0;
    }
 int res=   ota_http_download(OTA_DOWNLOAD_URL,OTA_LOCAL_FILE);
    if (res==-1){
        log_error("下载失败");
      
       //触发重试机制
        int res2=ota_http_download(OTA_DOWNLOAD_URL,OTA_LOCAL_FILE);
        if (res2==-1){
             cJSON_Delete(root);
             free(json);
            return -1;
        }
    } 
    //下载成功检验程序完整性
      char *remote_hash = cJSON_GetObjectItem(root, "sha1")->valuestring;
      char *local_hash = get_file_sha(OTA_LOCAL_FILE);
      if (strcmp(remote_hash, local_hash) != 0){
        log_error("文件校验失败");
        //释放资源
        cJSON_Delete(root);
        free(json);
        free(local_hash);
        //删除本地文件
        unlink(OTA_LOCAL_FILE);
        return -1;
      }


      //重启系统
      log_info("文件校验成功,重启系统");
      reboot(RB_AUTOBOOT); // 要求必须是root用户
      return 0;
}

void ota_version_printVersion()
{
    log_info("当前版本为:%d.%d.%d",VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH);
}

int ota_version_checkDailyUpdate()
{   
    while (1)
    {
        ota_version_checkUpdate();
        sleep(60*60*24);
    }
    return 0;
}
