#include "24C02.h"
#include "string.h"
#include "stdio.h"
//at24c02 2k bits  256bytes 
#define E2PROM_SIZE 0x0800 //2k bits  
#define E2PROM_BASE_ID 0xA0
 
#define E2PROM_WRITE 0x00
#define E2PROM_READ  0x01
 
#define E2PROM_BASE_WID E2PROM_BASE_ID + E2PROM_WRITE
#define E2PROM_BASE_RID E2PROM_BASE_ID + E2PROM_READ
 
#define E2PROM_PAGE_MASK 0x000F
uint8_t writeAT24C16(uint16_t addr, uint8_t *data, uint16_t len)
{
 uint8_t wNum = 0;
 uint16_t lenLeft = len;
 uint8_t deviceId ;
 uint8_t *p = data;
 
 /*is the address overfolw*/
 if(addr + len >= E2PROM_SIZE)
  return 1;
 
 /*calculate the current write position to know how many word can write continully*/
 wNum = 16 - addr & E2PROM_PAGE_MASK;
 if(wNum == 0)
  wNum = 16;
 wNum = lenLeft>=wNum ? wNum : lenLeft;
 
 /*transmit the date to e2prom*/
 while(lenLeft)
 {
  /*calculate the device id*/
  deviceId = (addr >> 8)<=0 ?E2PROM_BASE_WID : (E2PROM_BASE_WID | (uint8_t)((addr>>7)&0x0E));
  
  if( HAL_I2C_Mem_Write(&hi2c1, deviceId, addr&0x00FF, 
        I2C_MEMADD_SIZE_8BIT, p, wNum, 0x20) != HAL_OK)
  {
   HAL_Delay(5);
   continue;
  }   
  addr += wNum;
  lenLeft -= wNum;
  p += wNum;
  wNum = lenLeft > 16 ?16 : lenLeft; 
  HAL_Delay(5);
 }
 
 return HAL_OK;
}
 
uint8_t readAT24C16(uint16_t addr, uint8_t *data, uint16_t len)
{
 uint8_t rNum = 0;
 uint16_t lenLeft = len;
 uint8_t deviceId ;
 uint8_t *p = data;
 
 /*is the address overfolw*/
 if(addr + len >= E2PROM_SIZE)
  return 1;
 
 /*calculate the current write position to know how many word can write continully*/
 rNum = 16 - addr & E2PROM_PAGE_MASK;
 if(rNum == 0)
  rNum = 16;
 rNum = lenLeft>=rNum?rNum:lenLeft;
 
 /*transmit the date to e2prom*/ 
 while(lenLeft)
 {
  /*calculate the device id*/
  deviceId = (addr >> 8)<=0 ? E2PROM_BASE_RID : (E2PROM_BASE_RID | (uint8_t)((addr>>7)&0x0E));
  
  if( HAL_I2C_Mem_Read(&hi2c1, deviceId, addr&0x00FF, 
       I2C_MEMADD_SIZE_8BIT, p, rNum, 20) != HAL_OK)
  {
   printf("I2S Read error!\r\n");
   continue;
  }   
  addr += rNum;
  lenLeft -= rNum;
  p += rNum;
  rNum = lenLeft > 16 ? 16 : lenLeft; 
 }
 
 return HAL_OK; 
}
 
