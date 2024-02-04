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
    unsigned char i; //��������ʱ��ѭ��ʹ����ʱ����

    for (i = 0; i < 16; i++) //��16λ���ݰ�λ����
    {
      LED595_CLK_RESET;  //ʱ���ߵ͵�ƽ
      if ((SetData & 0x8000) == 0x8000) //�ж����ݸߵ�λ
      {
        LED595_DATA_SET; //�������ݸ�λ
      }
      else
      {
        LED595_DATA_RESET; //�������ݵ�λ
      }
      SetData = SetData << 1; //��������1λ
      LED595_CLK_SET;          //ʱ���߸ߵ�ƽ
    }
    
    //�������������
    LED595_LATCH_RESET;
    LED595_LATCH_SET;
    LED595_SOE_RESET;//�͵�ƽ�����Ч   ȫ���͵�ƽ  ��ͨ
}

void HC595Init(void)
{
    LED595_SCLR_RESET;
    LED595_SCLR_SET;//��λ
    LED595SendData(outputallinit);
}

