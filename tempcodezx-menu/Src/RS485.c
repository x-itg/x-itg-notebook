#include "stm32f0xx.h"
#include "RS485.h"
#include "SI4432.h"
#include "SavePara.h"
/**************************以下为协议所需常量定义*****************************/
/**************************以下为协议所需常量定义*****************************/
#define PROT_ADD SavePara.Addr
const unsigned char PROT_SOI                 =   0x7E;//头 不用分解为ascii码
const unsigned char PROT_EOI                 =   0x0D;//头 不用分解为ascii码
const unsigned char PROT_VER                 =   0x21;//本机的通信版本
 const unsigned char PROT_CID1_AC             =   0x40;//交流配电信息
const unsigned char PROT_CID1_AC2DC          =   0x41;//整流器信息
const unsigned char PROT_CID1_DC             =   0x42;//直流配电信息
const unsigned char PROT_CID2_ACQ_DATA_FLOAT =   0x41;//获取浮点数据
const unsigned char PROT_CID2_ACQ_DATA_INT   =   0x42;//获取定点数  × 
const unsigned char PROT_CID2_ACQ_DATA_SW    =   0x43;//获取开关量
const unsigned char PROT_CID2_ACQ_WARM       =   0x44;//获取告警
const unsigned char PROT_CID2_ACQ_REMOTE     =   0x45;//获取遥控 ×
const unsigned char PROT_CID2_ACQ_PARA_FLOAT =   0x46;//获取参数浮点数
const unsigned char PROT_CID2_ACQ_PARA_INT   =   0x47;//获取参数定点数  × 
const unsigned char PROT_CID2_SET_PARA_FLOAT =   0x48;//设定参数定浮点数 
const unsigned char PROT_CID2_SET_PARA_OwnInt=   0x81;//设定参数定自定义
const unsigned char PROT_CID2_SET_PARA_INT   =   0x49;//设定参数定定点数 × 
const unsigned char PROT_CID2_ACQ_VER        =   0x4F;//获取通信版本号
const unsigned char PROT_CID2_ACQ_ADD        =   0x4F;//获取通信版本号
const unsigned char PROT_CID2_FACTOR         =   0x51;//获取厂家信息
const unsigned char PROT_RTN                 =   0x00;//回复状态
const unsigned char PROT_DATAFLAG            =   0x00;//数据状态
const unsigned char PROT_MODNUM              =   0x01;//模块数量
const unsigned char PROT_MODCUR              =   0x01;//模块号码
 
/**************************以下为协议所需内容定义*****************************/
/**************************以下为协议所需内容定义*****************************/
Ac_ModFlaot     AcModFlaot;              //交流浮点数据
Ac_ModWarm      AcModWarm;               //交流警告数据
Ac_ModSw        AcModSw;                 //交流开关数据
Ac_ModSet       AcModSet;                //交流设定参数
Ac_ModOwn       AcModOwn;                //交流自定义数据
Dc_ModFloat     DcModFloat;              //直流浮点数据
Dc_ModWarm      DcModWarm;               //直流告警数据
Dc_ModSet       DcModSet;                //直流设定浮点数
AcDc_ModSw      AcDcModSw;               //交流开关数据
AcDc_ModWarm    AcDcModWarm;             //整流告警数据
AcDc_ModFloat   AcDcModFloat;            //整流模块浮点数
unsigned char   aRxBuffer;              //一个一个收的缓存
unsigned char  RxBuffer[30];            //接收缓存
unsigned char  Rx_count_UART1=0;        //UART1 接收计数器
unsigned char  TxAsciiBuffer[500];      //发送缓冲区
unsigned int   RS485RcvWaitTime=0;      //等待头超时时间
unsigned int   RS485RcvingTime=0;       //接收到头的超时时间
RS485STA RS485RunSta=RS485_RESET;        //RS485 运行状态

