#include "stm32f0xx.h"
#include "RS485.h"
#include "SI4432.h"
#include "SavePara.h"
/**************************����ΪЭ�����賣������*****************************/
/**************************����ΪЭ�����賣������*****************************/
#define PROT_ADD SavePara.Addr
const unsigned char PROT_SOI                 =   0x7E;//ͷ ���÷ֽ�Ϊascii��
const unsigned char PROT_EOI                 =   0x0D;//ͷ ���÷ֽ�Ϊascii��
const unsigned char PROT_VER                 =   0x21;//������ͨ�Ű汾
 const unsigned char PROT_CID1_AC             =   0x40;//���������Ϣ
const unsigned char PROT_CID1_AC2DC          =   0x41;//��������Ϣ
const unsigned char PROT_CID1_DC             =   0x42;//ֱ�������Ϣ
const unsigned char PROT_CID2_ACQ_DATA_FLOAT =   0x41;//��ȡ��������
const unsigned char PROT_CID2_ACQ_DATA_INT   =   0x42;//��ȡ������  �� 
const unsigned char PROT_CID2_ACQ_DATA_SW    =   0x43;//��ȡ������
const unsigned char PROT_CID2_ACQ_WARM       =   0x44;//��ȡ�澯
const unsigned char PROT_CID2_ACQ_REMOTE     =   0x45;//��ȡң�� ��
const unsigned char PROT_CID2_ACQ_PARA_FLOAT =   0x46;//��ȡ����������
const unsigned char PROT_CID2_ACQ_PARA_INT   =   0x47;//��ȡ����������  �� 
const unsigned char PROT_CID2_SET_PARA_FLOAT =   0x48;//�趨������������ 
const unsigned char PROT_CID2_SET_PARA_OwnInt=   0x81;//�趨�������Զ���
const unsigned char PROT_CID2_SET_PARA_INT   =   0x49;//�趨������������ �� 
const unsigned char PROT_CID2_ACQ_VER        =   0x4F;//��ȡͨ�Ű汾��
const unsigned char PROT_CID2_ACQ_ADD        =   0x4F;//��ȡͨ�Ű汾��
const unsigned char PROT_CID2_FACTOR         =   0x51;//��ȡ������Ϣ
const unsigned char PROT_RTN                 =   0x00;//�ظ�״̬
const unsigned char PROT_DATAFLAG            =   0x00;//����״̬
const unsigned char PROT_MODNUM              =   0x01;//ģ������
const unsigned char PROT_MODCUR              =   0x01;//ģ�����
 
/**************************����ΪЭ���������ݶ���*****************************/
/**************************����ΪЭ���������ݶ���*****************************/
Ac_ModFlaot     AcModFlaot;              //������������
Ac_ModWarm      AcModWarm;               //������������
Ac_ModSw        AcModSw;                 //������������
Ac_ModSet       AcModSet;                //�����趨����
Ac_ModOwn       AcModOwn;                //�����Զ�������
Dc_ModFloat     DcModFloat;              //ֱ����������
Dc_ModWarm      DcModWarm;               //ֱ���澯����
Dc_ModSet       DcModSet;                //ֱ���趨������
AcDc_ModSw      AcDcModSw;               //������������
AcDc_ModWarm    AcDcModWarm;             //�����澯����
AcDc_ModFloat   AcDcModFloat;            //����ģ�鸡����
unsigned char   aRxBuffer;              //һ��һ���յĻ���
unsigned char  RxBuffer[30];            //���ջ���
unsigned char  Rx_count_UART1=0;        //UART1 ���ռ�����
unsigned char  TxAsciiBuffer[500];      //���ͻ�����
unsigned int   RS485RcvWaitTime=0;      //�ȴ�ͷ��ʱʱ��
unsigned int   RS485RcvingTime=0;       //���յ�ͷ�ĳ�ʱʱ��
RS485STA RS485RunSta=RS485_RESET;        //RS485 ����״̬

