#ifndef __EC20_H
#define __EC20_H
#include "stm32f1xx_hal.h"
#include "string.h"
#include "main.h"
#include "stdio.h"
#pragma pack(1)
#define DEBUGMODE 1//0OLED     1����
#define FacIP     0//0�ֳ�IP   1����IP
#define FacSend   0//0�ֳ�     1�����ֶ����ͱ���
#define NORMARLTIME 800//�������
#define ALARMTIME   800//�������
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
//����RS232���� PA9 PA10
//���ڳ�ʼ��EC20����uart2  PA2��PA3 
//�״�ģ�� PB10��PB11(����U5��1��)
//����IO  //PB12 �̵���
//EC20 Power  PC1 
//EC20 Rst  PC13(����U5��2��)
#define WarmHig   HAL_GPIO_WritePin(AlarmPwr_GPIO_Port, AlarmPwr_Pin, GPIO_PIN_RESET)//PB12 �̵���
#define WarmLow    HAL_GPIO_WritePin(AlarmPwr_GPIO_Port, AlarmPwr_Pin, GPIO_PIN_SET)

#define powerHig  HAL_GPIO_WritePin(EC20PWR_GPIO_Port, EC20PWR_Pin, GPIO_PIN_SET)//PC1
#define powerLow HAL_GPIO_WritePin(EC20PWR_GPIO_Port, EC20PWR_Pin, GPIO_PIN_RESET)

#define rstHig  HAL_GPIO_WritePin(EC20RST_GPIO_Port, EC20RST_Pin, GPIO_PIN_SET)//PB1
#define rstLow HAL_GPIO_WritePin(EC20RST_GPIO_Port, EC20RST_Pin, GPIO_PIN_RESET)
 
#define readSXTwarm HAL_GPIO_ReadPin(SheXiangTou_GPIO_Port, SheXiangTou_Pin)//PC13  DTU��


typedef struct 
{
  unsigned int Len;                     //0 1 2 3
  unsigned int ID;                      //4 5 6 7
  unsigned int Send_Index;              //8 9 10 11
  unsigned int Radar1_Cnt;              //12 13 14 15
  unsigned int Radar2_Cnt;              //16 17 18 19
  unsigned int Radar3_Cnt;              //20 21 22 23
  unsigned int Radar4_Cnt;              //24 25 26 27
  unsigned char Radar1_Warm;            //28
  unsigned char Radar2_Warm;            //29
  unsigned char Radar3_Warm;            //30
  unsigned char Radar4_Warm;            //31
  unsigned char Alarm_Cnt;              //32
  unsigned char Alarm_Enable;           //33
  unsigned char GPS_State;              //34
  unsigned char Time[10];               //35 36 37 38 39 40 41 42 43 44     ��35-44�ֽ��ڵ���ֵΪʱ��
  unsigned char Latitude[10];           //45 46 47 48 49 50 51 52 53 54     ��46-55�ڵ���ֵ��N��SΪLatitude
  unsigned char longitude[11];          //55 56 57 58 59 60 61 62 63 64 65    ��56-66�ڵ���ֵ��E��WΪlongitude
  unsigned int End;                     //66 67 68 69
}Ec20SendData;
typedef struct 
{
  unsigned int Len;                     //0 1 2 3
  unsigned int ID;                      //4 5 6 7
}Ec20HEART;
//46000000ffffffff0000000067452301674523016745230167452301010100000201013139353835392e313233333035392e393939394e31323035392e39339939570000ffff
//46000000
//ffffffff
//00000000
//67452301
//67452301
//67452301
//67452301
//01
//01
//00
//00
//02
//01
//01
//3139353835392e313233
//333035392e393939394e
//31323035392e3939393957
//0000ffff
typedef struct    
{
  unsigned char status;
  unsigned char IMEI[20];
  unsigned char ipstatus;
  unsigned char tcpstaus;
  int sendcount;
  unsigned char datastatus;
  unsigned char ackstatus;
  unsigned char getcount;
  unsigned char getok;
  unsigned char tcpcount;
  unsigned char tcpflag;
  unsigned char enable;
  unsigned char dataunm;
}GSM_init;


typedef struct 
{
  char UtcDate[6];
  char Latitude[10];//����ԭ����
  char longitude[9];//γ��Դ����
  char Latitudess[3];//��������
  char longitudess[2];
  char Latitudedd[7];//С���㲿��
  char longitudedd[7];
  char TrueLatitude[11];//ת��������
  char Truelongitude[10];//ת��������	
}LongLatidata;


typedef struct    
{
  
  unsigned char Realtime[25];
  unsigned char Qcell[25];
  unsigned char GPS[30];
  unsigned char sendplus[100];
  unsigned char senddata[100];
  unsigned char sendackdata[100];	
  unsigned char sendgpsdata[100];	
  unsigned char count;
  unsigned char starnum[5];//��������
  unsigned char speed[10];//�ٶ�ֵ
  unsigned char getnum;
  unsigned char hardfalut;
}GPS_DATA;
extern unsigned char CloudtriggerFlag;
extern unsigned int  CloudtriggerTime;
extern unsigned int  simCnt;
extern unsigned char  waringtimeflag;
extern unsigned int   waringtime;
extern unsigned int   msint;
extern unsigned short ms10;
extern unsigned short PollTimeCnt;
extern GSM_init GSMinit;
extern GPS_DATA gspdata;
extern unsigned int     rsRxLen;
extern unsigned int     rsRxTime;
extern unsigned int     rsRxIndexLen;
extern unsigned char    rsPackFlag;
extern unsigned int     SendCntRecClear;

//RS232
void u1Conf(void);
void u1rxitProcess(void);
void SendTxDebug(unsigned char *p);

//EC20
#define DMARXBUFFERSIZE (100)//EC20��usart2 DMAһ�ν���
#define LASTSZIE        (200) //EC20��usart2 DMA��ν���
void u2Conf(void);
void u2rxitProcess(void);
void SendTxBuf( unsigned char *p);
extern unsigned char RxBufferDMA[DMARXBUFFERSIZE];
extern unsigned short          DMARxLenU2;
extern unsigned char           rsRxBuf[LASTSZIE];

//Radar
#define u3mx 100
extern unsigned char u3rx[u3mx];
extern unsigned short RxLenU3;
void u3Conf(void);
void u3rxProcess(void);

extern unsigned char ready_2_warning;
extern unsigned char waringtimeflag;
extern unsigned char warning_ID;
extern unsigned char warmSend[4];

//FUNC
void PollFun(void);
void ec20power(unsigned char onoffflag);
void Clear_Buffer(void);
void delay_ms(unsigned int n);    

//OLED
void delay_oledms(unsigned int ms);
void OLED_Clear(void);  
void OLED_CLS(void);
int PrintHZ(unsigned short  ucY, unsigned short ucX, const unsigned char *ptr, unsigned short ucRevX, unsigned char ucDispMode, unsigned short ucRevLen);
#endif