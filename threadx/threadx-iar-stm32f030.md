# 

# ThreaadX

## PendVC_Handle

1. stm32cubemx nvic 去掉Pendablerequest for system service
   
   > - NVIC >->- Code generation
   > - 利用stm32cubemx生成iar源代码

## 配置IAR工程

2. 将threadx源码的common和port拷贝到工作目录下面
   iar编译中添加inc[C/C++ Complier  Preprocessor]中添加包含文件夹
   
   > - 移植 stm32cubeid 时汇编的头文件也要添加
   > - $PROJ_DIR$/../common/inc
   > - $PROJ_DIR$/../ports/cortex_m0/iar/inc
   >   添加源文件
   > - 添加common/src下面所有.c文件
   > - 添加port/cortex_m0/iar/example_build/tx_iar.c
   > - 添加port/cortex_m0/iar/example_build/tx_initialize_low_level.s
   > - 添加port/cortex_m0/iar/src/tx_thread_context_restore.s
   > - 添加port/cortex_m0/iar/src/tx_thread_context_save.s
   > - 添加port/cortex_m0/iar/src/tx_thread_interrupt_control.s
   > - 添加port/cortex_m0/iar/src/tx_thread_interrupt_disable.s
   > - 添加port/cortex_m0/iar/src/tx_thread_interrupt_restore.s
   > - 添加port/cortex_m0/iar/src/tx_thread_schedule.s
   > - 添加port/cortex_m0/iar/src/tx_thread_stack_build.s
   > - 添加port/cortex_m0/iar/src/tx_thread_system_return.s
   > - 添加port/cortex_m0/iar/src/tx_timer_interrupt.s

## 修改频率

3. tx_initalize_low_level.s中修改频率
   
   > - SYSTEM_CLOCK      EQU   48000000
   > - SYSTICK_CYCLES    EQU   ((SYSTEM_CLOCK / 1000) >-1)

# STM32F030C8TX_FLASH.ld中加入__RAM_segment_used_end__ = .;

```
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

## 中断向量 _vectors换成g_pfnVectors

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

# v

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

## 事件 (32 bits)

> - 相当于一个32bits的变量，每个位可以放一个事件标志。
> - 高频放事件标志，旧的事件标志会被新的事件标志刷新

## 信号量 (Thredx里叫做信号计数灯)

> - 计数的 最大0xFFFFFFFF=4294967295
> - 放一次加1，取一次减1，高频的放低频的取会导致不同步

## 消息队列

```
#define     NUMBER_OF_MESSAGES 20
#define     MESSAGE_SIZE       1
#define     QUEUE_SIZE         NUMBER_OF_MESSAGES*MESSAGE_SIZE*sizeof(ULONG)
ULONG       UrgentQueuebuf[NUMBER_OF_MESSAGES]; //队列的内存  四字节对齐的
TX_QUEUE    UrgentQueue;                        //队列
ULONG       UrgentTxMessage;//发送的消息
ULONG       UrgentRxMessage;//接收的消息
//在中断中发送 务必要用TX_NO_WAIT选项
tx_queue_send(&UrgentQueue, &UrgentTxMessage, TX_NO_WAIT);
tx_queue_receive(&UrgentQueue, &UrgentRxMessage, 5);
```

## 事件 信号量 队列

- 事件   相当于一个32bits的变量其中每一个bit都可以作为事件标志

- 信号量 相当于一个32bits的计数变量，放一个加1，取一个减一

- 队列   相当于带数据的信号量
  
  ## RTOS
  
  > - 每个任务相当于一个软件定中断
  > - 统计任务+空闲任务 计算CPU使用率
  > - 统计任务中 
  > - &ensp;while(1){
  > - &ensp;&ensp;ctrrun=idlecnt;
  > - &ensp;&ensp;idlecnt=0;
  > - &ensp;&ensp;usage=100-ctrrun/ctrmax;
  > - &ensp;&ensp;delay(100);
  > - &ensp;}
  > - 空闲任务中
  > - &ensp;idlecnt++;
  > - 挂起
  > - &ensp;sleep=suspend->time->resume
  > - &ensp;终止terminate后可以reset
  > - &ensp;terminate->reset
  > - 打印栈空间
  > - &ensp;TX_THREAD      *p_tcb;            // 定义一个任务控制块指针 
  > - &ensp;p_tcb = &AppTaskStartTCB;
  > - &ensp;(int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_ptr;//当前使用栈
  > - &ensp;(int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_highest_ptr;//最大栈使用

## 中断优先级

> - STM32F030没有中断分组功能
> - 对于MCU中断数值越小优先级越高，即 0 代表最高优先级，15 代表最低优先级。
> - 用户配置任务的优先级数值越小，那么此任务的优先级越高（0 是最高优先级任务），ThreadX 没有
>   空闲任务，如果大家创建空闲任务，需要将其设置为最低优先级。

## 关停和开启

> - 为了保证一定被触发 还是放信号量
> - 开启指令，一次开启不成功，可以发两次，还是放事件标志。高频发送开启后避免被取到。