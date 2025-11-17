#include "ota_http.h"
size_t receive_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    char * res_data =(char *)ptr;
    char *json_buf = (char *)userdata;
     int len = size * nmemb;
    memcpy(json_buf, res_data, len);
    json_buf[len] = '\0';
    return len;
   
}
char *ota_http_getJson(char *url)
{
    //创建CURL对象
    CURL *curl = curl_easy_init();
    //1.配置url
    curl_easy_setopt(curl, CURLOPT_URL, url);
    //2.设置回调
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_callback);
    //3.传入回调函数参数
    char *json_buf = malloc(100);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, json_buf);

    //发送url请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK){
        log_error("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        //释放资源
        free(json_buf);
        curl_easy_cleanup(curl);
        return NULL;

    }
    curl_easy_cleanup(curl);
    return json_buf;
}

int ota_http_download(char *url, char *filepath)
{   
     //创建CURL对象
    CURL *curl = curl_easy_init();
    //1.配置url
    curl_easy_setopt(curl, CURLOPT_URL, url);
    //2.设置回调
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    //3.传入回调函数参数
    FILE *fp = fopen(filepath, "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    //发送url请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK){
        log_error("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        //释放资源
        fclose(fp);
        curl_easy_cleanup(curl);
        return -1;
    }
    fclose(fp);
    curl_easy_cleanup(curl);
    return 0;
}