/**************************协议框架所需功能函数*****************************/
/**************************协议框架所需功能函数*****************************/
//把4个char 编程一个float
//当收到的时候 float数是低8位 放在最前面的
//所以要把它反过来还 低8位放到最后面去还
void FourChar2Float(float *pfloat,unsigned char *pchar)
{
  unsigned int tmp=0;
   
  tmp=(tmp|(unsigned int)(*pchar++)<<0);
  tmp=(tmp|(unsigned int)(*pchar++)<<8);
  tmp=(tmp|(unsigned int)(*pchar++)<<16);
  tmp=(tmp|(unsigned int)(*pchar++)<<24);
  *pfloat=*(float *)&tmp;
}
//把这个0x16  变成0x31 0x36  长度为1
//len指的是0x16这个的长度
//少 变 多
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
//0x31 0x36 变成 0x16一个
//len指的是0x16这个的长度
//多 变 少
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
//把数据变成assic码
void Data2Assic(unsigned char *source,unsigned char *targetassic,unsigned short len)
{
  unsigned short i=0;
  for(i=0;i<len;i++)
  {
    Char2Asiic(source++,targetassic++,1);
    targetassic++;

  }
}
//所需校验字符串
//得到的校验值高位
//得到的校验值低位
//结尾所在下标
void CheckAsiicSum(unsigned char *pchar,unsigned char *pcharH,unsigned char *pcharL,unsigned short end)
{
          unsigned int chksumint=0;
          unsigned short i;
          pchar++;//把头给忽略掉
          for(i=1;i<(end-5);i++)
          {
            chksumint=chksumint+(*pchar++);//加进去
          }
          chksumint=chksumint&0x0000ffff;//模65536 
          chksumint=(~chksumint);//取反
          chksumint=chksumint+1;//加1
          
          *pcharH=((chksumint&0x0000ff00)>>8);
          *pcharL=((chksumint&0x000000ff)>>0);
          
}
//长度的两个自己的自身校验
//高四位为sum 低12位为发送asiic的出去SOI和EOI的长度
void EditLengthSum(unsigned short *psource)
{
  unsigned char txlenth[3];
  unsigned short pso=*psource;
  unsigned short psotmp;
           txlenth[0]=(pso&(0x00f))>>0;
           txlenth[1]=(pso&(0x0f0))>>4;
           txlenth[2]=(pso&(0xf00))>>8;
           psotmp=txlenth[0]+txlenth[1]+txlenth[2];
           *psource=(((~(psotmp&(0x0f)))+1)<<12)|pso;//对和模16取反+1
}


/******************以下为协议内容所需功能（获取数据）函数***********************/
/******************以下为协议内容所需功能（获取数据）函数***********************/
//获取交流自定义数据
void GetAcModOwn(void)
{
  unsigned char i;
    for(i=0;i<181;i++)
    {
      AcModOwn.Res[i]=0;
    }
}

//空函数
void GetDummy(void)
{
  return;
}
//获取直流参数
void GetDcModSet(void)
{
  DcModSet.DcVHig=(float)SavePara.DcHig/10;//直流高压告警值
  DcModSet.DcVLow=(float)SavePara.DcLow/10;//直流低压告警值
  DcModSet.OwnNum=5;//自定义字节数
  DcModSet.BatTempHig=60;//电池温度告警值
  DcModSet.EnvTempHig=50;//环境温度告警值
  DcModSet.DcVHigOff=60;//高压关机电压
  DcModSet.DcCD1=48;//均充值
  DcModSet.DcCD2=50;//浮充值
}
//获取整理模块浮点数
void GetAcDcModFloat(void)
{
  AcDcModFloat.AcDcV=300;//整流电压
  AcDcModFloat.AcDcNum=1;//整流数目
  AcDcModFloat.AcDcI=17;//电流
  AcDcModFloat.OwnNum=0;//自定义字节数
}
//获取整流告警
void GetAcDcModWarm(void)
{
  AcDcModWarm.WrongSta=2;//故障状态
  AcDcModWarm.OwnNum=3;//3个
  AcDcModWarm.VHigOffSta=1;//高压关机
  AcDcModWarm.CommWrongSta=1;//通信故障状态
  AcDcModWarm.FanWrongSta=1;//风扇故障状态
}
//获取交流设定值
void GetAcModSet(void)
{
  AcModSet.AcVHig=SavePara.AcHig;//交流电压上限
  AcModSet.AcVLow=SavePara.AcLow;//交流电压下限
  AcModSet.AcAHig=100.0;//交流电流上限
  AcModSet.HzHig=60.0;//频率上限
  AcModSet.HzLow=40.0;//频率上限
  AcModSet.OwnNum=0;//自定义数据字节数
}
//获取整流开关量函数
void GetAcDcModSw(void)
{
  AcDcModSw.Onoff=0x00;//开关机
  AcDcModSw.Liu=0x00;//限流、不限流
  AcDcModSw.Cd=0x00;//浮充、均充、测试
  AcDcModSw.OwnNum=0x00;//自定义字节=0
}
//获取交流警告函数
void GetAcModWarm(void)
{
  AcModWarm.VaWarm=0x00;//a相警告
  AcModWarm.VbWarm=0x00;//b相警告
  AcModWarm.VcWarm=0x00;//c相警告
  AcModWarm.FWarm=0x00;//频率警告
  AcModWarm.OpenNumWarm=0x00;//0 断开警告
  AcModWarm.OwnWarmNum=0x01;//0x01 //自定义告警数量 1
  AcModWarm.LeiWarm=0x00;//防雷器警告
}

