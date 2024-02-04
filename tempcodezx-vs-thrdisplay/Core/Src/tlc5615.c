#include "tlc5615.h"
#include "param.h"
#include "control.h"
#include "tm1640.h"

void Init_TLC5615(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void write_data_to_TLC5615(unsigned short data)
{
    unsigned char i = 0; // 一共发送 10位 数据位
    tlcCLK0;             // cs需要在 clk为低时，使能

    tlcCS0;
    ; // cs为低，串行数据才能被移入移位寄存器
    delay_us(1000);
    for (i = 0; i < 10; i++) // 一共十位 有效数据位
    {
        if (data & 0x200) // 发送数据 高位，第十位
        {
            tlcMOSI1; // 数据bit = 1
        }
        else
        {
            tlcMOSI0; // 数据bit = 0
        }

        tlcCLK1; // 时钟 从 0 ~ 1 发送数据传递

        tlcCLK0; // 时钟恢复 0

        data <<= 1; // 数据左移
    }

    // 传输 低两位 填充位
    tlcMOSI0;
    tlcCLK1; // 时钟 从 0 ~ 1 发送数据传递

    tlcCLK0; // 时钟恢复 0

    tlcCLK1; // 时钟 从 0 ~ 1 发送数据传递

    tlcCLK0; // 时钟恢复 0

    tlcCS1; // 恢复 cs 高 1
}

void TaDACOut(void)
{
    unsigned short sTa = 0;

    sTa = (unsigned short)((float)Ta * 0.5115);

    if (Ta <= 0)
        sTa = 0;

    if (sTa > 1023)
        sTa = 1023;

    write_data_to_TLC5615(sTa);
}