/**************************Э�������蹦�ܺ���*****************************/
/**************************Э�������蹦�ܺ���*****************************/
//��4��char ���һ��float
//���յ���ʱ�� float���ǵ�8λ ������ǰ���
//����Ҫ������������ ��8λ�ŵ������ȥ��
void FourChar2Float(float *pfloat,unsigned char *pchar)
{
  unsigned int tmp=0;
   
  tmp=(tmp|(unsigned int)(*pchar++)<<0);
  tmp=(tmp|(unsigned int)(*pchar++)<<8);
  tmp=(tmp|(unsigned int)(*pchar++)<<16);
  tmp=(tmp|(unsigned int)(*pchar++)<<24);
  *pfloat=*(float *)&tmp;
}
//�����0x16  ���0x31 0x36  ����Ϊ1
//lenָ����0x16����ĳ���
//�� �� ��
void Char2Asiic(unsigned char *pchar,unsigned char *passic,unsigned short len)
{   
	unsigned short i=0;
        unsigned char charh,charl=0;
        
	for(i=0;i<len;i++)
	{       
              charh=(((*pchar)&0xf0)>>4);
              
              charl=(((*pchar++)&0x0f)>>0);
              
              if(charh<10)
              {
                (*passic++)=charh+0x30;
              }else
              {
                (*passic++)=charh+0x30+7;//0x0A+0x37=0x41--'A'
              }
              
              if(charl<10)
              {
                (*passic++)=charl+0x30;
              }else
              {
                (*passic++)=charl+0x30+7;
              }  
	}
}
//0x31 0x36 ��� 0x16һ��
//lenָ����0x16����ĳ���
//�� �� ��
void Assic2Char(unsigned char *pchar,unsigned char *passic,unsigned short len)
{
	  unsigned char tmp1=0x00;
          unsigned char tmp2=0x00;
          unsigned short i=0;
          for(i=0;i<len;i++)
          {
            tmp1=((*passic++)-0x30);
            tmp2=((*passic++)-0x30);
            if(tmp1>=0x10)tmp1=tmp1-7;
            if(tmp2>=0x10)tmp2=tmp2-7;
            
            *pchar++ = (tmp1<<4)|tmp2;
          }
}
//�����ݱ��assic��
void Data2Assic(unsigned char *source,unsigned char *targetassic,unsigned short len)
{
  unsigned short i=0;
  for(i=0;i<len;i++)
  {
    Char2Asiic(source++,targetassic++,1);
    targetassic++;

  }
}
//����У���ַ���
//�õ���У��ֵ��λ
//�õ���У��ֵ��λ
//��β�����±�
void CheckAsiicSum(unsigned char *pchar,unsigned char *pcharH,unsigned char *pcharL,unsigned short end)
{
          unsigned int chksumint=0;
          unsigned short i;
          pchar++;//��ͷ�����Ե�
          for(i=1;i<(end-5);i++)
          {
            chksumint=chksumint+(*pchar++);//�ӽ�ȥ
          }
          chksumint=chksumint&0x0000ffff;//ģ65536 
          chksumint=(~chksumint);//ȡ��
          chksumint=chksumint+1;//��1
          
          *pcharH=((chksumint&0x0000ff00)>>8);
          *pcharL=((chksumint&0x000000ff)>>0);
          
}
//���ȵ������Լ�������У��
//����λΪsum ��12λΪ����asiic�ĳ�ȥSOI��EOI�ĳ���
void EditLengthSum(unsigned short *psource)
{
  unsigned char txlenth[3];
  unsigned short pso=*psource;
  unsigned short psotmp;
           txlenth[0]=(pso&(0x00f))>>0;
           txlenth[1]=(pso&(0x0f0))>>4;
           txlenth[2]=(pso&(0xf00))>>8;
           psotmp=txlenth[0]+txlenth[1]+txlenth[2];
           *psource=(((~(psotmp&(0x0f)))+1)<<12)|pso;//�Ժ�ģ16ȡ��+1
}


/******************����ΪЭ���������蹦�ܣ���ȡ���ݣ�����***********************/
/******************����ΪЭ���������蹦�ܣ���ȡ���ݣ�����***********************/
//��ȡ�����Զ�������
void GetAcModOwn(void)
{
  unsigned char i;
    for(i=0;i<181;i++)
    {
      AcModOwn.Res[i]=0;
    }
}

