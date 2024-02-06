
#include "stm32f0xx.h"
#include "SI4432.h"
#include "SavePara.h"
unsigned char RxBuf[RxBuf_Len];
unsigned char TxBuf[TxBuf_Len];  //每秒发射的固定内容的测试信号，第10个数据是前9个数据的校验和，分别为65，66，67，68，69，70，71，72，73，109
unsigned char ItStatus;  //发送接收中断
unsigned char ItStatus1,ItStatus2;
unsigned char RSSI;      //RSSI

FlagType Flag;

#define SI4432_PWRSTATE_READY		0x01		// 模块 Ready 状态定义
#define SI4432_PWRSTATE_TX		0x09		// 模块 发射状态定义
#define SI4432_PWRSTATE_RX		0x05		// 模块 接收状态定义
#define SI4432_PACKET_SENT_INTERRUPT	0x04		// 模块 发射完成中断
#define SI4432_Rx_packet_received_interrupt   0x02      // 模块 收到数据包中断

const unsigned char auchCRCHi[] = //CRC ????×??ú?μ±í
{ 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
}; 

const unsigned char auchCRCLo[] = //CRCμí??×??ú?μ±í
{ 
  0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
  0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
  0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
  0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
  0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
  0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
  0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
  0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
  0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
  0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
  0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
  0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
  0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
  0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
  0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
  0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
  0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
  0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
  0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
  0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
  0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
  0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
};


 //CRC 校验函数
unsigned int crc16(unsigned char *puchMsg, unsigned int usDataLen) 
{ 
  unsigned char uchCRCHi = 0xFF ; //??CRC×??ú3?ê??ˉ
  unsigned char uchCRCLo = 0xFF ; //μíCRC ×??ú3?ê??ˉ
  unsigned long int uIndex ; //CRC?-?·?Dμ??÷òy
  
  while(usDataLen--) // ′?ê????￠?o3???
  { 
    uIndex = uchCRCHi ^ *puchMsg++; //????CRC
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex]; 
    uchCRCLo = auchCRCLo[uIndex]; 
  } 
  return (uchCRCHi << 8 | uchCRCLo); 
}
/*****************************************
16MHZ下us级延时
有效范围是2us - xus
函数进入退出占用1us，实际延时比设定值大1us
*****************************************/
void delay_us(volatile uint16_t us)
{    
	
    while(us > 0)
    {
        us --; 
    }
}


void delay_ms(volatile uint16_t ms)
{
    while(ms > 0)
    {
        ms --;
        delay_us(998);
    }
}


	 
//*****************************************************************************************
//函数名：SpisendByte(INT8U dat)
//输入：发送的数据
//输出：无
//功能描述：SPI发送一个字节
//*****************************************************************************************
char SpiTxRxByte(char dat)
{
    unsigned char i;
    unsigned char temp;
    unsigned char ret;
    temp = dat;
    ret = 0;
    for(i=0;i<8;i++)
    {
        
        if(temp & 0x80)
        {
            MOSI(1);
        }
        else
        {
            MOSI(0);
        }
        SCK(1);
        ret = ret<<1;
        if(MISO)
        {
            ret |= 0x01;
        }
        SCK(0);
        temp = temp<<1;
        
    }

    return ret;           		  // return read byte
}

unsigned char SPI_RW_Reg(unsigned char addr, unsigned char data_)
{
	unsigned char status;
	
	nSEL(0);//NSS = 0;
	SpiTxRxByte(addr);
	status = SpiTxRxByte(data_);
	nSEL(1);//NSS = 1;
	
	return(status);
}
void SPI_Read_Buf(unsigned char addr, unsigned char *data_, unsigned char number)
{
	int number_ctr;
	
	nSEL(0);//NSS = 0;
	SpiTxRxByte(addr);
	for(number_ctr=0;number_ctr<number;number_ctr++)
	data_[number_ctr] = SpiTxRxByte(0);
	nSEL(1);//NSS = 1;
}
void SPI_Write_Buf(unsigned char addr, unsigned char *data_, unsigned char number)
{
	unsigned char number_ctr;
	nSEL(0);//NSS = 0;  
	SpiTxRxByte(addr);   
	for(number_ctr=0; number_ctr<number; number_ctr++)
	SpiTxRxByte(*data_++);
	nSEL(1);//NSS = 1;
}
  unsigned char a=0; 
