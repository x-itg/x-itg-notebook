#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f0xx_hal.h"


#define DS18B20_Direction_I  DS18B20GPIOINSET()
#define DS18B20_Direction_O  DS18B20GPIOOUTSET()

#define DS18B20_DQ_I (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6)==GPIO_PIN_RESET)
 
extern unsigned char DS18B20_IsOnline(void) ;
extern void DS18B20_StartConversion(void);
extern unsigned char DS18B20Configuration(void);
extern signed short DS18B20_GetTemperature(void);

#endif