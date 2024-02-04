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
#define ADDR_AT24C02_Read 0xA1
#define BcValHig (999) // 温度补偿
#define BcValLow (-999)
#define TsetValHig (2000) // 温度设定最高
#define TsetValLow (0)
#define MNValHig (2500) // 模拟温度最高
#define MNValLow (0)
#define AddrHig (240)
#define AddrLow (1)
#define HCHig 100 // 回差最高
#define HCLow 5
#define TMAX (2400)
#define TMIN (-300)
#define XHTIME 300 // 100*50=5000ms=5秒
#define addrFenH 0
#define addrTcala1 4
#define addrTcala2 8
#define addrTcala3 12
#define addrTcala4 90
#define addrTcala5 94 // 最后添加--------------------
#define addrTset5 98  // 最后添加--------------------
#define addrTset6 102 // 最后添加--------------------
#define addrTset7 106 // 最后添加--------------------
#define addrTset1 16
#define addrTset2 20
#define addrTset3 24
#define addrTset4 28
#define addrcshuiCha 32
#define addrcwhuiCha 36
#define addrtzhuiCha 40
#define addraddr 44
#define addrjojy 48
#define addrbtlv 52
#define addrFenM 56
#define addrk1 60
#define addrk2 64
#define addrk3 68
#define addrb1 72
#define addrb2 76
#define addrb3 80

uint8_t readAT24C16(uint16_t addr, uint8_t *data, uint16_t len);
uint8_t writeAT24C16(uint16_t addr, uint8_t *data, uint16_t len);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
