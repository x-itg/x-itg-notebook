#ifndef __RS485_H
#define __RS485_H	
#include "string.h"
extern unsigned char aRxBuffer;//��һ���ֽڽ����ַ��Ļ���
extern UART_HandleTypeDef huart1;//����1 ���
extern void Error_Handler(void);//��������
void RS485Process(void);//RS485 ��ѭ������
/***************************RS485״̬**************************************/
typedef enum {

	RS485_RESET,           //��ʼ��
        RS485_WaitRec,         //�ȴ����� ͷ
        RS485_Recing,          //�յ�ͷ�����ڽ��� ���ݼ���β
        RS485_Recok            //У���յ�β����ͨ��У�� �Ϳ���ȥ��װ���ݲ�����
  }RS485STA;
/***************************RS485�궨��**************************************/
#define RS485DirSen GPIOA->BSRR=1<<12//����rs485оƬΪ����״̬
#define RS485DirRec GPIOA->BRR=1<<12//����rs485оƬΪ����״̬
#define STARTTIMEOUT 2000 //��ʼ�յ�ͷ֮ǰ�ĳ�ʱ
#define AFTERTIMEOUT      500  //�յ�ͷ�� 500msӦ��������������
/***************************�ṹ���ڴ�����***********************************/
#pragma pack(1)//����ṹ����Ҫ �Ķ��뷽ʽΪ��1���ֽڶ���  �������÷Ų�Ȼsizeof�����ʱ������
/****************************************************************************/
typedef  struct XX{	
  //unsigned char Xnum;//·�� Ϊ1
  float Va;//������ѹa
  float Vb;//������ѹb
  float Vc;//������ѹc
  float Hz;//����Ƶ��
  unsigned char noone;//�Զ����ֽ��� Ϊ0
  float Ia;//��������a
  float Ib;//��������b
  float Ic;//��������c
}Ac_ModFlaot;//������������
/****************************************************************************/
typedef  struct dcmod{	
  unsigned char DcVwarm;//ֱ����ѹ����
  unsigned char DcOpenNum;//ֱ���� ����Ϊ1
  unsigned char DcOpen1;//����1
  unsigned char OwnWarmNum;//�Զ���澯����
  unsigned char OwnWarm[20];//�Զ���澯����
}Dc_ModWarm;//ֱ���澯����
/****************************************************************************/
typedef  struct acswmod{	
  unsigned char SwNum;//���Ŀ�������
  unsigned char Sw1Sta;// 
  unsigned char Sw2Sta;// 
  unsigned char Sw3Sta;// 
  unsigned char Sw4Sta;// 
  unsigned char Sw5Sta;// 
  unsigned char Sw6Sta;// 
  unsigned char Sw7Sta;// 
  unsigned char Sw8Sta;// 
  unsigned char Swxx;//
  //unsigned char OwnNum;//�Զ����� ==3  
  //unsigned char OwnSw[3];//�鶨��Ŀ�����
}Ac_ModSw;//����������
/****************************************************************************/
typedef  struct dcfmod{	
  float DcV;//dc v
  float DcI;//dc i
  unsigned char DcNum;//1
  float DcBI;//��һ·���س�ŵ����
  float DcI1;//ֱ����·
  unsigned OwmNum;//3 �Զ�����
  float Temp;//�¶�
  float BTemp;//����¶�
  float BV;//��һ·���ʣ������
}Dc_ModFloat;//ֱ����������
/****************************************************************************/
typedef  struct acmod{	
  unsigned char VaWarm;//a�ྯ��
  unsigned char VbWarm;//b�ྯ��
  unsigned char VcWarm;//c�ྯ��
  unsigned char FWarm;//Ƶ�ʾ���
  unsigned char OpenNumWarm;//0 �Ͽ�����
  unsigned char OwnWarmNum;//�Զ���澯���� 1
  unsigned char LeiWarm;//0x00
}Ac_ModWarm;//�����澯����
/****************************************************************************/
typedef  struct acdcmod{	
  unsigned char Onoff;//���ػ�
  unsigned char Liu;//������������
  unsigned char Cd;//���䡢���䡢����
  unsigned char OwnNum;//�Զ����ֽ�=0
}AcDc_ModSw;//����������
/****************************************************************************/
typedef  struct acfset{	
  float AcVHig;//������ѹ����
  float AcVLow;//������ѹ����
  float AcAHig;//������������
  float HzHig;//Ƶ������
  float HzLow;//Ƶ������
  unsigned char OwnNum;//�Զ��������ֽ���
}Ac_ModSet;//��������
/****************************************************************************/
typedef  struct acdcwarm{	
  unsigned char WrongSta;//����״̬
  unsigned char OwnNum;//�Զ����ֽ���Ŀ3
  unsigned char VHigOffSta;//��ѹ�ػ�
  unsigned char CommWrongSta;//ͨ�Ź���״̬
  unsigned char FanWrongSta;//���ȹ���״̬
}AcDc_ModWarm;//�����澯
/****************************************************************************/
typedef  struct acdcf{	
   float AcDcV;//������ѹ
   unsigned char AcDcNum;//����ģ������ ��������Ϊ1
   float AcDcI;//ģ���������
   unsigned char OwnNum;//�û��Զ����ֽ��� ����Ϊ0
}AcDc_ModFloat;//������������
/****************************************************************************/
typedef  struct dcmodset{	
   float DcVHig;//ֱ����ѹ����
   float DcVLow;//ֱ����ѹ����
   unsigned char OwnNum;//�Զ��ֽ���������5
   float BatTempHig;//��ظ���
   float EnvTempHig;//��������
   float DcVHigOff;//��ѹ�ػ�ֵ
   float DcCD1;//ƽ��
   float DcCD2;//����
}Dc_ModSet;//ֱ������
/****************************************************************************/
typedef  struct acmdown{	
 
   unsigned char  Res[181];//�����ֽ�
  
}Ac_ModOwn;//�����Զ������

#endif