//�պ���
void GetDummy(void)
{
  return;
}
//��ȡֱ������
void GetDcModSet(void)
{
  DcModSet.DcVHig=(float)SavePara.DcHig/10;//ֱ����ѹ�澯ֵ
  DcModSet.DcVLow=(float)SavePara.DcLow/10;//ֱ����ѹ�澯ֵ
  DcModSet.OwnNum=5;//�Զ����ֽ���
  DcModSet.BatTempHig=60;//����¶ȸ澯ֵ
  DcModSet.EnvTempHig=50;//�����¶ȸ澯ֵ
  DcModSet.DcVHigOff=60;//��ѹ�ػ���ѹ
  DcModSet.DcCD1=48;//����ֵ
  DcModSet.DcCD2=50;//����ֵ
}
//��ȡ����ģ�鸡����
void GetAcDcModFloat(void)
{
  AcDcModFloat.AcDcV=300;//������ѹ
  AcDcModFloat.AcDcNum=1;//������Ŀ
  AcDcModFloat.AcDcI=17;//����
  AcDcModFloat.OwnNum=0;//�Զ����ֽ���
}
//��ȡ�����澯
void GetAcDcModWarm(void)
{
  AcDcModWarm.WrongSta=2;//����״̬
  AcDcModWarm.OwnNum=3;//3��
  AcDcModWarm.VHigOffSta=1;//��ѹ�ػ�
  AcDcModWarm.CommWrongSta=1;//ͨ�Ź���״̬
  AcDcModWarm.FanWrongSta=1;//���ȹ���״̬
}
//��ȡ�����趨ֵ
void GetAcModSet(void)
{
  AcModSet.AcVHig=SavePara.AcHig;//������ѹ����
  AcModSet.AcVLow=SavePara.AcLow;//������ѹ����
  AcModSet.AcAHig=100.0;//������������
  AcModSet.HzHig=60.0;//Ƶ������
  AcModSet.HzLow=40.0;//Ƶ������
  AcModSet.OwnNum=0;//�Զ��������ֽ���
}
//��ȡ��������������
void GetAcDcModSw(void)
{
  AcDcModSw.Onoff=0x00;//���ػ�
  AcDcModSw.Liu=0x00;//������������
  AcDcModSw.Cd=0x00;//���䡢���䡢����
  AcDcModSw.OwnNum=0x00;//�Զ����ֽ�=0
}
//��ȡ�������溯��
void GetAcModWarm(void)
{
  AcModWarm.VaWarm=0x00;//a�ྯ��
  AcModWarm.VbWarm=0x00;//b�ྯ��
  AcModWarm.VcWarm=0x00;//c�ྯ��
  AcModWarm.FWarm=0x00;//Ƶ�ʾ���
  AcModWarm.OpenNumWarm=0x00;//0 �Ͽ�����
  AcModWarm.OwnWarmNum=0x01;//0x01 //�Զ���澯���� 1
  AcModWarm.LeiWarm=0x00;//����������
}

