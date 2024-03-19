/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: MPU6050陀螺仪（硬件I2C）
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "usart/bsp_debug_usart.h"
#include "i2c/bsp_i2c.h"
#include "mpu6050/mpu6050.h"
#include "led/bsp_led.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define TASK_ENABLE 0

/* 私有变量 ------------------------------------------------------------------*/             
/* 扩展变量 ------------------------------------------------------------------*/
extern unsigned int Task_Delay[NumOfTask];

/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;           // 9倍频，得到72MHz主时钟
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;      // 系统时钟：72MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                // APB1时钟：36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                // APB2时钟：72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    --> 1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 --> 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 --> 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{  
  short Accel[3];
	short Gyro [3];
	short Temp;
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();
  /*LED初始化*/
  LED_GPIO_Init();
  /*I2C初始化*/
  MX_I2C_MPU6050_Init();
  
  /*MPU6050初始化*/
  MPU6050_Init();

  /* 调用格式化输出函数打印输出数据 */
	//检测MPU6050
	if (MPU6050ReadID() == 0)
	{	
    printf("检测不到MPU6050，停机");
	  while(1);
  }
  /* 无限循环 */
  while (1)
  {
    if(Task_Delay[0]==TASK_ENABLE)
    {
      LED1_TOGGLE;
      Task_Delay[0]=1000;
    }
    
    if(Task_Delay[1]==0)
    {
      MPU6050ReadAcc(Accel);			
      printf("\r\n加速度： %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
      MPU6050ReadGyro(Gyro);
      printf("陀螺仪： %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
      
      MPU6050_ReturnTemp(&Temp);
      printf("温度： %d",Temp);
      
      Task_Delay[1]=100;//此值每1ms会减1，减到0才可以重新进来这里，所以执行的周期是100ms
    }
  }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
