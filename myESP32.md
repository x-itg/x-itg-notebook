- 将idf安装在C:\Espressif444下

- example中的例子移动到C:\Espressif444\frameworks\mycode\blink

- cd ../mycode/blink

- 设置芯片型号

- idf.py set-target esp32c3

- idf.py menuconfig

- idf.py all

- idf.py flash

- openocd -f board/esp32c3-builtin.cfg  #openocd -f board/stm32f0discovery.cfg

- D:\Espressif\tools\riscv32-esp-elf\esp-2021r2-patch5-8.4.0\riscv32-esp-elf\bin

- riscv32-esp-elf-gdb

###### 读取elf文件

- 在工程目录下新建一个gdbinit的文件

- ```
  set remotetimeout 100
  target extended-remote :3333
  set remote hardware-watchpoint-limit 2
  mon reset halt
  flushregs
  thb app_main
  c
  ```

- riscv32-esp-elf-gdb build/blink.elf -x gdbinit  #arm-none-eabi-gdb.exe build/MotDrv.elf

- b 78 #在78行创建断点

- b C:/Espressif444/frameworks/mycode/blink/main.c 78

- info b 

- info b 2

- c

- disable 2

- enable 2

- d 2 #删除2号断电

- tbreak 78 #触发一次后删除

- tb 78 #触发一次后删除

- watch i #i是变量名 

- print /x i

- ![](C:\Users\57298\AppData\Roaming\marktext\images\2023-02-24-23-08-05-image.png)

- print funcname::i

- print '路径'::i

- print i=6#print设置变量值

- display #在中断后打印

- x命令 查看对应内存的中地址

- set print address on

- n  #next

- step #进入

- finish #step的返回

- until 139 #全速运行到那一行

- tui enable 

- tui disable 

- jump 145#强制跳145行后运行至断点

- set var i=5

#### PIP

pip config set global.index-url http://mirrors.aliyun.com/pypi/simple
pip config set global.trusted-host mirrors.aliyun.com

#### 环境变量

D:\Espressif\tools\xtensa-esp-elf-gdb\11.2_20220823\xtensa-esp-elf-gdb\bin
D:\Espressif\tools\riscv32-esp-elf-gdb\11.2_20220823\riscv32-esp-elf-gdb\bin
D:\Espressif\tools\xtensa-esp32-elf\esp-2021r2-patch5-8.4.0\xtensa-esp32-elf\bin
D:\Espressif\tools\xtensa-esp32s2-elf\esp-2021r2-patch5-8.4.0\xtensa-esp32s2-elf\bin
D:\Espressif\tools\xtensa-esp32s3-elf\esp-2021r2-patch5-8.4.0\xtensa-esp32s3-elf\bin
D:\Espressif\tools\riscv32-esp-elf\esp-2021r2-patch5-8.4.0\riscv32-esp-elf\bin
D:\Espressif\tools\esp32ulp-elf\2.35_20220830\esp32ulp-elf\bin
D:\Espressif\tools\cmake\3.23.1\bin
D:\Espressif\tools\openocd-esp32\v0.11.0-esp32-20221026\openocd-esp32\bin
D:\Espressif\tools\ninja\1.10.2\
D:\Espressif\tools\idf-exe\1.0.3\
D:\Espressif\tools\ccache\4.3\ccache-4.3-windows-64
D:\Espressif\tools\dfu-util\0.9\dfu-util-0.9-win64
D:\Espressif\frameworks\esp-idf-v4.4.4\tools