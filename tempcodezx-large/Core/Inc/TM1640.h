#ifndef __TM1640_H__
#define __TM1640_H__
#include "stm32f0xx_hal.h"
#include <stdio.h>
#define   ADDR_START1640        0XC0  
  
#define     DSP1640_DIS     0  
#define     DSP1640_ENB     1  
#define   TeStFlag  0  
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
#define indexMain_A     0
#define indexMain_B     1
#define indexMain_C     2
#define indexMain_D     3
#define indexMain_E     4
#define indexMain_H     5
#define indexMain_S     6//选择 P03页
#define indexHist_A     7//P01 
#define indexHist_B     8 
#define indexHist_C     9 
#define indexHist_D     10 
#define indexHist_E     11 
#define indexFen_1      12//P02 
#define indexFen_2      13 
#define indexFen_3      14 
#define indexCala0_A    15//P03 
#define indexCala0_B    16 
#define indexCala0_C    17 
#define indexCala0_D    18 
#define indexCala0_E    19 
#define indexCala1_A    20 
#define indexCala1_B    21 
#define indexCala1_C    22 
#define indexCala1_D    23 
#define indexCala1_E    24 
#define indexTSet_A10   25//P04
#define indexTSet_A11   26
#define indexTSet_A20   27
#define indexTSet_A21   28
#define indexTSet_A30   29
#define indexTSet_A31   30
#define indexTSet_A40   31
#define indexTSet_A41   32
#define indexTest1      33//P05
#define indexTest2      34
#define indexCom1       35//P06
#define indexCom2       36
#define indexCom3       37
#define indexCom4       38
#define indexFH_1       39//P07
#define indexFH_2       40
#define indexF3_1       41//P08
#define indexF3_2       42
#define indexF4_1       43
#define indexF4_2       44
#define indexBC_1       45//P09
#define indexBC_2       46
#define indexBC_3       47
#define indexBC_4       48
#define indexBC_5       49
#define indexBC_6       50
#define indexDEH_D      51
#define indexDEH_E      52
#define indexDEH_H      53
#define indexa5       54
#define indexa590     55
#define indexb5       56
#define indexb5100    57
#define indexa6       58
#define indexa690     59
#define indexb6       60
#define indexb6100    61

#define indexa8       62
#define indexa890     63
#define indexb8       64
#define indexb8100    65
#define indexa9       66
#define indexa990     67
#define indexb9       68
#define indexb9100    69
#define indexa10      70
#define indexa1090    71
#define indexb10      72
#define indexb10100   73
//主页**    00.A20.0  01.B20.0   02.C20.0  03.D20.0 04.E20.0
//菜单  PXX 05.P0X
//历史  P01 06.AXXXX  07.BXXXX   08.CXXXX  09.DXXXX   10.EXXXX    历史最高温度查看
//风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
//计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200 
//温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
//测试  P05 24.h-CH-  25.h-30.0  
//串口  P06 26.h-dd-  27.h001    28.h01     29.h1
//回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
//补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.

#endif