//��ȡ��������������
void GetAcModFloat(void)
{
  //AcModFlaot.Xnum=1;//·��
  AcModFlaot.Va=Slave[0].V1;//����a���ѹ
  AcModFlaot.Vb=Slave[0].V2;//����b���ѹ
  AcModFlaot.Vc=Slave[0].V3;//����c���ѹ
  AcModFlaot.Hz=50;//����Ƶ��
  AcModFlaot.noone=0;//�û��Զ������� ����Ϊ0
  AcModFlaot.Ia=Slave[0].I1;//����a����
  AcModFlaot.Ib=Slave[0].I1;//����b����
  AcModFlaot.Ic=Slave[0].I1;//����c����
  

}
//��ȡֱ�����溯��
void GetDcModWarm(void)
{
  unsigned char i;
  DcModWarm.DcVwarm=0x00;////ֱ����ѹ����
  DcModWarm.DcOpenNum=0x01;//ֱ���� ����Ϊ1
  DcModWarm.DcOpen1=0x00;//����1
  DcModWarm.OwnWarmNum=20;//�Զ���澯����
  for(i=0;i<20;i++)//�Զ���澯����
  {
    DcModWarm.OwnWarm[i]=0x00;//�Զ���澯���ݸ���ʵ��չ������
  }
  
}
//��ȡ�������غ���
void GetAcModSw(void)
{
  AcModSw.SwNum=8;//���Ŀ�������
  AcModSw.Sw1Sta=0x1;// ����Ϊ1
  AcModSw.Sw2Sta=0x1;// ����Ϊ1
  AcModSw.Sw3Sta=0x1;// ����Ϊ1
  AcModSw.Sw4Sta=0x1;// ����Ϊ1
  AcModSw.Sw5Sta=0x1;// ����Ϊ1
  AcModSw.Sw6Sta=0x1;// ����Ϊ1
  AcModSw.Sw7Sta=0x1;// ����Ϊ1
  AcModSw.Sw8Sta=0x1;// ����Ϊ1
  AcModSw.Swxx=0;
}

//��ȡֱ�����㺯��
void GetDcModFloat(void)
{

  DcModFloat.DcV=(float)MainPara.DcV/10;//12.0;//ֱ����ѹ
  DcModFloat.DcI=13.0;//ֱ������
  DcModFloat.DcNum=1;//ֱ��·��
  DcModFloat.DcBI=14.0;////��һ·���س�ŵ����
  DcModFloat.DcI1=15.0;//ֱ����·����
  DcModFloat.OwmNum=3;////3 �Զ�����
  DcModFloat.Temp=20.0;//�¶�
  DcModFloat.BTemp=24.0;//����¶�
  DcModFloat.BV=400.0;//�������
  
}


