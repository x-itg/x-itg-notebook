```
汇编的.S全部改成小写的。
// 用stm32cube生成makefile工程时 threadx生成的是iar的汇编文件
// 用stm32cue生成stm32cubeide工程是 threadx生成的是gnu的汇编文件
// 所以生成makefile工程是需要替换成gnu的汇编文件 这个可能是stm32cube的bug 并将makefile中没有用到的.s文件删除
//threaddemo：
#include "main.h"
TX_THREAD thread_0; // 线程控制块
#define thread_0_STACK_SIZE 500
UCHAR thread_0_stack[thread_0_STACK_SIZE];
void thread_0_entry(ULONG thread_input)
{
  while (1)
  {
    HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
    tx_thread_sleep(10);
  }
  tx_thread_delete(&thread_0);
}

TX_THREAD thread_1; // 线程控制块
#define thread_1_STACK_SIZE 500
UCHAR thread_1_stack[thread_0_STACK_SIZE];
void thread_1_entry(ULONG thread_input)
{
  while (1)
  {
    HAL_GPIO_TogglePin(LED_ERR_GPIO_Port, LED_ERR_Pin);
    tx_thread_sleep(10);
  }
  tx_thread_delete(&thread_1);
}
```
