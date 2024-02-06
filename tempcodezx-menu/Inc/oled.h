//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PD6��SCL��
//              SDA   ��PD7��SDA��            
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h" 
#include "stdlib.h"	    	
#define KEYxUP        ((GPIOA->IDR&(1<<1))==0)
#define KEYxDOWN      ((GPIOA->IDR&(1<<4))==0)
#define KEYxLEFT      ((GPIOA->IDR&(1<<0))==0)
#define KEYxRIGHT     ((GPIOA->IDR&(1<<5))==0)
#define SCL_L         (GPIOB->BRR  = (1<<8))
#define SCL_H         (GPIOB->BSRR = (1<<8))
#define SDA_L         (GPIOB->BRR  = (1<<9))
#define SDA_H         (GPIOB->BSRR = (1<<9))
#define SDA_READ      ((GPIOB->IDR&(1<<9))>>9) 

#define TRUE		1
#define FALSE		0
typedef enum KeyType
{
	KEY_NULL,
    KEY_ESC,// 
	KEY_UP,
	KEY_ENTER,
	KEY_DOWN

}KEYTYPE_FRAME;
typedef enum	KeySta
{
	KEY_STA_NULL,
	KEY_STA_RELEASE,
	KEY_STA_PUSH

}KEYSTA_FRAME;
typedef struct Key
{
	unsigned short keyScanCode;//����Ҫ�ĳ�unsigned short
	KEYSTA_FRAME	ekeySta; 
	unsigned int keyNum;

}KEYBUF_FARME; 
typedef struct gbx16         // ������ģ���ݽṹ
{
	char Index[2];
	char Msk[32];
}GBX16;

typedef enum {
	V_NOP,            //null variable type
	V_CHAR,           //char
	V_UCHAR,          //unsigned char
	V_SHORT,            //integer
	V_USHORT,           //unsigned integer
	V_STRING,         //string
	V_ENUM          //enumeration

}VAR_TYPE;

typedef enum {
	REC_TOP,        //the top record in menu
	REC_MIDDLE,     //the middle record in menu
	REC_END         //the end record in menu
}REC_RANGE;
typedef enum {
	LINE_TOP,       //the top record in one line which made up with muliti-record
	LINE_MIDDLE,    //the middle record in one line with muliti-record
	LINE_END,       //the end record in one line with muliti-record
	LINE_TOPEND     //the line made up with only one record
}LINE_RANGE;
typedef enum {
	RECTP_DISP,       //��¼����ʾ����       record type is display, only for display 
	RECTP_NOCUR,	  //��¼�Ĳ��ܱ�ѡ������  ���ܳ�Ϊ��ǰ��¼
	RECTP_EDIT,       //��¼Ϊ�༭����       record type is edit, display and can be edited
	RECTP_LINK        //��¼����������       record type is link, link to top record in anther menu
}REC_TYPE;
typedef enum {

	MENU_DISP,        //current stage is in displaying
	MENU_EDIT         //current stage is in editing
}MENU_STA;

typedef struct RecFrame{
      unsigned char      *pTitle;             //point to string of title
      unsigned char      ucTitleX;            //x coordinate which title displayed at
      unsigned char      ucTitleRevX;         //x coordinate which title reverse video at
      unsigned char      ucTitleRevLen;       //length of title string reverse video at
      volatile void               *pVar;               //point to the variable
      unsigned char      ucVarX;              //the x coordinate which the variable string displayed at
      unsigned char      ucVarLen;            //the length of variable string, include sign, decimal point
      unsigned char      ucVarDecLen;         //the length of decimal part, exclude decimal point
      VAR_TYPE           VarType;             //the variable type, enumeration
      int                siVarMax;            //the maximum of variable
      int                siVarMin;            //the minimum of variable
      unsigned char  **pEnumStr;             //point to the enumeration string
      unsigned char      (* pEventFunc)();	
      REC_RANGE          RecRange;            //record range of a menu
      LINE_RANGE         LineRange;           //line range on one LCD row 
      REC_TYPE           RecType;             //the type of record(display, edit, title, link)
     const struct RecFrame    *pLinkEnter;         //link address to sub-menu if the record is RECTP_LINK
     const struct RecFrame    *pLinkEsc;           //
     
}REC_FRAME;


#define MODE_TEXT 1
#define  M_LCDLINE               4//2
#define  M_LCDTOPSCRLINE         1
#define  M_LCDENDSCRLINE         3
#define  M_LCDWIDTH              30
#define  M_LCDCHARWID            8
#define  M_LCDOFFTIME            20000  //ms
#define  M_FLUSHCYCLE            1
#define  M_STARTOVERTIME         100
#define STARTMENU_MAX_TIME    6 //sec
#define STARTMENU_OVERTIME   100
#define NORMAL			0
#define REVERSE			1
#define CGRAM			1
#define CGROM			2
		    

void SDA_Out(void);
void SDA_In(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char Read_Ask(void);
void Write_OneByte(unsigned char Dat);
void WriteCmd(unsigned char IIC_Cmd);
void WriteDat(unsigned char IIC_Dat);
void OLED_Init(void);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char *chn,unsigned char revshow);
void OLED_ShowCharNomAndRev(unsigned char x,unsigned char y,unsigned char chr,unsigned char rev);
int PrintHZ(unsigned short  ucY, unsigned short ucX, const unsigned char *ptr, unsigned short ucRevX, unsigned char ucDispMode, unsigned short ucRevLen, unsigned char ucCodeGen);
void MenuCtrl(void);
void MenuInit(void);


#endif  
	 