/****************************����ΪЭ������������װ����********************************/
/****************************����ΪЭ������������װ����********************************/
/****************************Э�鲿��˵��**********************************************/
//���ܣ����ݴ����ظ�
//CharHead��ɣ�[�汾][��ַ][CID1][LENGTH][DATAFLAG][M]
//����LENGTH�ĸ�4λΪ ��ȥͷβ��asiic�볤�ȵĺ�У��ģ16��+1
//����char��ɣ�[EOI][CharHead]+[n*Pack]+[2*Check][EOI]
//����asiic��ɣ�[EOI][CharHead_Asiic]+[n*Pack_Asiic]+[2*Check_Asiic][EOI]
/***************************�������˵��***********************************************/
//func �����뺯������Ϊ��ַ ��Ӧ��ȡ�÷������ݰ�pack������ֵ��Ӧ�ĺ���
//PackStruct���������ݰ�pack��Ӧ�Ľṹ��
//cid1����Ӧ�����cid1ֵ
//PackNum�����ݰ�pack�ĳ��ȡ�����sizeof(pack��Ӧ�ṹ��)
//[DataflagMLen=2]
//[DATAFLAG]������Щ����û��DATAFLAGʱ DataflagMLen=0��������Э�鷢��û��[DataFlag]������϶�û��[M]��
//[M]������Щ����ʱû��[M] DataflagMLen=1��
//[DATAFLAG]��[M]����ʱ DataflagMLen=2��
/***************************QQ:572981033**********************************************/
void DataMakeSend(void (*func)(void),unsigned char *PackStruct,unsigned char cid1,unsigned char PackNum,unsigned short PackLen,unsigned short DataflagMLen)
 {
   unsigned char CharHead[8];//�ַ���ʼ�Ǽ����̶�������
   unsigned char CharSum[2];//��βУ��2���ֽڵ�char
   unsigned char AsiicSum[4];//��βУ��4���ֽڵ�asiic
   
   unsigned char PackCharBegin=0;//��ʼ���飨�������ݵ�[]
   unsigned short  CharMiddleLen=0;// �ַ����ȣ���ȥ ͷ��β�ģ�
   unsigned short AsiicMiddleLen=0;//�ַ������asiic�ĳ��� ����ȥ ͷ��β�ģ�
   unsigned short AsiicLENGTH=0;//LENGTH��
   
   PackCharBegin=DataflagMLen+6;//char��ʼ��pack���ݵĵط�  
   CharMiddleLen=(PackNum*PackLen+DataflagMLen+6);//char��ȥͷβ�ĳ���
   AsiicMiddleLen=CharMiddleLen*2;//��ȥͷβ��asiic�볤��
   AsiicLENGTH=AsiicMiddleLen;//�������ݵ�asiic�ĳ��� ������ݴ����д��ڷ��ͳ�ȥ
   EditLengthSum(&AsiicLENGTH);//��asiic�ĳ��Ƚ���У�鴦�� �ڸ���λ�м���checksum
   TxAsciiBuffer[0]=PROT_SOI;//asiicͷֱ���ǡ�~����
   CharHead[0]=PROT_VER;//�汾��
   CharHead[1]=PROT_ADD;//��ַ
   CharHead[2]=cid1;//CID1
   CharHead[3]=PROT_RTN;//ʵʱ״̬RTN
   CharHead[4]=(AsiicLENGTH&(0xff00))>>8;//
   CharHead[5]=(AsiicLENGTH&(0x00ff))>>0;//�����������ж��е�
   CharHead[6]=PROT_DATAFLAG;//û��ûû���ϱ��ı��� 0x11 0x10 0x01 0x00
   CharHead[7]=PackNum;//ģ����1-5 �ڼ���ģ��
   Char2Asiic(CharHead,&TxAsciiBuffer[1],PackCharBegin);//����8���ȷ����asiic
   (*func)();//��ȡ���ݺ���
   Data2Assic(PackStruct,&TxAsciiBuffer[PackCharBegin*2+1],PackLen*PackNum);//�����ݰ����asiic��
   CheckAsiicSum(TxAsciiBuffer,&CharSum[0],&CharSum[1],AsiicMiddleLen+6);// ��asiic�м�����ݽ���У��ó�2��char
   Char2Asiic(CharSum,AsiicSum,2);//�ٰ�������У��Ū��asiic
   TxAsciiBuffer[AsiicMiddleLen+1]=AsiicSum[0];//У��asiic��ֵ
   TxAsciiBuffer[AsiicMiddleLen+2]=AsiicSum[1];//У��asiic��ֵ
   TxAsciiBuffer[AsiicMiddleLen+3]=AsiicSum[2];//У��asiic��ֵ
   TxAsciiBuffer[AsiicMiddleLen+4]=AsiicSum[3];//У��asiic��ֵ
   TxAsciiBuffer[AsiicMiddleLen+5]=0x0D;//asiic��β��ֵ���س�����
   RS485DirSen;//����
   if(HAL_UART_Transmit_IT(&huart1,(uint8_t*)TxAsciiBuffer,AsiicMiddleLen+6)!= HAL_OK)
   {
     Error_Handler();
   }//�����ݷ���ȥ
           
 }