void Si4432_init(void)
{
  	
	

  //si4432_Init();
  SDN(1);//SDNN = 1;
  delay_ms(10);	// RF 模块上电复位

  SDN(0);//SDNN = 0;
  delay_ms(500);
  a=SPI_RW_Reg(READREG + 0x03, 0x00);	 //清RF模块中断	
  a=SPI_RW_Reg(READREG + 0x04, 0x00);

  SPI_RW_Reg(WRITEREG + 0x06, 0x80);  //使能同步字侦测
  SPI_RW_Reg(WRITEREG + 0x07, 0x01);  //进入 Ready 模式

  SPI_RW_Reg(WRITEREG + 0x09, 0x7f);  //负载电容= 12P
  SPI_RW_Reg(WRITEREG + 0x0a, 0x05);  //关闭低频输出
  //SPI_RW_Reg(WRITEREG + 0x0b, 0xea);  //GPIO 0 当做普通输出口
  //SPI_RW_Reg(WRITEREG + 0x0c, 0xea);  //GPIO 1 当做普通输出口
  //SPI_RW_Reg(WRITEREG + 0x0d, 0xf4);  //GPIO 2 输出收到的数据
  SPI_RW_Reg(WRITEREG + 0x70, 0x2c);  
  SPI_RW_Reg(WRITEREG + 0x1d, 0x40);  //使能 afc
#if 0
  // 1.2K bps setting
  SPI_RW_Reg(WRITEREG + 0x1c, 0x16);  //发射16个Nibble的Preamble
  SPI_RW_Reg(WRITEREG + 0x20, 0x83);   
  SPI_RW_Reg(WRITEREG + 0x21, 0xc0);  //
  SPI_RW_Reg(WRITEREG + 0x22, 0x13);  // 
  SPI_RW_Reg(WRITEREG + 0x23, 0xa9);  //
  SPI_RW_Reg(WRITEREG + 0x24, 0x00);  //
  SPI_RW_Reg(WRITEREG + 0x25, 0x04);  //
  SPI_RW_Reg(WRITEREG + 0x2a, 0x14);
  SPI_RW_Reg(WRITEREG + 0x6e, 0x09);
  SPI_RW_Reg(WRITEREG + 0x6f, 0xd5);
  //1.2K bps setting end		
#endif
  
  // 9.6K bps setting
  SPI_RW_Reg(WRITEREG + 0x1c, 0x1e);  //发射16个Nibble的Preamble
  SPI_RW_Reg(WRITEREG + 0x20, 0xd0);   
  SPI_RW_Reg(WRITEREG + 0x21, 0x00);  //
  SPI_RW_Reg(WRITEREG + 0x22, 0x9d);  // 
  SPI_RW_Reg(WRITEREG + 0x23, 0x49);  //
  SPI_RW_Reg(WRITEREG + 0x24, 0x00);  //
  SPI_RW_Reg(WRITEREG + 0x25, 0x24);  //
  SPI_RW_Reg(WRITEREG + 0x2a, 0x20);
  SPI_RW_Reg(WRITEREG + 0x6e, 0x09);
  SPI_RW_Reg(WRITEREG + 0x6f, 0xd5);
  //9.6K bps setting end
  
  SPI_RW_Reg(WRITEREG + 0x30, 0x8c);  //使能PH+ FIFO模式，高位在前面，使能CRC校验
  SPI_RW_Reg(WRITEREG + 0x32, 0xff);  //byte0, 1,2,3 作为头码
  SPI_RW_Reg(WRITEREG + 0x33, 0x42);  //byte 0,1,2,3 是头码，同步字3,2 是同步字
  SPI_RW_Reg(WRITEREG + 0x34, 16);    //发射16个Nibble的Preamble
  SPI_RW_Reg(WRITEREG + 0x35, 0x20);  //需要检测4个nibble的Preamble
  SPI_RW_Reg(WRITEREG + 0x36, 0x2d);  //同步字为 0x2dd4
  SPI_RW_Reg(WRITEREG + 0x37, 0xd4);
  SPI_RW_Reg(WRITEREG + 0x38, 0x00);
  SPI_RW_Reg(WRITEREG + 0x39, 0x00);
  SPI_RW_Reg(WRITEREG + 0x3a, 's');   //发射的头码为： “swwx"
  SPI_RW_Reg(WRITEREG + 0x3b, 'w');
  SPI_RW_Reg(WRITEREG + 0x3c, 'w');
  SPI_RW_Reg(WRITEREG + 0x3d, 'x');
  SPI_RW_Reg(WRITEREG + 0x3e, 10);    //总共发射10个字节的数据
  SPI_RW_Reg(WRITEREG + 0x3f, 's');   //需要校验的头码为：”swwx"
  SPI_RW_Reg(WRITEREG + 0x40, 'w');
  SPI_RW_Reg(WRITEREG + 0x41, 'w');
  SPI_RW_Reg(WRITEREG + 0x42, 'x');
  SPI_RW_Reg(WRITEREG + 0x43, 0xff);  //头码1,2,3,4 的所有位都需要校验
  SPI_RW_Reg(WRITEREG + 0x44, 0xff);  // 
  SPI_RW_Reg(WRITEREG + 0x45, 0xff);  // 
  SPI_RW_Reg(WRITEREG + 0x46, 0xff);  // 
  SPI_RW_Reg(WRITEREG + 0x6d, 0x06);  //发射功率设置  0x00:+0dBM  0x01:+3dBM  0x02:+6dBM  0x03:+9dBM  0x04:+11dBM  0x05:+14dBM  0x06:+17dBM  0x07:20dBM
  SPI_RW_Reg(WRITEREG + 0x79, 0x0);   //不需要跳频
  SPI_RW_Reg(WRITEREG + 0x7a, 0x0);   //不需要跳频
  SPI_RW_Reg(WRITEREG + 0x71, 0x22);  //发射不需要CLK，FiFo，FSK模式
  SPI_RW_Reg(WRITEREG + 0x72, 0x30);  //频偏为 30KHz
  SPI_RW_Reg(WRITEREG + 0x73, 0x0);   //没有频率偏差
  SPI_RW_Reg(WRITEREG + 0x74, 0x0);   //没有频率偏差
  SPI_RW_Reg(WRITEREG + 0x75, 0x53);  //0x53  //频率设置 434
  SPI_RW_Reg(WRITEREG + 0x76, 0x64);  //
  SPI_RW_Reg(WRITEREG + 0x77, 0x00);
  
  a=SPI_RW_Reg(READREG + 0x03, 0x00);	 //清RF模块中断	
  a=SPI_RW_Reg(READREG + 0x04, 0x00);

  TX0_RX0;	// 天线开关不在发射，接收状态
}

