#include "stm32f0xx_hal.h"
#include "control.h"
#include "ds18b20.h"
#include "TM1640.h"
unsigned int sysms=0;
signed short ReadTemputer=0;
unsigned int willBeepTime=0;
void ReadKey(void)
{
  if(Key1Read)
  {
    //JDQ1_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ1_L;
    
  }
  
  if(Key2Read)
  {
    //JDQ2_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ2_L;
  }
  
  if(Key3Read)
  {
    //JDQ3_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ3_L;
  }
  
  if(Key4Read)
  {
    //JDQ4_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ4_L;
  }
  
  if(willBeepTime!=0)
  {
    BeepOn;
  }
  if(willBeepTime==0)
  {
    BeepOf;
  }
  UpateShData();
  
  if(sysms>500)
  { 
    sysms=0;
     
    ReadTemputer=DS18B20_GetTemperature();
  }
}