/****************************�������ݽ��չ��̺���********************************/
/****************************�������ݽ��չ��̺���********************************/
//����ص�����
//��������յ�asiic����У��ͷ��β��sumУ��
//У��ͨ����485״̬���RS485_Recok
//RS485_WaitRec
//   |
//   |�յ�ͷ��
//  \|/
//RS485_Recing              
//   |
//   |�յ�β����У��ͨ����
//  \|/
//RS485_Recok//��ѭ���м����������ݲ��ظ�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  unsigned char chktmp[2];
  unsigned char chktmpcac[2]; 
  UNUSED(huart);
  if(huart  == &huart1)//ȷ����usart1
  {
    switch(RS485RunSta)
    {
      case RS485_WaitRec://���ڵȴ����յ���һ���ֽ�
        if(aRxBuffer==PROT_SOI)
        {
          RxBuffer[Rx_count_UART1++] = aRxBuffer;
          RS485RunSta=RS485_Recing;
          break;//����յ�ͷ�� �Ǿͱ�����ڽ���
        }
        break;
      case RS485_Recing: 
         RxBuffer[Rx_count_UART1++] = aRxBuffer;
         
        if(aRxBuffer == 0x0D)
        {
          CheckAsiicSum(RxBuffer,&chktmpcac[0],&chktmpcac[1],Rx_count_UART1);
          Assic2Char(chktmp,&RxBuffer[Rx_count_UART1-5],2);//���յ���ascii�еõ�У����
          if((chktmpcac[0]==chktmp[0])&&(chktmpcac[1]==chktmp[1]))//У��
          {
            RS485RunSta=RS485_Recok;//������ɳ���
 
          }else
          {
            RS485RunSta=RS485_RESET;//У������������ ��λ״̬��
 
          }
        }
        break;
    default:break;
      
      }
    HAL_UART_Receive_IT(&huart1,&aRxBuffer,1); //������һ�ν����ж�
  }
    
}
/****************************������ѭ��RS485ͨ�Ź��̺���************************/
/****************************������ѭ��RS485ͨ�Ź��̺���************************/
//rs485 ��ѭ������շ����̴���
//           /
//RS485_RESET---------------------------------
//   |       \                      /|\      /|\
//   |��ʱ���㸴λ����               |        |
//  \|/                             |        |
//RS485_WaitRec----------��ʱ-------|        |
//   |                              |        |
//   |���ص������յ�ͷ��             |        |
//  \|/                             |        |
//RS485_Recing-----------��ʱ-------|        |                 
//   |                                       |
//   |���ص���������β��У��ͨ����û�г�ʱ    |
//  \|/                                      |
//RS485_Recok �����ظ�----------------------
void RS485Process(void)
{
  unsigned char  len=(Rx_count_UART1-6)/2;
  unsigned char rxcharbuff[20];
  unsigned char Dummy=0x00;
  //////////////////////////��ȡ������Ϣר�ñ���
  unsigned short xLen=0;
  unsigned char TxCharBuffer[8];
  unsigned char txchk[2];
  unsigned char txchkassic[4];
 
  switch(RS485RunSta)
  {
    case RS485_RESET://��λ״̬
      Rx_count_UART1 = 0;//�����ַ����������� �����ַ������±�[]
      RS485RcvWaitTime=0;//��ʱ����
      RS485RcvingTime=0;//��ʱ����
      RS485RunSta=RS485_WaitRec;//ȥ�ȴ������ַ���ͷ����״̬
      break;
    case RS485_WaitRec://�ȴ��е�һ�����ݡ�ͷ��0x7e��
      if(RS485RcvWaitTime>STARTTIMEOUT)//2����û���յ��κζ���
      {
        RS485RcvWaitTime=0;//��ʱ����
        memset(rxcharbuff, 0, sizeof(rxcharbuff));//��ʼ����ս��ջ���
        memset(RxBuffer, 0, sizeof(RxBuffer));//��ʼ����ս��ջ���
        RS485RunSta=RS485_RESET;//û���յ��κ��ֽڻص���λ״̬
        break;
      }
      break;//�ж���û���յ����� �� û�г�ʱ��һֱ�����״̬
    case RS485_Recing://��һ��ͷ�յ���
      
      if(RS485RcvingTime>AFTERTIMEOUT)//500msû������Ļ�����������
      {
        RS485RcvingTime=0;//��ʱ����
        memset(rxcharbuff, 0, sizeof(rxcharbuff));//��ʼ����ս��ջ���
        memset(RxBuffer, 0, sizeof(RxBuffer));//��ʼ����ս��ջ���
        RS485RunSta=RS485_RESET;//���ڽ��ճ�ʱ �յ���һ��ͷ��ʼ��������
        break;
      }
      break;//�ж���û���������� �� û�г�ʱ��һֱ�����״̬
    case RS485_Recok://������� �����Լ��ظ�
      
      Assic2Char(rxcharbuff,&RxBuffer[1],len);//�õ�char [�汾�� addr cid1 cid2 length info] ��Щchar
      
      if(rxcharbuff[0]==PROT_VER&&rxcharbuff[1]==PROT_ADD)//ͨ�Ű汾��ͨ�ŵ�ַ����Ҫ��
      {
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_FACTOR)//��������ȡ������Ϣ//
         {
           xLen=64;//32*2
           EditLengthSum(&xLen);
           TxAsciiBuffer[0]=PROT_SOI;
           
           TxCharBuffer[0]=PROT_VER;
           TxCharBuffer[1]=PROT_ADD;
           TxCharBuffer[2]=PROT_CID1_AC;
           TxCharBuffer[3]=PROT_RTN;
           TxCharBuffer[4]=(xLen&(0xff00))>>8;
           TxCharBuffer[5]=(xLen&(0x00ff))>>0;
           Char2Asiic(TxCharBuffer,&TxAsciiBuffer[1],6);
           strcpy(&TxAsciiBuffer[13],"0123456789012345678902010123456789012345678901234567890123456789");//������Ϣ�汾�ŵ�
           CheckAsiicSum(TxAsciiBuffer,&txchk[0],&txchk[1],82);//81+1
          // 
          // 
           Char2Asiic(txchk,txchkassic,2);
           strcpy(&TxAsciiBuffer[77],txchkassic);
           TxAsciiBuffer[81]=0x0D;
           RS485DirSen;//����
          if(HAL_UART_Transmit_IT(&huart1,(uint8_t*)TxAsciiBuffer,82)!= HAL_OK){Error_Handler();}//�����ݷ���ȥ
         }
         /*******************************AC******************************************/
         /*******************************AC******************************************/
         //21 01 40 41 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//������縡������  [6]��ȡ�ڼ���
         {
           DataMakeSend(GetAcModFloat,(unsigned char*)&AcModFlaot,PROT_CID1_AC,1,sizeof(Ac_ModFlaot),2);//40 41
          
         }
         //21 01 40 43 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_SW)//������翪���� [6]��ȡ�ڼ���
         {
           DataMakeSend(GetAcModSw,(unsigned char*)&AcModSw,PROT_CID1_AC,1,sizeof(Ac_ModSw),1);//40 43  
           
         }
         //21 01 40 44 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//������羯�� [6]��ȡ�ڼ���
         {
           DataMakeSend(GetAcModWarm,(unsigned char*)&AcModWarm,PROT_CID1_AC,1,sizeof(Ac_ModWarm),2);//40 44
           
         }
         //21 01 40 46 00 00  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_PARA_FLOAT)//������ȡ���� 40 46
         {
           DataMakeSend(GetAcModSet,(unsigned char*)&AcModSet,PROT_CID1_AC,1,sizeof(Ac_ModSet),0);//40 46  
         }
         //21 01 42 44 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//ֱ����羯�� [6]��ȡ�ڼ���
         {
           DataMakeSend(GetDcModWarm,(unsigned char*)&DcModWarm,PROT_CID1_DC,1,sizeof(Dc_ModWarm),2);//42 44
           
         }
         //21 01 40 80 00 00
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==0x80)//ac��ȡ�Զ������������Ϣ 40 80
         {
           DataMakeSend(GetAcModOwn,(unsigned char*)&AcModOwn,PROT_CID1_AC,1,sizeof(Ac_ModOwn),0);//40 80  
         }
         
         /*******************************DC******************************************/
         /*******************************DC******************************************/
          //21 01 42 41 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//ֱ����縡������  [6]��ȡ�ڼ���
         {
           DataMakeSend(GetDcModFloat,(unsigned char*)&DcModFloat,PROT_CID1_DC,1,sizeof(Dc_ModFloat),1);//42 41
           
         }
         //21 01 42 46 00 00
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_PARA_FLOAT)//dc������� 42 46
         {
           DataMakeSend(GetDcModSet,(unsigned char*)&DcModSet,PROT_CID1_DC,1,sizeof(Dc_ModSet),0);//42 46  
         }
         
         /*******************************AC2DC***************************************/
         /*******************************AC2DC***************************************/
         //21 01 41 43 00 00  
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_SW)//������翪�� [6]��ȡ�ڼ���
         {
           DataMakeSend(GetAcDcModSw,(unsigned char*)&AcDcModSw,PROT_CID1_AC2DC,1,sizeof(AcDc_ModSw),2);//41 43
           
         }
         //21 01 41 44 00 00
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//������ 41 44
         {
           DataMakeSend(GetAcDcModWarm,(unsigned char*)&AcDcModWarm,PROT_CID1_AC2DC,1,sizeof(AcDc_ModWarm),2);//41 44   
         }
         //21 01 41 41 00 00
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//�������� 41 41
         {
           DataMakeSend(GetAcDcModFloat,(unsigned char*)&AcDcModFloat,PROT_CID1_AC2DC,1,sizeof(AcDc_ModFloat),1);//41 41   
         }
         
         /*******************************�����趨**************************************/
         /*******************************�����趨**************************************/
         //21 01 40 48 60 0A 80-EF xx xx xx xx (���帡����)
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_SET_PARA_FLOAT)//���ý����������
         {
           //�������������
           switch(rxcharbuff[6])
           {
           case 0x80:break;//���ý���������/���ѹ���� 
           case 0x81:break;//���ý���������/���ѹ����
           case 0x82:break;//���ý�������������� 
           case 0x83:break;//����Ƶ������
           case 0x84:break;//����Ƶ������  
           default:break;
           }
           //40 �ظ� �������� ��ȥ��Ӧ��
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC,1,0,0);
         }
         //21 01 40 81 60 0A 80-EF xx xx xx xx (���帡����)
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_SET_PARA_OwnInt)//���ý����Զ������
         {
           //�������������
           switch(rxcharbuff[6])
           {
           case 0xCB:break;//����ģʽ  0:�ֶ� 1:�Զ�
           case 0xCC:break;//��ز���ģʽ 0:�ֶ� 1:�Զ�
           case 0xB4:break;//��ز��Թ��ܿ��� 0���� 1����
           
           default:break;
           }
           //40 �ظ� �������� ��ȥ��Ӧ��
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC,1,0,0);
         }
         
         //21 01 41 45 C0 04 80-EF xx  (�ڼ���ģ��)
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_REMOTE)//���ý����Զ������
         {
           //�������������
           switch(rxcharbuff[6])
           {
           case 0x10:break;//��rxcharbuff[7]ģ������Ϊ ����������
           case 0x1F:break;//��rxcharbuff[7]ģ������Ϊ ����������
           case 0x11:break;//��rxcharbuff[7]ģ������Ϊ ����������
           case 0x20:break;//��rxcharbuff[7]ģ������Ϊ ��������ģ��
           case 0x2F:break;//��rxcharbuff[7]ģ������Ϊ ��������ģ��
           default:break;
           }
           //40 �ظ� �������� ��ȥ��Ӧ��
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC2DC,1,0,0);
         }
         
         //21 01 42 48 60 0A 80-EF xx xx xx xx (���帡����)
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_SET_PARA_FLOAT)//���ý����������
         {
           //�������������
           switch(rxcharbuff[6])
           {
           case 0x80:break;//���� ֱ����ѹ���� 
           case 0x81:break;//���� ֱ����ѹ����
           case 0xE1:break;//���� ���õ���¶ȸ澯ֵ 
           case 0xE2:break;//���� ���û����¶ȸ澯ֵ
           case 0xE3:break;//���� ���ø�ѹ�ػ�ֵ 
           case 0xE4:break;//���� ���þ����ѹ 
           case 0xE5:break;//���� ���ø����ѹ
    
           default:break;
           }
           //40 �ظ� �������� ��ȥ��Ӧ��
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_DC,1,0,0);
         }
      }
      
     RS485RunSta=RS485_RESET;//������ȥ�� 485����ͬʱ�շ� �����ͻ

      break;
      
  }
  
}
 

 //����ص�����
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  UNUSED(huart);
  RS485DirRec;//����ص�������Ϊ��

  //memset(TxBuffer, 0, sizeof(TxBuffer));//������Ҳ��շ��ͻ���
  
}