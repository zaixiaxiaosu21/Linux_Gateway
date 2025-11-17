#include "ota_http.h"

int main(int argc, char const *argv[]){
    
  char *json=  ota_http_getJson(OTA_FILEINFO_URL);
  
      log_debug("json: %s",json);
  
    int res=  ota_http_download(OTA_DOWNLOAD_URL,OTA_LOCAL_FILE);
    if(res == 0){
        log_debug("download success");
    }
    return 0;
}
