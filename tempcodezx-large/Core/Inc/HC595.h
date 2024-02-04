#include "stm32f0xx_hal.h"
//74HC595口线高低电平宏定义
#define    LED595_DATA_SET     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET)   //(GPIOB->BSRR=(1<<4))//串行输入
#define    LED595_DATA_RESET   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET) //(GPIOB->BRR=(1<<4))
#define    LED595_CLK_SET      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)  //(GPIOC->BSRR=(1<<14))//时钟
#define    LED595_CLK_RESET    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)//(GPIOC->BRR=(1<<14))
#define    LED595_LATCH_SET    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)  //(GPIOC->BSRR=(1<<13))//输出锁存时钟
#define    LED595_LATCH_RESET  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)//(GPIOC->BRR=(1<<13))
#define    LED595_SCLR_SET     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET)  //(GPIOC->BSRR=(1<<15))//复位
#define    LED595_SCLR_RESET   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET)//(GPIOC->BRR=(1<<15))
#define    LED595_SOE_SET      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)  //(GPIOB->BSRR=(1<57))//输出有效
#define    LED595_SOE_RESET    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)//(GPIOB->BRR=(1<<5))

extern void HC595Init(void);
extern unsigned short hcreg;
extern void LED595SendData(unsigned short SetData);