//获取交流浮点数函数
void GetAcModFloat(void)
{
  //AcModFlaot.Xnum=1;//路数
  AcModFlaot.Va=Slave[0].V1;//交流a相电压
  AcModFlaot.Vb=Slave[0].V2;//交流b相电压
  AcModFlaot.Vc=Slave[0].V3;//交流c相电压
  AcModFlaot.Hz=50;//电流频率
  AcModFlaot.noone=0;//用户自定义数量 这里为0
  AcModFlaot.Ia=Slave[0].I1;//交流a电流
  AcModFlaot.Ib=Slave[0].I1;//交流b电流
  AcModFlaot.Ic=Slave[0].I1;//交流c电流
  

}
//获取直流警告函数
void GetDcModWarm(void)
{
  unsigned char i;
  DcModWarm.DcVwarm=0x00;////直流电压警告
  DcModWarm.DcOpenNum=0x01;//直流数 这里为1
  DcModWarm.DcOpen1=0x00;//开关1
  DcModWarm.OwnWarmNum=20;//自定义告警数量
  for(i=0;i<20;i++)//自定义告警数量
  {
    DcModWarm.OwnWarm[i]=0x00;//自定义告警内容根据实际展开设置
  }
  
}
//获取交流开关函数
void GetAcModSw(void)
{
  AcModSw.SwNum=8;//检测的开关数量
  AcModSw.Sw1Sta=0x1;// 这里为1
  AcModSw.Sw2Sta=0x1;// 这里为1
  AcModSw.Sw3Sta=0x1;// 这里为1
  AcModSw.Sw4Sta=0x1;// 这里为1
  AcModSw.Sw5Sta=0x1;// 这里为1
  AcModSw.Sw6Sta=0x1;// 这里为1
  AcModSw.Sw7Sta=0x1;// 这里为1
  AcModSw.Sw8Sta=0x1;// 这里为1
  AcModSw.Swxx=0;
}

//获取直流浮点函数
void GetDcModFloat(void)
{

  DcModFloat.DcV=(float)MainPara.DcV/10;//12.0;//直流电压
  DcModFloat.DcI=13.0;//直流电流
  DcModFloat.DcNum=1;//直流路数
  DcModFloat.DcBI=14.0;////第一路蓄电池充放电电流
  DcModFloat.DcI1=15.0;//直流分路电流
  DcModFloat.OwmNum=3;////3 自定义数
  DcModFloat.Temp=20.0;//温度
  DcModFloat.BTemp=24.0;//电池温度
  DcModFloat.BV=400.0;//电池容量
  
}