unsigned char RF4432_RSSI(void)
{	
	if(!(nIRQ))
	{
		ItStatus = SPI_RW_Reg(READREG + 0x04,0x00);  //读中断寄存器
	 	if((ItStatus&0x80)==0x80)
		{
			RSSI = SPI_RW_Reg(READREG + 0x26, 0x00);
		    return 1;
		}
		else
		{
		    return 0;
		}
	}
	else
	{
	return 0;
	}
}
unsigned char GetRF4432_RSSI(void)
{
        unsigned char ret;
	ret = SPI_RW_Reg(READREG + 0x26, 0x00);
        return ret;
        
        
}
unsigned char RF4432_RxPacket(unsigned char *rxBuffer)
{	
  if(!(nIRQ))
  {
    SPI_RW_Reg(WRITEREG + 0x07, 0x01);	//进入 Ready 模式
    
    ItStatus = SPI_RW_Reg(READREG + 0x03,0x00);  //读中断寄存器
    
    if((ItStatus&0x02)==0x02)
    {
      SPI_Read_Buf(0x7f, rxBuffer, RxBuf_Len);
      return 1;
    }
    else if((ItStatus & 0x01) == 0x01)//CRC出错
    {
      SPI_RW_Reg(WRITEREG + 0x08, 0x02); //接收FIFO清0 重新接收
      SPI_RW_Reg(WRITEREG + 0x08, 0x00);
      
      SPI_RW_Reg(WRITEREG + 0x07, 0x05);  //RF 模块进入接收模式
      return 0;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

void RF4432_TxPacket(unsigned char *dataBuffer)
{
  unsigned char TX_Timeout;
  SPI_RW_Reg(WRITEREG + 0x34, 40);   //发射40个Nibble 的前导码
  SPI_RW_Reg(WRITEREG + 0x3e, TxBuf_Len);  //总共发射10个字节的数据
  
  SPI_Write_Buf(WRITEREG + 0x7f, dataBuffer, TxBuf_Len);   //将要发送的数据写入寄存器
  
  ItStatus = SPI_RW_Reg(READREG + 0x03, 0x00);	 //清掉现有的中断标志
  ItStatus = SPI_RW_Reg(READREG + 0x04, 0x00); 
  SPI_RW_Reg(WRITEREG + 0x07, 0x09); //进入发射模式
  
  TX_Timeout = 0;
  while(nIRQ)		//等待中断
  {
    TX_Timeout++;
    if(TX_Timeout>=100)
    {
      //Uart_sentstr("发送失败...TX ERROR\r\n");
      TX_Timeout=0;
      delay_ms(200);

      Si4432_init();

      break;		//则强制跳出
    }
    delay_ms(5);
  }

  if(!(nIRQ))
  {
    ItStatus = SPI_RW_Reg(READREG + 0x03,0x00);  //读中断寄存器
    if((ItStatus&0x04)==0x04)
    {
      ItStatus=0  ;//Uart_sentstr("发送成功...TX OK\r\n");
    }
  }
}
//设置为接收模式
void SetRX_Mode(void)
{	
  SPI_RW_Reg(WRITEREG + 0x07, 0x01);	//进入 Ready 模式
  //delay_ms(1);
  TX0_RX1;		//设置天线开关
  //TX1_RX0;///////////////////////////////////////////
  //delay_ms(1);

  SPI_RW_Reg(WRITEREG + 0x08, 0x02); //接收FIFO清0
  SPI_RW_Reg(WRITEREG + 0x08, 0x00);

  ItStatus = SPI_RW_Reg(READREG + 0x03, 0x00);	 //清掉现有的中断标志
  ItStatus = SPI_RW_Reg(READREG + 0x04, 0x00);	 

  SPI_RW_Reg(WRITEREG + 0x05, 0x03);  //RF模块收到整包数据后，产生中断
  SPI_RW_Reg(WRITEREG + 0x06, 0x00);  

  ItStatus = SPI_RW_Reg(READREG + 0x03, 0x00);	 //清掉现有的中断标志
  ItStatus = SPI_RW_Reg(READREG + 0x04, 0x00);  

  SPI_RW_Reg(WRITEREG + 0x07, 0x05);  //RF 模块进入接收模式
}

void SetTX_Mode(void)
{
  SPI_RW_Reg(WRITEREG + 0x07, 0x01);	//rf模块进入Ready模式
  //delay_ms(5);
  TX1_RX0;		//设置天线开关的方向
  //delay_ms(5);

  //SPI_RW_Reg(WRITEREG + 0x08, SPI_RW_Reg(READREG + 0x08, 0x00)|0x01); //接收FIFO清0
  //SPI_RW_Reg(WRITEREG + 0x08, SPI_RW_Reg(READREG + 0x08, 0x00)&0xFE);

  SPI_RW_Reg(WRITEREG + 0x05, 0x04);	//整包数据发射完后，产生中断
  SPI_RW_Reg(WRITEREG + 0x06, 0x00);  

  ItStatus = SPI_RW_Reg(READREG + 0x03, 0x00);	 //清掉现有的中断标志
  ItStatus = SPI_RW_Reg(READREG + 0x04, 0x00); 

}
unsigned char SetFrequency(unsigned int freq)
{
#if     1
	unsigned char SelectREG;
	unsigned int FCREG;
	unsigned int temp_int;
	unsigned char temp_fb;
	
	float result;
        SelectREG = 0;
        FCREG = 0;
	SelectREG |=0x40;
	temp_int = freq/10;
	if((freq>=4800)&&(freq<=9300))
	{
		SelectREG |=0x20;
		temp_int = temp_int>>1;
		temp_fb = (temp_int-240)/10;
		SelectREG = SelectREG + temp_fb;
		result = freq;
		result = (result/200)-24-temp_fb;
		result = result*64000;
		FCREG = (unsigned int)result;
	}
	else if((freq>=2400)&&(freq<=4790))
	{
		//temp_int = temp_int>>1;
		temp_fb = (temp_int-240)/10;
		SelectREG = SelectREG + temp_fb;
		result = freq;
		result = (result/100)-24-temp_fb;
		result = result*64000;
		FCREG = (unsigned int)result;
	}
	else
	{
		return 1;		//err 
	}
	SPI_RW_Reg(WRITEREG + 0x75, SelectREG);  //频率设置 
	SPI_RW_Reg(WRITEREG + 0x76, (FCREG>>8));  //
	SPI_RW_Reg(WRITEREG + 0x77, FCREG & 0xff);
#endif        
	return 0;
}
uint8_t SetRxBasePower(uint8_t Power)
{
  Power = Power&0x0f;
  
    if(Power<8)
    {
        SPI_RW_Reg(WRITEREG + 0x6d, Power);
        return 0;
    }
    return 1;
}

//设置SI4432 波特率
uint8_t SetSi4432_Bps(uint8_t mark)
{
  mark = mark >> 4;
  
  if(mark > 6)
    return 0;
  
  switch(mark)
  {
    case 0:  //1200
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x09);
      SPI_RW_Reg(WRITEREG + 0x6f, 0xd5);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  

      break;
    }
  case 1:  //2400
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x13);
      SPI_RW_Reg(WRITEREG + 0x6f, 0xa9);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  
      break;
    }
  case 2: //4800
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x27);
      SPI_RW_Reg(WRITEREG + 0x6f, 0x52);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  
      break;
    }
  case 3:  //9600
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x4e);
      SPI_RW_Reg(WRITEREG + 0x6f, 0xa5);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  
      break;
    }
  case 4:  //14400
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x75);
      SPI_RW_Reg(WRITEREG + 0x6f, 0xf7);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  
      break;
    }
  case 5:  //19200
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x9d);
      SPI_RW_Reg(WRITEREG + 0x6f, 0x49);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x24);  
      break;
    }
  case 6:   //38400
    {
      SPI_RW_Reg(WRITEREG + 0x6e, 0x09);
      SPI_RW_Reg(WRITEREG + 0x6f, 0xd5);          
      SPI_RW_Reg(WRITEREG + 0x70, 0x04);  
      break;
    }

    default : break;
  }
  
  return 1;
}
POLLSTA PollSta=YESPOLL;//需要轮询
PROCESTA ProcessSta=PROC_RESET;
unsigned int rcvtimecnt=0;//接收超时计数实在收不到就算了、
unsigned int si4432waittime=0;//避免发送频繁、 
unsigned short SetDacValue=0;
RCV_FRAME Slave[16];//一共支持16个无线从机
RCV_FRAME iMenu;//用于显示

