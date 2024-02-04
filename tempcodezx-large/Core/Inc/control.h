#ifndef __CONTROL_H
#define __CONTROL_H
#define  SUMC     100
#define  SUMCFIVE 500
#define  SUMCHAN  5
#include "HC595.h"
extern unsigned short AdDataOrig[SUMC][SUMCHAN];

#define JDQ1_H JDQ_Hx(0) //hcreg=hcreg|0x0001;LED595SendData((unsigned char *)&hcreg)   //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)
#define JDQ1_L JDQ_Lx(0) //hcreg=hcreg&(~0x0001);LED595SendData((unsigned char *)&hcreg)//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

#define JDQ2_H JDQ_Hx(1) 
#define JDQ2_L JDQ_Lx(1) 

#define JDQ3_H JDQ_Hx(2)
#define JDQ3_L JDQ_Lx(2)

#define JDQ4_H JDQ_Hx(3)
#define JDQ4_L JDQ_Lx(3)

#define JDQ5_H JDQ_Hx(4)
#define JDQ5_L JDQ_Lx(4)

#define JDQ6_H JDQ_Hx(5)
#define JDQ6_L JDQ_Lx(5)

#define JDQ7_H JDQ_Hx(6)
#define JDQ7_L JDQ_Lx(6)

#define JDQ8_H JDQ_Hx(7)
#define JDQ8_L JDQ_Lx(7)

#define JDQ9_H JDQ_Hx(8)
#define JDQ9_L JDQ_Lx(8)

#define JDQ10_H JDQ_Hx(9)
#define JDQ10_L JDQ_Lx(9)

#define CLK0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)
#define CLK1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)

#define MOSI0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)
#define MOSI1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET)
#define NETCSH JDQ_Hx(15)
#define NETCSL JDQ_Lx(154) 


#define Key1Read  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_RESET)
#define Key2Read  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)==GPIO_PIN_RESET)
#define Key3Read  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==GPIO_PIN_RESET)
#define Key4Read  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)

#define Key1ReadNone  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_SET)
#define Key2ReadNone  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)==GPIO_PIN_SET)
#define Key3ReadNone  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==GPIO_PIN_SET)
#define Key4ReadNone  (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)

#define URead (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)
#define VRead (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)
#define WRead (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_RESET)
#define UReadNone (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_SET)
#define VReadNone (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_SET)
#define WReadNone (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_SET)

#define DoorError (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_RESET)
#define DoorOK    (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_SET)

#define BeepOn  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_SET)
#define BeepOf  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_RESET)

#define RxEn  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)
#define TxEn  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)
void ReadKey(void);

extern unsigned int sysms;
extern unsigned int readKeyTime;
extern unsigned int willBeepTime;
extern unsigned int oneMinuteCnt;
extern unsigned int one1000msCnt;

extern unsigned char macFlag[23];
extern signed int Ta;//三个温度
extern signed int Tb;
extern signed int Tc;
extern signed int pTa;//断电前三个温度
extern signed int pTb;
extern signed int pTc;
extern signed int pTd;
extern signed int pTe;
extern unsigned int FenH;//风机定时  每隔多少小时
extern unsigned int FenM;//风机定时  持续转几分钟
extern signed int Tcala1;//温度补偿
extern signed int Tcala2;
extern signed int Tcala3;
extern signed int Tset1;//温度阈值
extern signed int Tset2;
extern signed int Tset3;
extern signed int Tset4;
extern signed int cshuiCha;//除湿回差 三个回差值
extern signed int cwhuiCha;//超温报警回差
extern signed int tzhuiCha;//跳闸回差
extern signed int MnTest;//测试 温度值
extern unsigned int addr;
extern unsigned int jojy;
extern unsigned int btlv;
extern float k[5];
extern float b[5];
extern float x1[5];
extern float x2[5];

void JDQ_Hx(unsigned char x);
void JDQ_Lx(unsigned char x);
void write_data_to_TLC5615(unsigned char n,unsigned short data);

extern signed int Ta;//三个温度
extern signed int Tb;
extern signed int Tc;
extern signed int Td;
extern signed int Te;
extern signed int Th;
#endif