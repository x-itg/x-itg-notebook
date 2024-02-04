#include "TM1640.h"
#include "control.h"
#include "24c02.h"
#include "DHT.h"

/********  TM1640 GPIO 配置 *************************/
void TM1640_GPIO_Config(void)
{
    TM1640SLK_HING;
    TM1640DAT_HING;
}

//*************************************
// 函数名称：_Nopt
// 函数功能：延时函数
// 入口参数：延时时间
// 出口参数：无
//***************************************/
void delay_us(unsigned short T_Dly)
{
    unsigned int cnt = 3;
    while (T_Dly--)
    {
        cnt--;
        cnt--;
        cnt--;

        ;
        cnt = 0;
    }
    return;
}

/****************************************
起始函数
***************************************/
void TM1640Start(void)
{
    TM1640SLK_HING;
    delay_us(2);
    TM1640DAT_HING;
    delay_us(2);
    TM1640DAT_LOW;
    delay_us(2);
}
/*************************************
结束函数
***************************************/
void TM1640Stop(void)
{
    TM1640SLK_LOW;
    delay_us(2);
    TM1640DAT_LOW;
    delay_us(2);
    TM1640SLK_HING;
    delay_us(2);
    TM1640DAT_HING;
}
/*************************************
TM1640WriteByte
写一字节数据  date 为所要写的数据
***************************************/
void TM1640WriteByte(u8 date)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        TM1640SLK_LOW;
        delay_us(2);
        if (date & 0x01) // 先低位后高位
        {
            TM1640DAT_HING;
            delay_us(3);
        }
        else
        {
            TM1640DAT_LOW;
            delay_us(3);
        }
        date = date >> 1; // 数据右移一位
        TM1640SLK_HING;
        delay_us(3);
    }
}
/***************************************
发送数组
Addr1640：起始地址值
*a : 所要发送的数组
ValueLong:想要发送的数据长度
适用于地址自加1模式
**************************************/
void TM1640_SendData(u8 Addr1640, u8 *a, u8 ValueLong)
{
    u8 i;
    TM1640Start();
    TM1640WriteByte(Addr1640);

    for (i = 0; i < ValueLong; i++)
    {
        TM1640WriteByte(a[i]);
    }
    TM1640Stop();
}

extern unsigned char tmNum[7];
void SendData(void)
{

    TM1640Start();
    TM1640WriteByte(0x44);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xC0);
    TM1640WriteByte(dis[tmNum[0]]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xC1);
    TM1640WriteByte(dis[tmNum[1]]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xC2);
    TM1640WriteByte(dis[tmNum[2]]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xC3);
    TM1640WriteByte(dis[tmNum[3]]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xC4);
    TM1640WriteByte(dis[tmNum[4]]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xC5);
    TM1640WriteByte(tmNum[5]); // 其他的点
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0x8b);
    TM1640Stop();
}

// 左边4个数字的显示
void SendDataLed(unsigned char n1, unsigned char n2)
{
    TM1640Start();
    TM1640WriteByte(0x44);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xCF);
    TM1640WriteByte(n1);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xCE);
    TM1640WriteByte(n2);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b);
    TM1640Stop();
}