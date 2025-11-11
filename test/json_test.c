#include "cJSON.h"
#include "log.h"
#include <stdlib.h>
int main(){

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "zhangsan");
    cJSON_AddNumberToObject(root, "age", 20);
    char *json_str = cJSON_Print(root);
    log_debug("json_str: %s");
   cJSON *root2 =cJSON_Parse(json_str);
   if(root2 == NULL){
       log_error("parse json error");
       return -1;
   }
   char *name= cJSON_GetObjectItem(root2, "name")->valuestring;
   double age = cJSON_GetObjectItem(root2, "age")->valuedouble;
   log_debug("name: %s, age: %lf", name, age);
    cJSON_Delete(root);
    cJSON_Delete(root2);
    free(json_str);
}