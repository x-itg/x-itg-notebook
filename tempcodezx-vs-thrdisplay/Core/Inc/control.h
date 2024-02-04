#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#define ADDTNUM 500
extern ADC_HandleTypeDef hadc;
extern unsigned short AdDataOrig[ADDTNUM];
#define qK1_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET)
#define qK1_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET)
#define qK2_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
#define qK2_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define qK3_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET)
#define qK3_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET)

#define JDQ1_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define JDQ1_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)

#define JDQ3_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET) // 5
#define JDQ3_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define JDQ4_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET) // 4
#define JDQ4_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define JDQ5_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define JDQ5_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)

#define Key1Read (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET)
#define Key2Read (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
#define Key3Read (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)
#define Key4Read (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET)
#define Key1ReadNone (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET)
#define Key2ReadNone (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET)
#define Key3ReadNone (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET)
#define Key4ReadNone (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET)
#define URead (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET)
#define VRead (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
#define WRead (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
#define UReadNone (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET)
#define VReadNone (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET)
#define WReadNone (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_SET)
#define DoorError (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
#define DoorOK (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_SET)
#define BeepOn HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET) // SET
#define BeepOf HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET)
#define RxEn HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define TxEn HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
void ReadKey(void);

extern unsigned int sysms;
extern unsigned int readKeyTime;
extern unsigned int willBeepTime;
extern unsigned int oneMinuteCnt;
extern unsigned int one1000msCnt;
extern unsigned char macFlag[17];
extern signed int Ta;
extern signed int Tb;
extern signed int Tc;
extern signed int pTa;
extern signed int pTb;
extern signed int pTc;
extern unsigned int FenH;
extern unsigned int FenM;
extern signed int Tcala1;
extern signed int Tcala2;
extern signed int Tcala3;
extern signed int Tset1;
extern signed int Tset2;
extern signed int Tset3;
extern signed int Tset4;
extern signed int cshuiCha;
extern signed int cwhuiCha;
extern signed int tzhuiCha;
extern signed int MnTest;
extern unsigned int addr;
extern unsigned int jojy;
extern unsigned int btlv;
extern float k1;
extern float k2;
extern float k3;
extern float b1;
extern float b2;
extern float b3;
#endif