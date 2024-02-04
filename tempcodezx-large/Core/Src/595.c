#include "HC595.h"

unsigned short outputallinit=0x03ff;
unsigned short hcreg=0; 
unsigned int timedelaycnt=0;
unsigned int timetestcnt=0;

void delayms(unsigned int ms)
{
    timedelaycnt=0;
    while(timedelaycnt<ms);
    timedelaycnt=0;
}

void LED595SendData(unsigned short SetData)
{
    unsigned char i; //发送数据时做循环使用临时变量

    for (i = 0; i < 16; i++) //将16位数据按位发送
    {
      LED595_CLK_RESET;  //时钟线低电平
      if ((SetData & 0x8000) == 0x8000) //判断数据高低位
      {
        LED595_DATA_SET; //发送数据高位
      }
      else
      {
        LED595_DATA_RESET; //发送数据低位
      }
      SetData = SetData << 1; //数据左移1位
      LED595_CLK_SET;          //时钟线高电平
    }
    
    //上升沿输出数据
    LED595_LATCH_RESET;
    LED595_LATCH_SET;
    LED595_SOE_RESET;//低电平输出有效   全部低电平  导通
}

void HC595Init(void)
{
    LED595_SCLR_RESET;
    LED595_SCLR_SET;//复位
    LED595SendData(outputallinit);
}

