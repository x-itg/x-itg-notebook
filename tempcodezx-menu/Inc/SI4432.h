#ifndef __si4432_H
#define __si4432_H	 

#include "stm32f0xx.h"

#define TxBuf_Len 10 
#define RxBuf_Len 23     //定义RF4432数据包长度


//LED端口定义
#define uchar unsigned char

#define INPUT			1
#define OUTPUT			0
#define READREG        0x00  	//读寄存器指令
#define WRITEREG       0x80 	//写寄存器指令

 

#define nIRQ             ((GPIOB->IDR&(1<<6))>>6)                 //中断     
#define nSEL(x)        x?(GPIOB->BSRR=1<<5):(GPIOB->BRR=1<<5)     //片选  
#define SDN(x)         x?(GPIOB->BSRR=1<<15):(GPIOB->BRR=1<<15)   //使能
#define MISO             ((GPIOB->IDR&(1<<14))>>14)               //数据输入  
#define MOSI(x)        x?(GPIOB->BSRR=1<<13):(GPIOB->BRR=1<<13)   //串行数据输出
#define SCK(x)         x?(GPIOB->BSRR=1<<12):(GPIOB->BRR=1<<12)   //时钟 Sck PF7

#define  TX1_RX0	SPI_RW_Reg(0x0e|0x80, 0x01)		// ·￠é?×′ì?μ?ìì???a1??¨ò?
#define  TX0_RX1	SPI_RW_Reg(0x0e|0x80, 0x02)		// 接收状态的天线开关定义
#define  TX0_RX0	SPI_RW_Reg(0x0e|0x80, 0x00)         // 非发射，接收状态的天线开关定义

#define   ModeType_si4432_XDZY      0x37  //波特率和增益
#define   ModeType_si4432_PL        8689  //频率

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
extern  RCV_FRAME iMenu;                        //显示的数据
extern unsigned  char iMenuNumOnlineFlag;       //要显示的地址是否在线
extern unsigned char  OnLineNum;//在先数
extern RCV_FRAME Slave[16];//一共支持16个无线从机
typedef enum {

	PROC_RESET,           //初始化
	PROC_Sending,         //正在发送
        PROC_Recing,
        PROC_Recok
}PROCESTA;//485控制过程
typedef enum {
	YESPOLL,           //轮询
	NOPOLL         //不轮询
}POLLSTA;//轮询状态
 

 
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
