#include "app_modbus.h"

static modbus_t * modbus; // modbus句柄

int app_modbus_init(char *device_path)
{
 modbus= modbus_new_rtu(device_path, 115200, 'N', 8, 1);
  //1.配置调试日志
  modbus_set_debug(modbus, TRUE);
  //2.Rts
  modbus_rtu_set_rts(modbus, MODBUS_RTU_RTS_NONE);
  //3.设置超时时间
  modbus_set_response_timeout(modbus, 1, 0);
  
  if(modbus_connect(modbus) == -1){
    log_error("Modbus connect failed: %s\n", modbus_strerror(errno));
    modbus_free(modbus);
    return -1;
  }
  return 0;
}

int app_modbus_writeCoil(int slave_id, int addr, int reg_count, uint8_t *data)
{ 
   // 刷新清理缓存
  modbus_flush(modbus);
  // 设置从机id
  modbus_set_slave(modbus, slave_id);
  // 写入线圈
  if (modbus_write_bits(modbus, addr, reg_count, data) == -1)
  {
    usleep(5*1000);
    if (modbus_write_bits(modbus, addr, reg_count, data) == -1)
  { 
    log_error("Modbus write coil failed: %s\n", modbus_strerror(errno));
    return -1;
    
  }

  }
  log_debug("写线圈成功");
  return 0;
  
}

int app_modbus_readCoil(int slave_id, int addr, int reg_count, uint8_t *data_buf)
{ 

  // 刷新清理缓存
  modbus_flush(modbus);
  // 设置从机id
  modbus_set_slave(modbus, slave_id);
  
  if (modbus_read_bits(modbus, addr, reg_count, data_buf) == -1){
    usleep(5*1000);
    if (modbus_read_bits(modbus, addr, reg_count, data_buf) == -1){
      log_error("Modbus read coil failed: %s\n", modbus_strerror(errno));
      return -1;
    }
  }
  log_debug("读线圈成功");
  return 0;
  
}

int app_modbus_writeHoldReg(int slave_id, int addr, int reg_count, uint16_t *data)
{
  // 刷新清理缓存
  modbus_flush(modbus);
  // 设置从机id
  modbus_set_slave(modbus, slave_id);
  // 写保持寄存器
  if (modbus_write_registers(modbus, addr, reg_count, data) == -1) {
    // 等待5ms后，再写一次，如果失败，则返回失败
    usleep(5000);
    if (modbus_write_registers(modbus, addr, reg_count, data) == -1) {
      log_error("modbus写保持寄存器失败：%s", modbus_strerror(errno));
      return -1;
    }
  }
log_debug("写保持寄存器成功");

  return 0;
  
}

int app_modbus_readInputReg(int slave_id, int addr, int reg_count, uint16_t *data_buf)
{
   // 刷新清理缓存
  modbus_flush(modbus);
  // 设置从机id
  modbus_set_slave(modbus, slave_id);
  // 读输入寄存器
  if (modbus_read_input_registers(modbus, addr, reg_count, data_buf) == -1) {
    // 等待5ms后，再读一次，如果失败，则返回失败
    usleep(5000);
    if (modbus_read_input_registers(modbus, addr, reg_count, data_buf) == -1) {
      log_error("modbus读输入寄存器失败：%s", modbus_strerror(errno));
      return -1;
    }
}

  return 0;
}

void app_modbus_free(void)
{ 
  //关闭连接
  modbus_close(modbus);
  //释放资源
  modbus_free(modbus);

}