#include "ds18b20.h"
void DelayUs(unsigned int nus)
{
  unsigned int x;
  for (; nus > 0; nus--)
    for (x = 100; x > 0; x--)
      ;
}

void DS18B20GPIOINSET(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void DS18B20GPIOOUTSET(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

// 复位DS18B20
void DS18B20_Reset(void)
{
  DS18B20GPIOOUTSET();                                  // SET PG11 OUTPUT
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET); // 拉低DQ
  DelayUs(750);                                         // 拉低750us
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);   // DQ=1
  DelayUs(15);                                          // 15US
}

// 等待DS18B20的回应
// 返回1:未检测到DS18B20的存在
// 返回0:存在
unsigned char DS18B20_IsOnline(void)
{
  uint16_t retry = 0;
  DS18B20GPIOINSET(); // SET PA0 INPUT

  while (DS18B20_DQ_I && retry < 200)
  {
    retry++;
    DelayUs(1);
  };

  if (retry >= 200)
  {
    return 1;
  }
  else
  {
    retry = 0;
  }

  while (!DS18B20_DQ_I && retry < 240)
  {
    retry++;
    DelayUs(1);
  };

  if (retry >= 240)
  {
    return 1;
  }

  return 0;
}

// 从DS18B20读取一个位
// 返回值：1/0
unsigned char DS18B20_ReadBit(void)
{
  unsigned char data;
  DS18B20GPIOOUTSET();
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
  DelayUs(2);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
  DS18B20GPIOINSET();
  DelayUs(12);
  if (DS18B20_DQ_I)
  {
    data = 1;
  }
  else
  {
    data = 0;
  }
  DelayUs(50);
  return data;
}

// 从DS18B20读取一个字节
// 返回值：读到的数据
unsigned char DS18B20_ReadByte(void)
{
  unsigned char i, j, dat;
  dat = 0;
  for (i = 1; i <= 8; i++)
  {
    j = DS18B20_ReadBit();
    dat = (j << 7) | (dat >> 1);
  }
  return dat;
}

// 写一个字节到DS18B20
// dat：要写入的字节
void DS18B20_WriteByte(unsigned char dat)
{
  unsigned char j;
  unsigned char testb;
  DS18B20GPIOOUTSET(); // SET PG11 OUTPUT;
  for (j = 1; j <= 8; j++)
  {
    testb = dat & 0x01;
    dat >>= 1;
    if (testb)
    {
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET); // Write 1
      DelayUs(2);
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
      DelayUs(60);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET); // Write 0
      DelayUs(60);
      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
      DelayUs(2);
    }
  }
}

// 开始温度转换
void DS18B20_StartConversion(void)
{
  DS18B20_Reset();
  DS18B20_IsOnline();
  DS18B20_WriteByte(0xcc); // skip rom
  DS18B20_WriteByte(0x44); // convert
}

// 初始化DS18B20的IO口 DQ 同时检测DS的存在
// 返回1:不存在
// 返回0:存在
unsigned char DS18B20Configuration(void)
{

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET); // 输出1

  DS18B20_Reset();

  return DS18B20_IsOnline();
}

// 从ds18b20得到温度值
// 精度：0.1C
// 返回值：温度值 （-550~1250）
signed short DS18B20_GetTemperature(void)
{
  unsigned char temp;
  unsigned char TL, TH;
  signed short tem;
  DS18B20_StartConversion(); // ds1820 start convert
  DS18B20_Reset();
  DS18B20_IsOnline();
  DS18B20_WriteByte(0xCC); // skip rom
  DS18B20_WriteByte(0xBE); // convert
  TL = DS18B20_ReadByte(); // LSB
  TH = DS18B20_ReadByte(); // MSB

  if (TH > 7)
  {
    TH = ~TH;
    TL = ~TL;
    temp = 0; // 温度为负
  }
  else
  {
    temp = 1; // 温度为正
  }
  tem = TH; // 获得高八位
  tem <<= 8;
  tem += TL;                                  // 获得底八位
  tem = ((signed short)((float)tem * 0.625)); // 转换
  if (temp)
  {
    return tem; // 返回温度值
  }
  else
  {
    return -tem;
  }
}
