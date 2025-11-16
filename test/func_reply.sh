#!/bin/bash

# 修正版Modbus模拟器脚本（无默认应答）
while true; do
    # 使用dd读取一个字节，这样更可靠
    byte=$(dd bs=1 count=1 2>/dev/null | od -An -tx1 | tr -d ' ')
    
    if [ ! -z "$byte" ]; then
        # 读取设备地址（第一个字节）
        device_addr="$byte"
        
        # 读取功能码（第二个字节）
        function_code=$(dd bs=1 count=1 2>/dev/null | od -An -tx1 | tr -d ' ')
        
        if [ ! -z "$function_code" ]; then
            # 读取剩余字节（简化处理，读取固定9个字节来完成完整帧）
            remaining=$(dd bs=9 count=1 2>/dev/null)
            
            # 根据功能码返回相应的应答（仅在匹配时）
            case "$function_code" in
                "10")
                    # 功能码16（0x10）- 写多个保持寄存器的应答
                    echo -ne '\x01\x10\x00\x00\x00\x04\xC1\xCA'
                    ;;
                "0f")
                    # 功能码15（0x0F）- 写多个线圈的应答
                    echo -ne '\x01\x0F\x00\x02\x00\x01\x35\xCB'
                    ;;
                "03")
                    # 功能码03 - 读保持寄存器的应答
                    echo -ne '\x01\x03\x06\x00\x00\x45\x61\x00\x00\xFF\xFF'
                    ;;
                "04")
                    # 功能码04 - 读输入寄存器的应答
                    echo -ne '\x01\x04\x06\x00\x00\x45\x61\x00\x01\xE5\x81'
                    ;;
                "01")
                    # 功能码01 - 读线圈状态的应答
                    echo -ne '\x01\x01\x01\x01\x90\x48'
                    ;;
                "02")
                    # 功能码02 - 读离散输入的应答
                    echo -ne '\x01\x02\x01\x01\xFF\xFF'
                    ;;
                "05")
                    # 功能码05 - 写单个线圈的应答
                    echo -ne '\x01\x05\x00\x02\xFF\x00\xFF\xFF'
                    ;;
                "06")
                    # 功能码06 - 写单个寄存器的应答
                    echo -ne '\x01\x06\x00\x02\x12\x34\xFF\xFF'
                    ;;
                *)
                    # 未知功能码 - 不返回任何应答
                    ;;
            esac
            
            # 短暂延迟
            sleep 0.01
        fi
    fi
done
