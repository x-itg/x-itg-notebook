#ifndef __TLC5615_H
#define __TLC5615_H
#include "main.h"

#define tlcMOSI1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define tlcMOSI0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define tlcCLK1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define tlcCLK0 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define tlcCS1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define tlcCS0 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
void Init_TLC5615(void);
void write_data_to_TLC5615(unsigned short data);
void TaDACOut(void);
extern void delay_us(unsigned short T_Dly);
#endif