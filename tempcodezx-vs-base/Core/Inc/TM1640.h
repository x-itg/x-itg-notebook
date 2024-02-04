#ifndef __TM1640_H__
#define __TM1640_H__
#include "stm32f0xx_hal.h"
#include <stdio.h>
#define   ADDR_START1640        0XC0  
  
#define     DSP1640_DIS     0  
#define     DSP1640_ENB     1  
  
/* 
控制显示 
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f 分别对应 
1/16,  2/16,  4/16,  10/16, 11/16, 12/16, 13/16, 14/16    
 
设置数据，0x40,0x44分别对应地址自动加一和固定地址模式      
*/  

#define SET_DATA_ORDER  0X40  //数据命令设置 地址自动加1  
#define SET_DISPLAY     0X8F  //显示命令控制  脉冲宽度14/16  
#define SET_DSP1640_DIS 0x80  //显示关控制  
   
#define VT_DLY1640      150//80//10//2000  
#define VT_DLYNOP       10 //80//10  

#define TM1640SLK_LOW       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)
#define TM1640SLK_HING      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)
#define TM1640DAT_LOW       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET)
#define TM1640DAT_HING      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET)
 
void TM1640_GPIO_Config(void);
void tm1640send(void);  

#define u16 unsigned short
#define u8 unsigned char
void Nop1640(u16 T_Dly);
void TM1640Start(void);
void TM1640Stop(void);
void TM1640WriteByte(u8 date);
void TM1640_Init(u8 InValue);
void TM1640_SendData(u8 Addr1640,u8 *a,u8 ValueLong);
void SendData(void);
void LoadPara(void);
void UpateShData(void); 
extern unsigned short AdDataOrig[500][3];

#endif