unsigned  char iMenuNum=0;//要显示的地址
unsigned  char iMenuNumOnlineFlag;//要显示的地址是否在线

unsigned char CurAddr=0;//当前访问的那个
unsigned char ModNum=3;//总共的模块个数
//SI4432 发送一串数据
void Si4432_SendTxBuf(void)
{
  uint16_t sum = 0;
  
  TxBuf[0]=CurAddr;//目标地址
  TxBuf[1]=0xCD;//主板标志高位
  TxBuf[2]=0xEF;//主板标志低位
  TxBuf[3]=0x00;//是否设置从机DAC
  TxBuf[4] = ((SetDacValue>>8)&0x00ff);//DacValueH
  TxBuf[5] = ((SetDacValue>>0)&0x00ff);//DacValueL
  sum = crc16(TxBuf,6);
  TxBuf[6] = ((sum>>0)&0x00ff);//crcL
  TxBuf[7] = ((sum>>8)&0x00ff);//crcH
  sum = 0;
   
  SetTX_Mode();		// 发送模式
  
  RF4432_TxPacket(TxBuf);
  
  SetRX_Mode();//发完后变成接收模式
}



 unsigned char  Handle_Pack(void)
{
  unsigned char rxflag=0;
  unsigned short check_tmp=0;
  unsigned char check_tmpL=0;
  unsigned char check_tmpH=0;
  
  unsigned short tmp=0;
  
  
  rxflag=RF4432_RxPacket(RxBuf);
  
  if(rxflag==1)//数据接收成功那就分析数据
  {
    check_tmp = crc16(RxBuf,RxBuf_Len-2);
    check_tmpL= ((check_tmp>>0)&0x00ff);
    check_tmpH= ((check_tmp>>8)&0x00ff);
    //收到的和刚发出去的都地址一样
    //从板标志
    //校验和通过
    if(CurAddr==RxBuf[0]&&0xFE==RxBuf[1]&&0xDC==RxBuf[2]&&check_tmpL==RxBuf[21]&&check_tmpH==RxBuf[22])
    {
      tmp=RxBuf[3];//电压1
      tmp=tmp<<8;
      tmp=tmp|RxBuf[4];
	  Slave[CurAddr].V1  =tmp;

      //Slave[CurAddr].V1  =(long)tmp*220/SaveCalaCoefficient[CurAddr][0];
      
      tmp=RxBuf[5];//电压2
      tmp=tmp<<8;
      tmp=tmp|RxBuf[6];
	  Slave[CurAddr].V2  =tmp;
      //Slave[CurAddr].V2  =(long)tmp*220/SaveCalaCoefficient[CurAddr][1];
      
      tmp=RxBuf[7];//电压3
      tmp=tmp<<8;
      tmp=tmp|RxBuf[8];
	  Slave[CurAddr].V3  =tmp;
      //Slave[CurAddr].V3  =(long)tmp*220/SaveCalaCoefficient[CurAddr][2];
      
      tmp=RxBuf[9];//电流1
      tmp=tmp<<8;
      tmp=tmp|RxBuf[10];
	  Slave[CurAddr].I1  =tmp;
      //Slave[CurAddr].I1  =(long)tmp*100/SaveCalaCoefficient[CurAddr][3];
      
      tmp=RxBuf[11];//电流2
      tmp=tmp<<8;
      tmp=tmp|RxBuf[12];
	  Slave[CurAddr].I2  =tmp;
      //Slave[CurAddr].I2   =(long)tmp*100/SaveCalaCoefficient[CurAddr][4];
      
      tmp=RxBuf[13];//电流3
      tmp=tmp<<8;
      tmp=tmp|RxBuf[14];
	  Slave[CurAddr].I3  =tmp;
      //Slave[CurAddr].I3   =(long)tmp*100/SaveCalaCoefficient[CurAddr][5];
      //从板DAC功能未用 
      /*
      tmp=RxBuf[15];//DAC值
      tmp=tmp<<8;
      tmp=tmp|RxBuf[16];
      Slave[CurAddr].DAC =tmp;
      
      tmp=RxBuf[17];//状态1
      tmp=tmp<<8;
      tmp=tmp|RxBuf[18];
      Slave[CurAddr].STA1=tmp;
      
      tmp=RxBuf[19];//状态2
      tmp=tmp<<8;
      tmp=tmp|RxBuf[20];
      Slave[CurAddr].STA2=tmp;
      */ 
      return 1;//接收正确
       
    }else
    {
      return 0;//没收对
    }
  }else
  {
    return 0;//没收到
  }
}


