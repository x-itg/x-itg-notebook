/*
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __24C02_H
#define __24C02_H
 

#include "stm32f0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
 


#define ADDR_AT24C02_Write 0xA0
#define ADDR_AT24C02_Read  0xA1



uint8_t readAT24C16(uint16_t addr, uint8_t *data, uint16_t len);
uint8_t writeAT24C16(uint16_t addr, uint8_t *data, uint16_t len);

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

