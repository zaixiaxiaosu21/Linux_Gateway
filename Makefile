# -g 开启gdb的debug调试（生成的二进制文件中包含调试信息）
# -O0 禁用优化（确保调试一致性）
# -Wall 显示所有警告
CFLAGS := -g -O0 -Wall

log:= thirdparty/log.c thirdparty/log.h
json:=thirdparty/cJSON.c thirdparty/cJSON.h
common:= app/app_common.c app/app_common.h
message:= app/app_message.c app/app_message.h
pool:= app/app_pool.c app/app_pool.h
buffer:= app/app_buffer.c app/app_buffer.h
app_modbus := app/app_modbus.h app/app_modbus.c
app_device := app/app_device.h app/app_device.c
logtest:test/logtest.c $(log)
		-gcc $(CFLAGS) $^ -o $@ -I thirdparty
#		-./$@
#		-rm $@ 
json_test:test/json_test.c $(log) $(json)
		-gcc $^ -o $@ -I thirdparty
		-./$@
		-rm $@
app_common_test:test/app_common_test.c $(common) $(log) 
		-gcc $^ -o $@ -I app -I thirdparty
		-./$@
		-rm $@
app_message_test:test/app_message_test.c $(log) $(json) $(message)
		-gcc $(CFLAGS) $^ -o $@ -I app -I thirdparty
#		-./$@
#		-rm $@
app_mqtt := app/app_mqtt.h app/app_mqtt.c
app_mqtt_test: test/app_mqtt_test.c $(app_mqtt) $(log)
	-gcc $^ -o $@ -Ithirdparty -Iapp -lpaho-mqtt3c
	-./$@
	-rm $@
app_pool_test: test/app_pool_test.c $(log) $(pool)	
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@
app_buffer_test: test/app_buffer_test.c $(log) $(buffer)
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@
app_modbus_test: test/app_modbus_test.c $(app_modbus) $(log)
	-$(CC) $(CFLAGS) $^ -o $@ -Ithirdparty -Iapp -lmodbus
	./$@1
	-rm $@
device_objs:=$(app_device) $(log) $(json) $(buffer)\
	$(app_modbus) $(pool) $(app_mqtt) $(message) 
app_device_test: test/app_device_test.c $(device_objs)
	-$(CC) $(CFLAGS) $^ -o $@ -Ithirdparty -Iapp -lmodbus -lpaho-mqtt3c
	-./$@
	-rm $@
