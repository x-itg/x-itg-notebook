#ifndef __si4432_H
#define __si4432_H	 

#include "stm32f0xx.h"

#define TxBuf_Len 10 
#define RxBuf_Len 23     //����RF4432���ݰ�����


//LED�˿ڶ���
#define uchar unsigned char

#define INPUT			1
#define OUTPUT			0
#define READREG        0x00  	//���Ĵ���ָ��
#define WRITEREG       0x80 	//д�Ĵ���ָ��

 

#define nIRQ             ((GPIOB->IDR&(1<<6))>>6)                 //�ж�     
#define nSEL(x)        x?(GPIOB->BSRR=1<<5):(GPIOB->BRR=1<<5)     //Ƭѡ  
#define SDN(x)         x?(GPIOB->BSRR=1<<15):(GPIOB->BRR=1<<15)   //ʹ��
#define MISO             ((GPIOB->IDR&(1<<14))>>14)               //��������  
#define MOSI(x)        x?(GPIOB->BSRR=1<<13):(GPIOB->BRR=1<<13)   //�����������
#define SCK(x)         x?(GPIOB->BSRR=1<<12):(GPIOB->BRR=1<<12)   //ʱ�� Sck PF7

#define  TX1_RX0	SPI_RW_Reg(0x0e|0x80, 0x01)		// ���騦?���䨬?��?����???a1??����?
#define  TX0_RX1	SPI_RW_Reg(0x0e|0x80, 0x02)		// ����״̬�����߿��ض���
#define  TX0_RX0	SPI_RW_Reg(0x0e|0x80, 0x00)         // �Ƿ��䣬����״̬�����߿��ض���

#define   ModeType_si4432_XDZY      0x37  //�����ʺ�����
#define   ModeType_si4432_PL        8689  //Ƶ��

typedef struct 
{	
  unsigned char reach_1s			: 1;
  unsigned char rf_reach_timeout		: 1;
  unsigned char is_tx				: 1;	
}FlagType;
typedef  struct RcvFrame{	
  unsigned short V1;
  unsigned short V2;
  unsigned short V3;
  unsigned short I1;
  unsigned short I2;
  unsigned short I3;
  unsigned short DAC;
  unsigned short STA1;
  unsigned short STA2;	
}RCV_FRAME;
extern  RCV_FRAME iMenu;                        //��ʾ������
extern unsigned  char iMenuNumOnlineFlag;       //Ҫ��ʾ�ĵ�ַ�Ƿ�����
extern unsigned char  OnLineNum;//������
extern RCV_FRAME Slave[16];//һ��֧��16�����ߴӻ�
typedef enum {

	PROC_RESET,           //��ʼ��
	PROC_Sending,         //���ڷ���
        PROC_Recing,
        PROC_Recok
}PROCESTA;//485���ƹ���
typedef enum {
	YESPOLL,           //��ѯ
	NOPOLL         //����ѯ
}POLLSTA;//��ѯ״̬
 

 
void Si4432_init(void);
extern unsigned char RF4432_RxPacket(unsigned char *rxBuffer);
extern unsigned char RxBuf[RxBuf_Len];
void RF4432_TxPacket(unsigned char *dataBuffer);
void SetRX_Mode(void);
void SetTX_Mode(void);
unsigned char SetFrequency(unsigned int freq);
void delay_ms(uint16_t ms);
uint8_t SetSi4432_Bps(uint8_t mark);
uint8_t SetRxBasePower(uint8_t Power);
 void Si4432_SendTxBuf(void);
 unsigned int crc16(unsigned char *puchMsg, unsigned int usDataLen);
 unsigned char Handle_Pack(void);
 void ModProcess(void);
 void delay_us(volatile uint16_t us);
 void delay_ms(volatile uint16_t ms);
 
 


#endif
