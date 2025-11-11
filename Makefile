# -g 开启gdb的debug调试（生成的二进制文件中包含调试信息）
# -O0 禁用优化（确保调试一致性）
# -Wall 显示所有警告
CFLAGS := -g -O0 -Wall

log:= thirdparty/log.c thirdparty/log.h
json:=thirdparty/cJSON.c thirdparty/cJSON.h
common:= app/app_common.c app/app_common.h
message:= app/app_message.c app/app_message.h
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
		-gcc  $^ -o $@ -I app -I thirdparty
		-./$@
		-rm $@