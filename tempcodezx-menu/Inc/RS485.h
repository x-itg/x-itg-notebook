#ifndef __RS485_H
#define __RS485_H	
#include "string.h"
extern unsigned char aRxBuffer;//以一个字节接收字符的缓存
extern UART_HandleTypeDef huart1;//串口1 句柄
extern void Error_Handler(void);//错误处理函数
void RS485Process(void);//RS485 主循环函数
/***************************RS485状态**************************************/
typedef enum {

	RS485_RESET,           //初始化
        RS485_WaitRec,         //等待接收 头
        RS485_Recing,          //收到头后正在接收 数据及结尾
        RS485_Recok            //校验收到尾、并通过校验 就可以去组装数据并发送
  }RS485STA;
/***************************RS485宏定义**************************************/
#define RS485DirSen GPIOA->BSRR=1<<12//设置rs485芯片为发送状态
#define RS485DirRec GPIOA->BRR=1<<12//设置rs485芯片为接收状态
#define STARTTIMEOUT 2000 //开始收到头之前的超时
#define AFTERTIMEOUT      500  //收到头后 500ms应该收完整个数据
/***************************结构体内存排列***********************************/
#pragma pack(1)//下面结构体需要 的对齐方式为以1个字节对齐  这个必须得放不然sizeof计算的时候会变多的
/****************************************************************************/
typedef  struct XX{	
  //unsigned char Xnum;//路数 为1
  float Va;//交流电压a
  float Vb;//交流电压b
  float Vc;//交流电压c
  float Hz;//交流频率
  unsigned char noone;//自定义字节数 为0
  float Ia;//交流电流a
  float Ib;//交流电流b
  float Ic;//交流电流c
}Ac_ModFlaot;//交流浮点数据
/****************************************************************************/
typedef  struct dcmod{	
  unsigned char DcVwarm;//直流电压警告
  unsigned char DcOpenNum;//直流数 这里为1
  unsigned char DcOpen1;//开关1
  unsigned char OwnWarmNum;//自定义告警数量
  unsigned char OwnWarm[20];//自定义告警内容
}Dc_ModWarm;//直流告警数据
/****************************************************************************/
typedef  struct acswmod{	
  unsigned char SwNum;//检测的开关数量
  unsigned char Sw1Sta;// 
  unsigned char Sw2Sta;// 
  unsigned char Sw3Sta;// 
  unsigned char Sw4Sta;// 
  unsigned char Sw5Sta;// 
  unsigned char Sw6Sta;// 
  unsigned char Sw7Sta;// 
  unsigned char Sw8Sta;// 
  unsigned char Swxx;//
  //unsigned char OwnNum;//自定义数 ==3  
  //unsigned char OwnSw[3];//组定义的开关量
}Ac_ModSw;//交流开关量
/****************************************************************************/
typedef  struct dcfmod{	
  float DcV;//dc v
  float DcI;//dc i
  unsigned char DcNum;//1
  float DcBI;//第一路蓄电池充放电电流
  float DcI1;//直流分路
  unsigned OwmNum;//3 自定义数
  float Temp;//温度
  float BTemp;//电池温度
  float BV;//第一路电池剩余容量
}Dc_ModFloat;//直流浮点数据
/****************************************************************************/
typedef  struct acmod{	
  unsigned char VaWarm;//a相警告
  unsigned char VbWarm;//b相警告
  unsigned char VcWarm;//c相警告
  unsigned char FWarm;//频率警告
  unsigned char OpenNumWarm;//0 断开警告
  unsigned char OwnWarmNum;//自定义告警数量 1
  unsigned char LeiWarm;//0x00
}Ac_ModWarm;//交流告警数据
/****************************************************************************/
typedef  struct acdcmod{	
  unsigned char Onoff;//开关机
  unsigned char Liu;//限流、不限流
  unsigned char Cd;//浮充、均充、测试
  unsigned char OwnNum;//自定义字节=0
}AcDc_ModSw;//整流开关量
/****************************************************************************/
typedef  struct acfset{	
  float AcVHig;//交流电压上限
  float AcVLow;//交流电压下限
  float AcAHig;//交流电流上限
  float HzHig;//频率上限
  float HzLow;//频率下限
  unsigned char OwnNum;//自定义数据字节数
}Ac_ModSet;//交流参数
/****************************************************************************/
typedef  struct acdcwarm{	
  unsigned char WrongSta;//故障状态
  unsigned char OwnNum;//自定义字节数目3
  unsigned char VHigOffSta;//高压关机
  unsigned char CommWrongSta;//通信故障状态
  unsigned char FanWrongSta;//风扇故障状态
}AcDc_ModWarm;//整流告警
/****************************************************************************/
typedef  struct acdcf{	
   float AcDcV;//整流电压
   unsigned char AcDcNum;//整流模块数量 这里设置为1
   float AcDcI;//模块输出电流
   unsigned char OwnNum;//用户自定义字节数 这里为0
}AcDc_ModFloat;//整流浮点数据
/****************************************************************************/
typedef  struct dcmodset{	
   float DcVHig;//直流电压上限
   float DcVLow;//直流电压下限
   unsigned char OwnNum;//自动字节数这里是5
   float BatTempHig;//电池高温
   float EnvTempHig;//环境高温
   float DcVHigOff;//高压关机值
   float DcCD1;//平充
   float DcCD2;//浮充
}Dc_ModSet;//直流参数
/****************************************************************************/
typedef  struct acmdown{	
 
   unsigned char  Res[181];//保留字节
  
}Ac_ModOwn;//交流自定义参数

#endif