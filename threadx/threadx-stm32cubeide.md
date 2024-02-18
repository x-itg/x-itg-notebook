# cubdeide工程

### 2.1 汇编和C语言的头文件都要添加将threadx源码参与编译

> threadx移植参考[threadx 移植(二)-基于 CubeIDE 移植 threadx_tyustli的博客-CSDN博客](https://blog.csdn.net/tyustli/article/details/108373791)

### 2.1.1 添加C语言头文件

> Project Properties --> C/C++ Build --> Settings --> Tools Settings --> MCU GCC Compiler --> Include paths

```将这两个文件夹参与编译
../threadx/ports/cortex_m0/gnu/inc
../threadx/common/inc
```

### 2.1.2 添加汇编头文件

> Project Properties --> C/C++ Build --> Settings --> Tools Settings --> MCU GCC Assembler --> Include paths

```将这两个文件夹参与编译
../threadx/ports/cortex_m0/gnu/inc
../threadx/common/inc
```

### 2.1.3 将源码参与编译

> Project Properties --> C/C++ Build --> Paths and Sysmbols --> Source Location --> Add Folder

```将这两个文件夹参与编译
#
../threadx/ports
../threadx/common 
```

### 2.1.4 优化等级

> stm32cubeide 调试需要-Og等级
> 
> C/C++ --> Settings -->C/C++ Build --> Tool Settings --> MCU GCC Compiler --> Optimization --> None(-Og)

### 2.1.5 tx_initialize_low_level.S

> 将../threadx/ports/cortex_m0/gnu/example_build/tx_initialize_low_level.S
> 移到ports/cortex_m0/gnu/src中
> 刪除ports/cortex_m0/gnu/example_build
> 编辑相应CMakeLists文件

### 2.1.6 加入__RAM_segment_used_end__ = .;

```
 @ STM32F030C8TX_FLASH.ld 文件中
   ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
    __RAM_segment_used_end__ = .;
  } >RAM
```

### 2.1.7 中断向量 _vectors换成g_pfnVectors

```
    LDR     r0, =0xE000E000                         @ Build address of NVIC registers
    LDR     r2, =0xD08                              @ Offset to vector base register
    ADD     r0, r0, r2                              @ Build vector base register
    LDR     r1, =_vectors                           @ Pickup address of vector table
    STR     r1, [r0]                                @ Set vector table address    
@
@    /* Set system stack pointer from vector value.  */
@
    LDR     r0, =_tx_thread_system_stack_ptr        @ Build address of system stack pointer
    LDR     r1, =_vectors                           @ Pickup address of vector table
    LDR     r1, [r1]                                @ Pickup reset stack pointer
    STR     r1, [r0]                                @ Save system stack pointer
```

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

```
    LDR     r0, =0xE000E000                         @ Build address of NVIC registers
    LDR     r2, =0xD08                              @ Offset to vector base register
    ADD     r0, r0, r2                              @ Build vector base register
    LDR     r1, =g_pfnVectors                           @ Pickup address of vector table
    STR     r1, [r0]                                @ Set vector table address    
@
@    /* Set system stack pointer from vector value.  */
@
    LDR     r0, =_tx_thread_system_stack_ptr        @ Build address of system stack pointer
    LDR     r1, =g_pfnVectors                           @ Pickup address of vector table
    LDR     r1, [r1]                                @ Pickup reset stack pointer
    STR     r1, [r0]                                @ Save system stack pointer
```

### 2.1.8 菜单栏debug图标 debug configurations 选择elf文件

## 2.2 VSCODE

> 使用VSCODE编译STM32CUBEIDE工程

### 2.2.1 PowerShell安装vcpkg

> iex (iwr -useb https://aka.ms/vcpkg-init.ps1)
> 下载vcpkg-init.ps1 
> 在powershell中执行iex ./vcpkg-init.ps1

## 2.2.2 VSCode安装Embedded Tools插件

> 查看 命令面板 st project从.cproject 创建项目

#### 菜单栏debug图标  debug configurations  选择elf文件

##### PowerShell安装vcpkg

> iex (iwr -useb https://aka.ms/vcpkg-init.ps1)
> 下载vcpkg-init.ps1 
> 在powershell中执行iex ./vcpkg-init.ps1

# VSCode安装Embedded Tools插件

# 查看 命令面板 st project从.cproject 创建项目

###### Please use the vscode stm32cubeide and stlink to edit debug or program.

###### We plan to gradually abandon them(iar keil jlink) in our MCU software.

###### About RTOS, we will use Azure Threadx more than FreeRTOS.

###### STM32cubeIDE workpace的文件夹 为工程所在目录的上层文件夹

###### Open projetcs form file system

###### Project配置中增加ThreadX头文件路径

###### threadx移植参考https://blog.csdn.net/tyustli/article/details/108373791

###### 计划用vscode导入stm32cubeide的工程 https://devblogs.microsoft.com/cppblog/importing-st-projects-into-visual-studio-code/

###### 汇编和C语言的头文件都要添加将threadx源码参与编译


-------------------------------------------------------------------------------------------------------------------------------------------------

# stm32cubeide支持sprintf浮点数的打印

- Project->Properties->C/C++ Build->Setting->Tool Setting->勾选 Use float with printf from newlib-nano(-u_printf_float)
  
  <img width="" height="480" class="embed-show" src="http://iamrobot.top:7878/?explorer/share/file&hash=1c41C52ve1ZQbdibwikxXq6kEq9Yry-xGCGfjjthz-elPK8NWsINvoDn8BS4SOrkaxU&name=/3.png" alt="3.png"/>

# 使用stm32cube生成iar和stm32cubeide工程

- 记得**不要勾选**Generate Under Root
  
  <img width="" height="480" class="embed-show" src="http://iamrobot.top:7878/?explorer/share/file&hash=48d4XYjbheDuVVEz4Jll94t7GpHbpWsHO3Ttwv8ZzAXKcKI_nbXljiFe4QRQTYlfjBU&name=/iar%E5%92%8Cstm32cubeide.png" alt="iar和stm32cubeide.png"/>