/****************************以下为协议所需数据组装函数********************************/
/****************************以下为协议所需数据组装函数********************************/
/****************************协议部分说明**********************************************/
//功能：数据处理及回复
//CharHead组成：[版本][地址][CID1][LENGTH][DATAFLAG][M]
//其中LENGTH的高4位为 除去头尾的asiic码长度的和校验模16求反+1
//发送char组成：[EOI][CharHead]+[n*Pack]+[2*Check][EOI]
//发送asiic组成：[EOI][CharHead_Asiic]+[n*Pack_Asiic]+[2*Check_Asiic][EOI]
/***************************传入参数说明***********************************************/
//func ：传入函数名作为地址 对应、取得发送数据包pack中数据值对应的函数
//PackStruct：传入数据包pack对应的结构体
//cid1：对应命令的cid1值
//PackNum：数据包pack的长度、可以sizeof(pack对应结构体)
//[DataflagMLen=2]
//[DATAFLAG]：在有些命令没有DATAFLAG时 DataflagMLen=0；（根据协议发现没有[DataFlag]的命令肯定没有[M]）
//[M]：在有些命令时没有[M] DataflagMLen=1；
//[DATAFLAG]和[M]都有时 DataflagMLen=2；
/***************************QQ:572981033**********************************************/
void DataMakeSend(void (*func)(void),unsigned char *PackStruct,unsigned char cid1,unsigned char PackNum,unsigned short PackLen,unsigned short DataflagMLen)
 {
   unsigned char CharHead[8];//字符开始那几个固定的数据
   unsigned char CharSum[2];//结尾校验2个字节的char
   unsigned char AsiicSum[4];//结尾校验4个字节的asiic
   
   unsigned char PackCharBegin=0;//开始放组（包）数据的[]
   unsigned short  CharMiddleLen=0;// 字符长度（除去 头和尾的）
   unsigned short AsiicMiddleLen=0;//字符串变成asiic的长度 （除去 头和尾的）
   unsigned short AsiicLENGTH=0;//LENGTH码
   
   PackCharBegin=DataflagMLen+6;//char开始放pack数据的地方  
   CharMiddleLen=(PackNum*PackLen+DataflagMLen+6);//char除去头尾的长度
   AsiicMiddleLen=CharMiddleLen*2;//除去头尾的asiic码长度
   AsiicLENGTH=AsiicMiddleLen;//发送数据的asiic的长度 这个数据处理后夹带在发送出去
   EditLengthSum(&AsiicLENGTH);//对asiic的长度进行校验处理 在高四位中加入checksum
   TxAsciiBuffer[0]=PROT_SOI;//asiic头直接是“~”符
   CharHead[0]=PROT_VER;//版本号
   CharHead[1]=PROT_ADD;//地址
   CharHead[2]=cid1;//CID1
   CharHead[3]=PROT_RTN;//实时状态RTN
   CharHead[4]=(AsiicLENGTH&(0xff00))>>8;//
   CharHead[5]=(AsiicLENGTH&(0x00ff))>>0;//到这里是所有都有的
   CharHead[6]=PROT_DATAFLAG;//没有没没有上报的报警 0x11 0x10 0x01 0x00
   CharHead[7]=PackNum;//模块数1-5 第几个模块
   Char2Asiic(CharHead,&TxAsciiBuffer[1],PackCharBegin);//把这8个先翻译成asiic
   (*func)();//获取数据函数
   Data2Assic(PackStruct,&TxAsciiBuffer[PackCharBegin*2+1],PackLen*PackNum);//将数据包变成asiic码
   CheckAsiicSum(TxAsciiBuffer,&CharSum[0],&CharSum[1],AsiicMiddleLen+6);// 对asiic中间的数据进行校验得出2个char
   Char2Asiic(CharSum,AsiicSum,2);//再把这两个校验弄成asiic
   TxAsciiBuffer[AsiicMiddleLen+1]=AsiicSum[0];//校验asiic赋值
   TxAsciiBuffer[AsiicMiddleLen+2]=AsiicSum[1];//校验asiic赋值
   TxAsciiBuffer[AsiicMiddleLen+3]=AsiicSum[2];//校验asiic赋值
   TxAsciiBuffer[AsiicMiddleLen+4]=AsiicSum[3];//校验asiic赋值
   TxAsciiBuffer[AsiicMiddleLen+5]=0x0D;//asiic结尾赋值（回车符）
   RS485DirSen;//发送
   if(HAL_UART_Transmit_IT(&huart1,(uint8_t*)TxAsciiBuffer,AsiicMiddleLen+6)!= HAL_OK)
   {
     Error_Handler();
   }//把数据发出去
           
 }

