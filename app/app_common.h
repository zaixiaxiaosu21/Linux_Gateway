#ifndef __app_common_H
#define __app_common_H

#include <stdlib.h> // NULL
#include <sys/time.h> // timeval / gettimeofday()

/**
 * @brief 获取自1970年开始的时间戳，以ms为单位
 * 
 * @return long 时间戳
 */
long app_common_getCurrentTime();

#endif /* __app_common_H */
