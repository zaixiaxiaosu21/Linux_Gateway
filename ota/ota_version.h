#ifndef __OTA_VERSION_H__
#define __OTA_VERSION_H__

#define VERSION_MAJOR 3  // 主版本号
#define VERSION_MINOR 3 // 次版本号
#define VERSION_PATCH 0 // 修订版本号

/**
 * 检查并更新程序
 * 1 网关程序 查询 远端ota服务发布的最新版本号
  2 和本地当前程序的版本号进行比对 如果本地低则需要下载远端应用程序
  3 下远端应用程序
  4 本地根据下载的文件生产一个验证码  (ai生成)
  5 和远端的验证码进行对比 如果相同则认可当前文件
  6 结束当前进程 
 */
int ota_version_checkUpdate();

/**
 * 输出当前版本号
 */
void ota_version_printVersion();

/**
 * 每日检查是否需要更新
 */
int ota_version_checkDailyUpdate();


#endif /* __OTA_VERSION_H__ */