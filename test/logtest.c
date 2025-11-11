#include "log.h"

int main(int argc, char *argv[]){
    //设置输出级别
    log_set_level(LOG_ERROR);
    log_debug("This is a debug message");
    log_info("This is an info message");
    log_warn("This is a warning message");
    log_error("This is an error message");
    log_fatal("This is a fatal message");
    return 0;
}