//模块数据无限收发处理
//发 命令
//收 收数据CurAddr
unsigned short OnLineFlag=0;//当地址位在线的为1表示在线否则不在线
unsigned char  OnLineNum=0;//统计在线数目 最多16个 最少零个
unsigned int PollTime=0;//每隔一段时间 地址0-15轮询一遍因为我不知道有几个模块
//根据在线标志、计算在线个数
//计算在线的个数 将在线的数据取到菜单显示的变量
void CalcOnLinNum(unsigned char *OnLineNum,unsigned short flag)
{
  unsigned char i=0;
  unsigned char j=0;
  
  iMenuNumOnlineFlag=(flag&(1<<iMenuNum))>>iMenuNum;//1在线 0 不在线
  
  for(i=0;i<16;i++)
  {
    if((flag&(1<<i))!=0)//在线的
    {
      if(iMenuNum==i)//在线的 为要显示的数据
      {
        iMenu.V1=Slave[i].V1;
        iMenu.V2=Slave[i].V2;
        iMenu.V3=Slave[i].V3;
        iMenu.I1=Slave[i].I1;
        iMenu.I2=Slave[i].I2;
        iMenu.I3=Slave[i].I3;
        iMenu.DAC=Slave[i].DAC;
        
       
      }
      
      j++;
    }
  }
  *OnLineNum=j;//在线数
}

