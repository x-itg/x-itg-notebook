#### 一、安装Espressif-IDE 2.8.1 with EspIDF v5.0

- https://dl.espressif.cn/dl/esp-idf/?idf=4.4
- 安装到C:\EspressifIDE这个路径

#### 二、VScode安装espidf插件

#### 三、F1 EspIDF Openocd管理器 或 打开openocd，vscode右下脚espidf插件的start openocd按钮，如果vscode 无法打开openocd的话可以用ESP-IDF 5.0 CMD下手动运行 openocd -f board/esp32c3-builtin.cfg

#### 四、F5开始调试

#### 五、安装路径变了的话 修改.vscode文件夹中的launch.json 和 settings.json

#### 六、命令行gdb运行：riscv32-esp-elf-gdb build/blink.elf -x gdbinit

#### 七、文件gdbinit内容：

```
set remotetimeout 100
target extended-remote :3333
set remote hardware-watchpoint-limit 2
mon reset halt
flushregs
thb app_main
c
```
