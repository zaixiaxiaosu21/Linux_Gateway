#include "daemon_runner.h"
#include "app_runner.h"
#include "ota_version.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
       log_error("必须携带参数：app|ota|daemon|version");
        return -1;
    }
    else if (strcmp(argv[1], "app") == 0)
    {
        app_runner_run();
    }
    else if (strcmp(argv[1], "ota") == 0)
    {
        ota_version_checkDailyUpdate();
    }
    else if (strcmp(argv[1], "daemon") == 0){
        daemon_runner_run();
    }
    else if (strcmp(argv[1], "version")==0)
    {
        ota_version_printVersion();
    }else{
        log_error("参数只能是：app|ota|daemon|version");
        return -1;
    }
    return 0;

    
    
    
    
}

/*
./gateway app
./gateway ota
./gateway daemon
*/