//根据轮询后得到的在线的标志
//根据现在访问的地址计算下一次访问的地址
unsigned char GetNextAddr(unsigned char *addr,unsigned short flag)
{
  unsigned char i=0;
  unsigned char j=0;

  for(i=(*addr);i<15;i++)//0~14
  {
    if((flag&(1<<(i+1)))!=0)//当前地址的下一个在线
    {
      (*addr)=(i+1);//就让当前地址=下一个地址
      
      return 1;//有在线的
    }
    
    if(i==14)//第14  就看第15个
    {
      if((flag&(1<<15))==0)//第15个不在线的话  就回到最前一个
      {
        for(j=0;j<15;j++)
        {
          if((flag&(1<<j))!=0)
          {
            (*addr)=j;//回到头
            return 0;//没有在线了回到头
          }
        }
      }
    }
    
  }
  
  return 0;//没在线的
}

void ModProcess(void)
{
  static unsigned char rxtimeoutcnt=0;
 
  CalcOnLinNum(&OnLineNum,OnLineFlag);//统计在线数目
  
  
  if(PollTime>60000&&PollSta==NOPOLL)//每隔1分钟轮询一次
  {
    PollTime=0;
    PollSta=YESPOLL;
  }
  
  switch(ProcessSta)
  {
    case PROC_RESET:
      
      if(si4432waittime>10)
      {
        ProcessSta=PROC_Sending;//去发送/////////////////////////
        si4432waittime=0;
      }else
      {
        ProcessSta=PROC_RESET;//等待
      }
      rcvtimecnt=0;
      
      //下一个地址的模块处理
      
      break;
    case PROC_Sending:
    
      Si4432_SendTxBuf();//发送的真实动作
      
      ProcessSta=PROC_Recing;//去收
      
      break;
   
    case PROC_Recing://正在接收
      ///////////////////////////正常收到///////////////////////////////
      if(Handle_Pack()==1)//接收正确/////////////////////////////
      {
        ProcessSta=PROC_Recok;//收到当前地址的回音
        
        rxtimeoutcnt=0;
        //每隔一段时间轮询一次
        if(PollSta==YESPOLL)
        {
          OnLineFlag=OnLineFlag|(1<<CurAddr);//置1 
         
          if(CurAddr<16)//16次依次问过来
          {
            
            CurAddr++;
          }
          else 
          {
            CurAddr=0;
            PollSta=NOPOLL;//轮询完一次后不需要轮询了 在其他地方再会被提起轮询的
          }
          
        }else//不轮询
        {
          GetNextAddr(&CurAddr,OnLineFlag);//不用轮询时 根据在线标志得到下一个访问地址
        }
        
              break;
      }else
      {
        ProcessSta=PROC_Recing;
      }
      
      ///////////////////////////没有收到///////////////////////////////
      if(rcvtimecnt>500)//没有收到
      {
        //每隔一段时间轮询一次
        if(PollSta==YESPOLL)
        {
          OnLineFlag=OnLineFlag&(~(1<<CurAddr));//清零
          
          if(CurAddr<16)//16次依次问过来
          {
             
            CurAddr++;
          }
          else 
          {
            
            CurAddr=0;
            PollSta=NOPOLL;//轮询完一次后不需要轮询了  在其他地方再会被提起轮询的
          }
        }else//不轮询  
        {
          GetNextAddr(&CurAddr,OnLineFlag);//不用轮询时 根据在线标志得到下一个访问地址
        }
        
        ProcessSta=PROC_RESET;//没有收到当前地址的回音
        
        rcvtimecnt=0;
        
        rxtimeoutcnt++;
        
        if(rxtimeoutcnt>16)//10次超时
        {
          rxtimeoutcnt=0;//16次这样的超时说明一个模块都不在那就一直复位
          Si4432_init();                         //si4432初始化
          SetFrequency(ModeType_si4432_PL);     //si4432初始化频率
          SetRxBasePower(ModeType_si4432_XDZY); //si4432初始化增益
          SetSi4432_Bps(ModeType_si4432_XDZY);  //si4432初始化波特率
        }
              break;
      }
      
      break;

    
    case PROC_Recok:
    
      ProcessSta=PROC_RESET;
    
    break;
    
  }
}
