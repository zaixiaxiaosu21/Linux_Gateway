#include "app_common.h"
#include  "log.h"

int main(void){
   long time= app_common_getCurrentTime();
   log_info("Current time is %ld", time);
}