/****************************以下数据接收过程函数********************************/
/****************************以下数据接收过程函数********************************/
//收完回调函数
//在这里对收到asiic进行校验头、尾、sum校验
//校验通过后485状态变成RS485_Recok
//RS485_WaitRec
//   |
//   |收到头后
//  \|/
//RS485_Recing              
//   |
//   |收到尾并且校验通过后
//  \|/
//RS485_Recok//主循环中继续处理数据并回复
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  unsigned char chktmp[2];
  unsigned char chktmpcac[2]; 
  UNUSED(huart);
  if(huart  == &huart1)//确定是usart1
  {
    switch(RS485RunSta)
    {
      case RS485_WaitRec://正在等待接收到第一个字节
        if(aRxBuffer==PROT_SOI)
        {
          RxBuffer[Rx_count_UART1++] = aRxBuffer;
          RS485RunSta=RS485_Recing;
          break;//如果收到头了 那就变成正在接受
        }
        break;
      case RS485_Recing: 
         RxBuffer[Rx_count_UART1++] = aRxBuffer;
         
        if(aRxBuffer == 0x0D)
        {
          CheckAsiicSum(RxBuffer,&chktmpcac[0],&chktmpcac[1],Rx_count_UART1);
          Assic2Char(chktmp,&RxBuffer[Rx_count_UART1-5],2);//从收到的ascii中得到校验码
          if((chktmpcac[0]==chktmp[0])&&(chktmpcac[1]==chktmp[1]))//校验
          {
            RS485RunSta=RS485_Recok;//接收完成成了
 
          }else
          {
            RS485RunSta=RS485_RESET;//校验错误别收了了 复位状态了
 
          }
        }
        break;
    default:break;
      
      }
    HAL_UART_Receive_IT(&huart1,&aRxBuffer,1); //开启下一次接收中断
  }
    
}
/****************************以下主循环RS485通信过程函数************************/
/****************************以下主循环RS485通信过程函数************************/
//rs485 主循环里的收发过程处理
//           /
//RS485_RESET---------------------------------
//   |       \                      /|\      /|\
//   |计时清零复位缓存               |        |
//  \|/                             |        |
//RS485_WaitRec----------超时-------|        |
//   |                              |        |
//   |待回调函数收到头后             |        |
//  \|/                             |        |
//RS485_Recing-----------超时-------|        |                 
//   |                                       |
//   |待回调函数收完尾、校验通过、没有超时    |
//  \|/                                      |
//RS485_Recok 处理及回复----------------------
void RS485Process(void)
{
  unsigned char  len=(Rx_count_UART1-6)/2;
  unsigned char rxcharbuff[20];
  unsigned char Dummy=0x00;
  //////////////////////////获取厂家信息专用变量
  unsigned short xLen=0;
  unsigned char TxCharBuffer[8];
  unsigned char txchk[2];
  unsigned char txchkassic[4];
 
  switch(RS485RunSta)
  {
    case RS485_RESET://复位状态
      Rx_count_UART1 = 0;//接收字符串的索引号 接收字符串的下标[]
      RS485RcvWaitTime=0;//计时清零
      RS485RcvingTime=0;//计时清零
      RS485RunSta=RS485_WaitRec;//去等待接收字符串头部的状态
      break;
    case RS485_WaitRec://等待有第一个数据【头部0x7e】
      if(RS485RcvWaitTime>STARTTIMEOUT)//2秒钟没有收到任何东西
      {
        RS485RcvWaitTime=0;//计时清零
        memset(rxcharbuff, 0, sizeof(rxcharbuff));//初始化清空接收缓存
        memset(RxBuffer, 0, sizeof(RxBuffer));//初始化清空接收缓存
        RS485RunSta=RS485_RESET;//没有收到任何字节回到复位状态
        break;
      }
      break;//中断中没有收到数据 且 没有超时就一直在这个状态
    case RS485_Recing://第一个头收到后
      
      if(RS485RcvingTime>AFTERTIMEOUT)//500ms没有收完的话就重新来过
      {
        RS485RcvingTime=0;//计时清零
        memset(rxcharbuff, 0, sizeof(rxcharbuff));//初始化清空接收缓存
        memset(RxBuffer, 0, sizeof(RxBuffer));//初始化清空接收缓存
        RS485RunSta=RS485_RESET;//正在接收超时 收到第一个头后开始收内容了
        break;
      }
      break;//中断中没有收完数据 且 没有超时就一直在这个状态
    case RS485_Recok://接收完成 解析以及回复
      
      Assic2Char(rxcharbuff,&RxBuffer[1],len);//得到char [版本号 addr cid1 cid2 length info] 这些char
      
      if(rxcharbuff[0]==PROT_VER&&rxcharbuff[1]==PROT_ADD)//通信版本和通信地址符合要求
      {
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_FACTOR)//交流配电获取厂家信息//
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
           strcpy(&TxAsciiBuffer[13],"0123456789012345678902010123456789012345678901234567890123456789");//厂家信息版本号等
           CheckAsiicSum(TxAsciiBuffer,&txchk[0],&txchk[1],82);//81+1
          // 
          // 
           Char2Asiic(txchk,txchkassic,2);
           strcpy(&TxAsciiBuffer[77],txchkassic);
           TxAsciiBuffer[81]=0x0D;
           RS485DirSen;//发送
          if(HAL_UART_Transmit_IT(&huart1,(uint8_t*)TxAsciiBuffer,82)!= HAL_OK){Error_Handler();}//把数据发出去
         }
         /*******************************AC******************************************/
         /*******************************AC******************************************/
         //21 01 40 41 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//交流配电浮点数据  [6]获取第几组
         {
           DataMakeSend(GetAcModFloat,(unsigned char*)&AcModFlaot,PROT_CID1_AC,1,sizeof(Ac_ModFlaot),2);//40 41
          
         }
         //21 01 40 43 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_SW)//交流配电开关量 [6]获取第几组
         {
           DataMakeSend(GetAcModSw,(unsigned char*)&AcModSw,PROT_CID1_AC,1,sizeof(Ac_ModSw),1);//40 43  
           
         }
         //21 01 40 44 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//交流配电警告 [6]获取第几组
         {
           DataMakeSend(GetAcModWarm,(unsigned char*)&AcModWarm,PROT_CID1_AC,1,sizeof(Ac_ModWarm),2);//40 44
           
         }
         //21 01 40 46 00 00  
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_ACQ_PARA_FLOAT)//交流获取参数 40 46
         {
           DataMakeSend(GetAcModSet,(unsigned char*)&AcModSet,PROT_CID1_AC,1,sizeof(Ac_ModSet),0);//40 46  
         }
         //21 01 42 44 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//直流配电警告 [6]获取第几组
         {
           DataMakeSend(GetDcModWarm,(unsigned char*)&DcModWarm,PROT_CID1_DC,1,sizeof(Dc_ModWarm),2);//42 44
           
         }
         //21 01 40 80 00 00
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==0x80)//ac获取自定义参数命令信息 40 80
         {
           DataMakeSend(GetAcModOwn,(unsigned char*)&AcModOwn,PROT_CID1_AC,1,sizeof(Ac_ModOwn),0);//40 80  
         }
         
         /*******************************DC******************************************/
         /*******************************DC******************************************/
          //21 01 42 41 e0 02 01  
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//直流配电浮点数据  [6]获取第几组
         {
           DataMakeSend(GetDcModFloat,(unsigned char*)&DcModFloat,PROT_CID1_DC,1,sizeof(Dc_ModFloat),1);//42 41
           
         }
         //21 01 42 46 00 00
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_ACQ_PARA_FLOAT)//dc浮点参数 42 46
         {
           DataMakeSend(GetDcModSet,(unsigned char*)&DcModSet,PROT_CID1_DC,1,sizeof(Dc_ModSet),0);//42 46  
         }
         
         /*******************************AC2DC***************************************/
         /*******************************AC2DC***************************************/
         //21 01 41 43 00 00  
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_SW)//整流配电开关 [6]获取第几组
         {
           DataMakeSend(GetAcDcModSw,(unsigned char*)&AcDcModSw,PROT_CID1_AC2DC,1,sizeof(AcDc_ModSw),2);//41 43
           
         }
         //21 01 41 44 00 00
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_WARM)//整理警告 41 44
         {
           DataMakeSend(GetAcDcModWarm,(unsigned char*)&AcDcModWarm,PROT_CID1_AC2DC,1,sizeof(AcDc_ModWarm),2);//41 44   
         }
         //21 01 41 41 00 00
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_DATA_FLOAT)//整流浮点 41 41
         {
           DataMakeSend(GetAcDcModFloat,(unsigned char*)&AcDcModFloat,PROT_CID1_AC2DC,1,sizeof(AcDc_ModFloat),1);//41 41   
         }
         
         /*******************************参数设定**************************************/
         /*******************************参数设定**************************************/
         //21 01 40 48 60 0A 80-EF xx xx xx xx (具体浮点数)
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_SET_PARA_FLOAT)//设置交流浮点参数
         {
           //在这里解析命令
           switch(rxcharbuff[6])
           {
           case 0x80:break;//设置交流输入线/相电压上限 
           case 0x81:break;//设置交流输入线/相电压下限
           case 0x82:break;//设置交流输出电流上限 
           case 0x83:break;//设置频率上限
           case 0x84:break;//设置频率下限  
           default:break;
           }
           //40 回复 解析好了 回去个应答
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC,1,0,0);
         }
         //21 01 40 81 60 0A 80-EF xx xx xx xx (具体浮点数)
         if(rxcharbuff[2]==PROT_CID1_AC&&rxcharbuff[3]==PROT_CID2_SET_PARA_OwnInt)//设置交流自定义参数
         {
           //在这里解析命令
           switch(rxcharbuff[6])
           {
           case 0xCB:break;//均充模式  0:手动 1:自动
           case 0xCC:break;//电池测试模式 0:手动 1:自动
           case 0xB4:break;//电池测试功能开关 0：关 1：开
           
           default:break;
           }
           //40 回复 解析好了 回去个应答
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC,1,0,0);
         }
         
         //21 01 41 45 C0 04 80-EF xx  (第几个模块)
         if(rxcharbuff[2]==PROT_CID1_AC2DC&&rxcharbuff[3]==PROT_CID2_ACQ_REMOTE)//设置交流自定义参数
         {
           //在这里解析命令
           switch(rxcharbuff[6])
           {
           case 0x10:break;//第rxcharbuff[7]模块设置为 整流器均充
           case 0x1F:break;//第rxcharbuff[7]模块设置为 整流器浮充
           case 0x11:break;//第rxcharbuff[7]模块设置为 整流器测试
           case 0x20:break;//第rxcharbuff[7]模块设置为 开整流器模块
           case 0x2F:break;//第rxcharbuff[7]模块设置为 关整流器模块
           default:break;
           }
           //40 回复 解析好了 回去个应答
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_AC2DC,1,0,0);
         }
         
         //21 01 42 48 60 0A 80-EF xx xx xx xx (具体浮点数)
         if(rxcharbuff[2]==PROT_CID1_DC&&rxcharbuff[3]==PROT_CID2_SET_PARA_FLOAT)//设置交流浮点参数
         {
           //在这里解析命令
           switch(rxcharbuff[6])
           {
           case 0x80:break;//设置 直流电压上限 
           case 0x81:break;//设置 直流电压下限
           case 0xE1:break;//设置 设置电池温度告警值 
           case 0xE2:break;//设置 设置环境温度告警值
           case 0xE3:break;//设置 设置高压关机值 
           case 0xE4:break;//设置 设置均充电压 
           case 0xE5:break;//设置 设置浮充电压
    
           default:break;
           }
           //40 回复 解析好了 回去个应答
           DataMakeSend(GetDummy,(unsigned char*)&Dummy,PROT_CID1_DC,1,0,0);
         }
      }
      
     RS485RunSta=RS485_RESET;//发完再去收 485不能同时收发 避免冲突

      break;
      
  }
  
}
 

 //发完回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  UNUSED(huart);
  RS485DirRec;//发完回调后设置为收

  //memset(TxBuffer, 0, sizeof(TxBuffer));//发完了也清空发送缓存
  
}