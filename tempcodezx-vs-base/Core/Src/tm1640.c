

#include "TM1640.h"
#include "control.h"
#include "24c02.h"
unsigned char dis[] = {
    0x3F, // 0  0
    0x06, // 1  1
    0x5B, // 2  2
    0x4F, // 3  3
    0x66, // 4  4
    0x6d, // 5  5
    0x7D, // 6  6
    0x07, // 7  7
    0x7F, // 8  8
    0x6F, // 9  9
    0x77, // A  10
    0x7C, // b  11
    0x39, // C  12
    0x5E, // d  13
    0x79, // E  14
    0x71, // F  15
    0x00, //   16
    0xff, //   17
    0xBF, // 0. 18
    0x86, // 1. 19
    0xDB, // 2. 20
    0xCF, // 3. 21
    0xE6, // 4. 22
    0xEd, // 5. 23
    0xFD, // 6. 24
    0x87, // 7. 25
    0xFF, // 8. 26
    0xEF, // 9. 27
    0x73, // P  28
    0x40, //-  29
    0x74, // h  30
    0x76, // H  31
    0x38, // L  32

}; // 共阴极

/********  TM1640 GPIO 配置 *************************/
void TM1640_GPIO_Config(void)
{
  TM1640SLK_HING;
  TM1640DAT_HING;
}

//*************************************
// 函数名称：_Nopt
// 函数功能：延时函数
// 入口参数：延时时间
// 出口参数：无
//***************************************/
void delay_us(unsigned short T_Dly)
{
  unsigned int cnt = 3;
  while (T_Dly--)
  {
    cnt--;
    cnt--;
    cnt--;

    ;
    cnt = 0;
  }
  return;
}

/****************************************
起始函数
***************************************/
void TM1640Start(void)
{
  TM1640SLK_HING;
  delay_us(2);
  TM1640DAT_HING;
  delay_us(2);
  TM1640DAT_LOW;
  delay_us(2);
}
/*************************************
结束函数
***************************************/
void TM1640Stop(void)
{
  TM1640SLK_LOW;
  delay_us(2);
  TM1640DAT_LOW;
  delay_us(2);
  TM1640SLK_HING;
  delay_us(2);
  TM1640DAT_HING;
}
/*************************************
TM1640WriteByte
写一字节数据  date 为所要写的数据
***************************************/
void TM1640WriteByte(u8 date)
{
  u8 i;
  for (i = 0; i < 8; i++)
  {
    TM1640SLK_LOW;
    delay_us(2);
    if (date & 0x01) // 先低位后高位
    {
      TM1640DAT_HING;
      delay_us(3);
    }
    else
    {
      TM1640DAT_LOW;
      delay_us(3);
    }
    date = date >> 1; // 数据右移一位
    TM1640SLK_HING;
    delay_us(3);
  }
}
/***************************************
发送数组
Addr1640：起始地址值
*a : 所要发送的数组
ValueLong:想要发送的数据长度
适用于地址自加1模式
**************************************/
void TM1640_SendData(u8 Addr1640, u8 *a, u8 ValueLong)
{
  u8 i;
  TM1640Start();
  TM1640WriteByte(Addr1640);

  for (i = 0; i < ValueLong; i++)
  {
    TM1640WriteByte(a[i]);
  }
  TM1640Stop();
}

unsigned char tmNum[7] = {0, 0, 0, 0, 0, 0, 0};

void SendData(void)
{

  TM1640Start();
  TM1640WriteByte(0x44);
  TM1640Stop();

  TM1640Start();
  TM1640WriteByte(0xC0);
  TM1640WriteByte(dis[tmNum[0]]);
  TM1640Stop();
  TM1640Start();
  TM1640WriteByte(0xC1);
  TM1640WriteByte(dis[tmNum[1]]);
  TM1640Stop();
  TM1640Start();
  TM1640WriteByte(0xC2);
  TM1640WriteByte(dis[tmNum[2]]);
  TM1640Stop();
  TM1640Start();
  TM1640WriteByte(0xC3);
  TM1640WriteByte(dis[tmNum[3]]);
  TM1640Stop();
  TM1640Start();
  TM1640WriteByte(0xC4);
  TM1640WriteByte(dis[tmNum[4]]);
  TM1640Stop();
  TM1640Start();
  TM1640WriteByte(0xC5);
  TM1640WriteByte(tmNum[5]);
  TM1640Stop();

  TM1640Start();
  TM1640WriteByte(0x8b);
  TM1640Stop();
}

signed int Ta = 162; // 三个温度
signed int Tb = 167;
signed int Tc = 166;

signed int mmTa = 162; // 三个温度
signed int mmTb = 167;
signed int mmTc = 166;

signed int pTa = 0; // 断电前三个温度
signed int pTb = 0;
signed int pTc = 0;

unsigned int FenH = 0; // 风机定时  每隔多少小时
unsigned int FenM = 0; // 风机定时  持续转几分钟
signed int Tcala1 = 0; // 温度补偿
signed int Tcala2 = 0;
signed int Tcala3 = 0;
signed int Tset1 = 1000; // 温度阈值
signed int Tset2 = 800;
signed int Tset3 = 1300;
signed int Tset4 = 1500;
signed int cshuiCha = 50; // 除湿回差 三个回差值
signed int cwhuiCha = 50; // 超温报警回差
signed int tzhuiCha = 50; // 跳闸回差
signed int MnTest = 0;    // 测试 温度值
unsigned int jcBootCnt = 0;
unsigned int addr = 1;
// 0 无
// 1 偶
// 2 奇
unsigned int jojy = 0;
unsigned int btlv = 0;
// 0 1200
// 1 2400
// 2 4800
// 3 9600
// 4 19200

unsigned short AdDataOrig[500][3];
unsigned short AdcValue[3];
unsigned int readKeyTime = 0;
unsigned int oneMinuteCnt = 0;
unsigned int one1000msCnt = 0;
unsigned int KeyV = 0;
unsigned char JDQFLAG = 0;
unsigned char JDQSTAT = 1;
unsigned char JDQCHAN = 0;
unsigned int JDQTIME = 0;
unsigned char TestModeFlag = 0;
unsigned int LogTime = 0;
float k1 = 11.0;
float k2 = 11.0;
float k3 = 11.0;
float b1 = 2705.0;
float b2 = 2705.0;
float b3 = 2705.0;
float xa1 = 0;
float xa2 = 0;
float xb1 = 0;
float xb2 = 0;
float xc1 = 0;
float xc2 = 0;
#define BcValHig (999) // 温度补偿
#define BcValLow (-999)
#define TsetValHig (2000) // 温度设定最高
#define TsetValLow (0)
#define MNValHig (2500) // 模拟温度最高
#define MNValLow (0)
#define AddrHig (240)
#define AddrLow (1)
#define HCHig 100 // 回差最高
#define HCLow 5
#define TMAX (2400)
#define TMIN (-300)
#define XHTIME 300 // 100*50=5000ms=5秒
#define addrFenH 0
#define addrTcala1 4
#define addrTcala2 8
#define addrTcala3 12
#define addrTset1 16
#define addrTset2 20
#define addrTset3 24
#define addrTset4 28
#define addrcshuiCha 32
#define addrcwhuiCha 36
#define addrtzhuiCha 40
#define addraddr 44
#define addrjojy 48
#define addrbtlv 52
#define addrFenM 56
#define addrk1 60
#define addrk2 64
#define addrk3 68
#define addrb1 72
#define addrb2 76
#define addrb3 80
void LoadPara(void)
{
  readAT24C16(addrFenH, (unsigned char *)&FenH, 4);
  if (FenH > 24)
    FenH = 0;
  readAT24C16(addrFenM, (unsigned char *)&FenM, 4);
  if (FenM > 60)
    FenM = 0;
  readAT24C16(addrTcala1, (unsigned char *)&Tcala1, 4);
  if (Tcala1 < -1000 || Tcala1 > 1000)
    Tcala1 = 0;
  readAT24C16(addrTcala2, (unsigned char *)&Tcala2, 4);
  if (Tcala2 < -1000 || Tcala2 > 1000)
    Tcala2 = 0;
  readAT24C16(addrTcala3, (unsigned char *)&Tcala3, 4);
  if (Tcala3 < -1000 || Tcala3 > 1000)
    Tcala3 = 0;

  readAT24C16(addrk1, (unsigned char *)&k1, 4);
  readAT24C16(addrb1, (unsigned char *)&b1, 4);
  readAT24C16(addrk2, (unsigned char *)&k2, 4);
  readAT24C16(addrb2, (unsigned char *)&b2, 4);
  readAT24C16(addrk3, (unsigned char *)&k3, 4);
  readAT24C16(addrb3, (unsigned char *)&b3, 4);

  readAT24C16(addrTset1, (unsigned char *)&Tset1, 4);
  if (Tset1 < TsetValLow || Tset1 > TsetValHig)
    Tset1 = 1000;
  readAT24C16(addrTset2, (unsigned char *)&Tset2, 4);
  if (Tset2 < TsetValLow || Tset2 > TsetValHig)
    Tset2 = 800;
  readAT24C16(addrTset3, (unsigned char *)&Tset3, 4);
  if (Tset3 < TsetValLow || Tset3 > TsetValHig)
    Tset3 = 1300;
  readAT24C16(addrTset4, (unsigned char *)&Tset4, 4);
  if (Tset4 < TsetValLow || Tset4 > TsetValHig)
    Tset4 = 1500;

  readAT24C16(addrcshuiCha, (unsigned char *)&cshuiCha, 4);
  if (cshuiCha < HCLow || cshuiCha > HCHig)
    cshuiCha = 4;
  readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
  if (cwhuiCha < HCLow || cwhuiCha > HCHig)
    cwhuiCha = 4;
  readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
  if (tzhuiCha < HCLow || tzhuiCha > HCHig)
    tzhuiCha = 4;

  readAT24C16(addraddr, (unsigned char *)&addr, 4);
  if (addr > 254)
    addr = 1;
  readAT24C16(addrjojy, (unsigned char *)&jojy, 4);
  if (jojy > 2)
    jojy = 0;
  readAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
  if (btlv > 4)
    btlv = 3;
}

#include <stdio.h>

void updateValues(signed int *PreTa, signed int *PreTb, signed int *PreTc, signed int *mmPreTa, signed int *mmPreTb, signed int *mmPreTc)
{
  signed int maxVal = *PreTa;
  signed int minVal = *PreTa;

  // 找到PreTa、PreTb和PreTc中的最大值和最小值
  if (*PreTb > maxVal)
  {
    maxVal = *PreTb;
  }
  if (*PreTc > maxVal)
  {
    maxVal = *PreTc;
  }
  if (*PreTb < minVal)
  {
    minVal = *PreTb;
  }
  if (*PreTc < minVal)
  {
    minVal = *PreTc;
  }

  // 判断最大值与最小值之间的差是否小于50
  if ((maxVal - minVal) < 50)
  {
    // 将Ta、Tb和Tc都设置为最小值
    *mmPreTa = minVal;
    *mmPreTb = minVal;
    *mmPreTc = minVal;
  }
  else
  {
    *mmPreTa = *PreTa;
    *mmPreTb = *PreTb;
    *mmPreTc = *PreTc;
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{

  unsigned int i, j;
  unsigned int tmp[3] = {0, 0, 0};
  static unsigned int cnt = 0;
  static unsigned int max[3] = {0, 0, 0};
  for (j = 0; j < 3; j++)
  {
    tmp[j] = 0;
    for (i = 0; i < 500; i++)
    {
      tmp[j] = tmp[j] + AdDataOrig[i][j];
    }
    AdcValue[j] = tmp[j] / 500;

    tmp[j] = 0;
  }
  cnt++;

  if (cnt > 1000) // 3000
  {
    cnt = 0;

    if (TestModeFlag == 0)
    {
#if 0
      float x1[3]={0,0,0};//0度adc
      float x2[3]={0,0,0};//200度adc
      float x3[3]={0,0,0};//中点
      float x4[3]={0,0,0};//修正系数
      float va[3];//此时的adc
      x1[0]=b1/k1;
      x1[1]=b2/k2;
      x1[2]=b3/k3;
      x2[0]=(b1+2000.0)/k1;
      x2[1]=(b2+2000.0)/k2;
      x2[2]=(b3+2000.0)/k3;
      x3[0]=(x1[0]+x2[0])/2;
      x3[1]=(x1[1]+x2[1])/2;
      x3[2]=(x1[2]+x2[2])/2;
      //max[2]=x2[0];
      //max[0]=x2[1];
      //max[1]=x2[2];
      va[0]=(float)max[2];
      va[1]=(float)max[0];
      va[2]=(float)max[1];
      if(va[0]<x3[0])x4[0]=140.0*va[0]/(x2[0]-x1[0])-140.0*x1[0]/(x2[0]-x1[0]);else 140.0-140.0*va[0]/(x2[0]-x1[0])+140.0*x1[0]/(x2[0]-x1[0]);
      if(va[1]<x3[1])x4[1]=140.0*va[1]/(x2[1]-x1[1])-140.0*x1[1]/(x2[1]-x1[1]);else 140.0-140.0*va[1]/(x2[1]-x1[1])+140.0*x1[1]/(x2[1]-x1[1]);
      if(va[2]<x3[2])x4[2]=140.0*va[2]/(x2[2]-x1[2])-140.0*x1[2]/(x2[2]-x1[2]);else 140.0-140.0*va[2]/(x2[2]-x1[2])+140.0*x1[2]/(x2[2]-x1[2]);
#endif
#if 1
      if (jcBootCnt >= 500) // 8000 开机自检 500开机不自检测
      {
        jcBootCnt = 500;
        if (JDQSTAT == 0)
          Ta = (signed int)((k1 * (float)max[2] - b1) + (float)Tcala1); //+(float)Tcala1;//
        if (JDQSTAT == 0)
          Tb = (signed int)((k2 * (float)max[0] - b2) + (float)Tcala2); //+(float)Tcala2;//
        if (JDQSTAT == 0)
          Tc = (signed int)((k3 * (float)max[1] - b3) + (float)Tcala3); //+(float)Tcala3;//
      }
      else
      {
        Ta = max[2] / 2;
        Tb = max[0] / 2;
        Tc = max[1] / 2;
      }

#endif
      // Ta=10*AdcValue[5]-Tcala1;
      // Tb=10*AdcValue[4]-Tcala2;
      // Tc=10*AdcValue[3]-Tcala3;
    }
    else
    {
      Ta = MnTest;
      Tb = MnTest;
      Tc = MnTest;
    }
    max[0] = 0;
    max[1] = 0;
    max[2] = 0;
  }
  else
  {
    if (max[0] < AdcValue[0])
      max[0] = AdcValue[0];
    if (max[1] < AdcValue[1])
      max[1] = AdcValue[1];
    if (max[2] < AdcValue[2])
      max[2] = AdcValue[2];
  }

  if (Ta >= -30 && Ta <= 30)
    Ta = 0;
  if (Ta >= 290 && Ta <= 330)
    Ta = 312;
  if (Ta >= 1980 && Ta <= 2030)
    Ta = 2002;
  if (Tb >= -30 && Tb <= 30)
    Tb = -1;
  if (Tb >= 290 && Tb <= 330)
    Tb = 311;
  if (Tb >= 1980 && Tb <= 2030)
    Tb = 2002;
  if (Tc >= -30 && Tc <= 30)
    Tc = 2;
  if (Tc >= 290 && Tc <= 330)
    Tc = 309;
  if (Tc >= 1980 && Tc <= 2030)
    Tc = 2001;

  updateValues(&Ta, &Tb, &Tc, &mmTa, &mmTb, &mmTc);

  // PreTa PreTb PreTc 其中最大-最小的数值>50

  return;
}
void shValue(signed int Num, unsigned char dp, unsigned char mode)
{
  char shStr[4];
  if (Num <= 10000 && Num >= -999)
  {
    if (mode == 0)
      sprintf(shStr, "%4d", Num);
    if (mode == 1)
      sprintf(shStr, "%01d", Num);
    if (mode == 2)
      sprintf(shStr, "%02d", Num);
    if (mode == 3)
      sprintf(shStr, "%03d", Num);
    if (mode == 4)
      sprintf(shStr, "%04d", Num);

    if (shStr[0] == '-')
      tmNum[1] = 29;
    else if (shStr[0] == ' ')
      tmNum[1] = 16;
    else
      tmNum[1] = shStr[0] - '0';

    if (shStr[1] == '-')
      tmNum[2] = 29;
    else if (shStr[1] == ' ')
      tmNum[2] = 16;
    else
      tmNum[2] = shStr[1] - '0';

    if (shStr[2] == '-')
      tmNum[3] = 29;
    else if (shStr[2] == ' ')
      tmNum[3] = 16;
    else
      tmNum[3] = shStr[2] - '0';

    if (shStr[3] == '-')
      tmNum[4] = 29;
    else if (shStr[3] == ' ')
      tmNum[4] = 16;
    else
      tmNum[4] = shStr[3] - '0';
    if (mode == 0 && Num < 10 && Num > -10)
      tmNum[3] = 18; // 0.
    if (mode == 4 && Num < 10 && Num > -10)
      tmNum[3] = 18; // 0.

    if (dp == 1 && Num >= 10)
      tmNum[3] = tmNum[3] + 18;
    if (dp == 2 && Num >= 100)
      tmNum[2] = tmNum[2] + 18;
    if (dp == 3 && Num >= 1000)
      tmNum[1] = tmNum[1] + 18;

    if (dp == 1 && Num <= -10)
      tmNum[3] = tmNum[3] + 18;
    if (dp == 2 && Num <= -100)
      tmNum[2] = tmNum[2] + 18;
  }
  else
  {
    tmNum[1] = 9;
    tmNum[2] = 9;
    tmNum[3] = 9;
    tmNum[4] = 9;
  }
}

// 0     风机
// 1     故障
// 2     超温报警
// 3     跳闸
// 4     门禁
// 5     a温度故障0 1过低 2过高
// 6     b温度故障0 1过低 2过高
// 7     c温度故障0 1过低 2过高
// 8     a没电故障0 1没电
// 9     b没电故障0 1没电
// 12    0自动     1手动开 2手动关
// 13    0巡回     1最大
// 14    0不叫     1叫  beep
// 15    0不消音   1消音
// 16    0巡回     1最大温度显示
unsigned char macFlag[17] = {0}; // 最终执行的标价  macFlag[12] 0自动 1手动开 2手动关

void UpateShData(void)
{
  static unsigned char Cnt1 = 0;
  static unsigned char Cnt2 = 0;
  static unsigned char Cnt3 = 0;
  static unsigned char Cnt4 = 0;
  static unsigned int lCnt1 = 0;
  static unsigned int lCnt2 = 0;
  static unsigned int lCnt3 = 0;
  static unsigned int lCnt4 = 0;
  static signed int Pcnt = 1;
  static unsigned int LoopCnt = 0;
  static unsigned int xunhuiCnt = 0;

  unsigned char LedFlag = 0;
  if (LogTime >= 2000) // 开机2秒后开始采集最大值
  {
    LogTime = 2000;
    if (Ta > pTa)
      pTa = Ta;
    if (Tb > pTb)
      pTb = Tb;
    if (Tc > pTc)
      pTc = Tc;
  }
  // 风机
  if (Ta > Tset1 || Tb > Tset1 || Tc > Tset1) // 启动温度
  {
    macFlag[0] = 1; // 转
  }
  if (Ta < Tset2 && Tb < Tset2 && Tc < Tset2) // 停止温度
  {
    macFlag[0] = 0; // 不转
  }

  if (macFlag[12] == 1) // 手动开
    macFlag[0] = 1;     // 转
  if (macFlag[12] == 2) // 手动关
    macFlag[0] = 0;     // 不转
  if (macFlag[12] > 2)  // 自动 手动开 手动关
    macFlag[12] = 0;

  // 自动的情况下定时补转风机
  // if(one1000msCnt>1000)
  {
    // one1000msCnt=0;
    // 自动
    if (macFlag[12] == 0 && oneMinuteCnt > FenH * 60 && oneMinuteCnt < (FenH * 60 + FenM))
    {
      if (FenH != 0 && FenM != 0)
        macFlag[0] = 1; // 转
    }
    if (oneMinuteCnt > (FenH * 60 + FenM))
    {
      oneMinuteCnt = 0;
    }
  }
  // 取得故障类型
  if (Ta < TMIN || Ta > TMAX)
  {
    if (Ta < TMIN)
      macFlag[5] = 1;
    if (Ta > TMAX)
      macFlag[5] = 2;
  }
  else
  {
    macFlag[5] = 0;
  }
  // 取得故障类型
  if (Tb < TMIN || Tb > TMAX)
  {
    if (Tb < TMIN)
      macFlag[6] = 1;
    if (Tb > TMAX)
      macFlag[6] = 2;
  }
  else
  {
    macFlag[6] = 0;
  }
  // 取得故障类型
  if (Tc < TMIN || Tc > TMAX)
  {
    if (Tc < TMIN)
      macFlag[7] = 1;
    if (Tc > TMAX)
      macFlag[7] = 2;
  }
  else
  {
    macFlag[7] = 0;
  }
  if (UReadNone)
    macFlag[8] = 1;
  else
    macFlag[8] = 0;

  if (VReadNone)
    macFlag[9] = 1;
  else
    macFlag[9] = 0;
  // 故障报警
  // 温度超上下限
  // ab项没电
  if (((Ta < TMIN || Ta > TMAX) || (Tb < TMIN || Tb > TMAX) || (Tc < TMIN || Tc > TMAX))) //||UReadNone||VReadNone))
  {
    // Ta=156;
    // Tb=157;
    // Tc=158;
    macFlag[1] = 1;  // 故障报警
    macFlag[14] = 1; // 叫
    macFlag[2] = 0;  // 取消超温报警
    macFlag[3] = 0;  // 取消跳闸报警
    macFlag[0] = 0;  // 不转
  }
  if ((Ta >= TMIN && Ta <= TMAX) && (Tb >= TMIN && Tb <= TMAX) && (Tc >= TMIN && Tc <= TMAX)) //&&URead&&VRead)
  {
    macFlag[1] = 0; // 不故障报警
  }

  // 超温报警
  if ((Ta > Tset3 || Tb > Tset3 || Tc > Tset3))
  {
    if (macFlag[1] != 1) // 非故障报警
      macFlag[2] = 1;    // 超温报警
    macFlag[14] = 1;     // 叫
  }
  if (Ta < (Tset3 - cwhuiCha) && Tb < (Tset3 - cwhuiCha) && Tc < (Tset3 - cwhuiCha)) // 停止温度
  {
    macFlag[2] = 0; // 没有超温报警
  }

  // 跳闸
  if ((Ta > Tset4 || Tb > Tset4 || Tc > Tset4))
  {
    if (macFlag[1] != 1) // 非故障报警
      macFlag[3] = 1;    // 跳闸
    macFlag[14] = 1;     // 叫
  }
  if (Ta < (Tset4 - tzhuiCha) && Tb < (Tset4 - tzhuiCha) && Tc < (Tset4 - tzhuiCha)) // 停止温度
  {
    macFlag[3] = 0; // 不跳闸
  }

  // 门禁
  if (DoorError) // 门开 高电平
  {
    macFlag[4] = 1;  // 门开报警
    macFlag[14] = 1; // 叫
  }
  else
  {
    macFlag[4] = 0; // 门关无报警
  }

  if (Ta < Tset3 && Tb < Tset3 && Tc < Tset3 && Ta > TMIN && Tb > TMIN && Tc > TMIN && DoorOK)
  {
    macFlag[14] = 0; // 不叫
  }
  LedFlag = 0;
  if (macFlag[12] == 0)
  {
    LedFlag = LedFlag & 0xFE;
  } // 手动灯 1
  if (macFlag[12] == 1)
  {
    LedFlag = LedFlag | 0x01;
  }
  if (macFlag[12] == 2)
  {
    LedFlag = LedFlag | 0x01;
  }

  switch (JDQSTAT)
  {
  case 0:
    switch (macFlag[0])
    {
    case 0:
      LedFlag = LedFlag & 0xFD;
      if (JDQFLAG == 1)
        JDQSTAT = 1;
      JDQFLAG = 0;
      break;
    case 1:
      LedFlag = LedFlag | 0x02;
      if (JDQFLAG == 0)
        JDQSTAT = 1;
      JDQFLAG = 1;
      break;
    }
    JDQTIME = 0;
    break;
  case 1: // 决定动作 暂停采样
    if (JDQTIME > 100)
      JDQSTAT = 2;
    break;
  case 2: // 动继电器
    if (macFlag[0] == 1)
    {
      if (TestModeFlag == 0)
      {
        JDQ1_H;
      }
    }
    if (macFlag[0] != 1)
    {
      if (TestModeFlag == 0)
      {
        JDQ1_L;
      }
    }
    JDQSTAT = 3;
    break;
  case 3: // 开启采样
    if (JDQTIME > 2000)
    {
      JDQSTAT = 0;
      JDQTIME = 0;
    }
    break;
  }

  if (macFlag[1] == 1)
  {
    if (TestModeFlag == 0)
      JDQ2_H;
    else
      JDQ2_L;
    LedFlag = LedFlag | 0x10;
  } // 故障 5
  if (macFlag[1] != 1)
  {
    if (TestModeFlag == 0)
      JDQ2_L;
    LedFlag = LedFlag & 0xEF;
  }
  if (macFlag[2] == 1)
  {
    if (TestModeFlag == 0)
      JDQ3_H;
    else
      JDQ3_L;
    LedFlag = LedFlag | 0x04;
  } // 超温 3
  if (macFlag[2] != 1)
  {
    if (TestModeFlag == 0)
      JDQ3_L;
    LedFlag = LedFlag & 0xFB;
  }
  if (macFlag[3] == 1)
  {
    if (TestModeFlag == 0)
      JDQ4_H;
    else
      JDQ4_L;
    LedFlag = LedFlag | 0x08;
  } // 跳闸 4
  if (macFlag[3] != 1)
  {
    if (TestModeFlag == 0)
      JDQ4_L;
    LedFlag = LedFlag & 0xF7;
  }
  if (macFlag[4] == 1)
  {
    if (TestModeFlag == 0)
      JDQ5_H;
    else
      JDQ5_L;
  } // 门禁
  if (macFlag[4] != 1)
  {
    if (TestModeFlag == 0)
      JDQ5_L;
  }
  if (macFlag[5] == 1)
  {
    if (TestModeFlag == 0)
      JDQ6_H;
    else
      JDQ6_L;
  }
  if (macFlag[5] != 1)
  {
    if (TestModeFlag == 0)
      JDQ6_L;
  }
  if (macFlag[14] == 1 && macFlag[15] == 0)
    willBeepTime = 500;
  if (macFlag[14] == 0 || macFlag[15] == 1)
    willBeepTime = 0;
  if (macFlag[16] == 1)
  {
    LedFlag = LedFlag | 0x20;
  } // 最大
  if (macFlag[16] != 1)
  {
    LedFlag = LedFlag & 0xDF;
  } // 巡回

  tmNum[5] = LedFlag;

  if (readKeyTime > 10)
  {
    readKeyTime = 0;

    if (Key1Read)
    {
      Cnt1++;
      Cnt2 = 0;
      Cnt3 = 0;
      Cnt4 = 0;
      lCnt1++;
      lCnt2 = 0;
      lCnt3 = 0;
      lCnt4 = 0;
    }
    if (Key2Read)
    {
      Cnt1 = 0;
      Cnt2++;
      Cnt3 = 0;
      Cnt4 = 0;
      lCnt1 = 0;
      lCnt2++;
      lCnt3 = 0;
      lCnt4 = 0;
    }
    if (Key3Read)
    {
      Cnt1 = 0;
      Cnt2 = 0;
      Cnt3++;
      Cnt4 = 0;
      lCnt1 = 0;
      lCnt2 = 0;
      lCnt3++;
      lCnt4 = 0;
    }
    if (Key4Read)
    {
      Cnt1 = 0;
      Cnt2 = 0;
      Cnt3 = 0;
      Cnt4++;
      lCnt1 = 0;
      lCnt2 = 0;
      lCnt3 = 0;
      lCnt4++;
    }

    // 短按弹起
    if (Cnt1 >= 5 && Key1ReadNone)
    {
      KeyV = 1;
      Cnt1 = 0;
    }
    if (Cnt2 >= 5 && Key2ReadNone)
    {
      KeyV = 2;
      Cnt2 = 0;
    }
    if (Cnt3 >= 5 && Key3ReadNone)
    {
      KeyV = 3;
      Cnt3 = 0;
    }
    if (Cnt4 >= 5 && Key4ReadNone)
    {
      KeyV = 4;
      Cnt4 = 0;
    }
    // 长按
    if (Key1Read && lCnt1 >= 100)
      KeyV = 1;
    if (Key2Read && lCnt2 >= 100)
      KeyV = 2;
    if (Key3Read && lCnt3 >= 100)
      KeyV = 3;
    if (Key4Read && lCnt4 >= 100)
      KeyV = 4;
    // 长按计数清零
    if (Key1ReadNone)
      lCnt1 = 0;
    if (Key2ReadNone)
      lCnt2 = 0;
    if (Key3ReadNone)
      lCnt3 = 0;
    if (Key4ReadNone)
      lCnt4 = 0;

    // Ta=30;
    // Tb=31;
    // Tc=30;

    switch (tmNum[6])
    {

    // 主页**    00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 0: // 初始显示 A温度
      TestModeFlag = 0;
      tmNum[0] = 0xA;
      xunhuiCnt++;
      if (Ta < TMIN)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Ta > TMAX)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(mmTa, 1, 0);
      }
      // shValue(Ta,1,0);
      if (KeyV == 1 && lCnt1 >= 40)
      {
        tmNum[6] = 3;
        KeyV = 0;
        lCnt1 = 0;
        break;
      }
      if (KeyV == 4 && lCnt4 >= 40)
      {
        KeyV = 0;
        if (macFlag[16] == 0)
          macFlag[16] = 1;
        else
          macFlag[16] = 0;
        break;
      } // 巡回
      if (KeyV == 4 && Key4ReadNone)
      {
        tmNum[6] = 0;
        KeyV = 0;
        if (macFlag[15] == 0)
          macFlag[15] = 1;
        else
          macFlag[15] = 0;
        break;
      } // 消音
      if (KeyV == 2)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 1;
        else
          macFlag[12] = 0;
        break;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 2;
        else
          macFlag[12] = 0;
        break;
      }
      if (macFlag[16] == 0) // 巡回
      {
        if (xunhuiCnt >= XHTIME)
        {
          xunhuiCnt = 0;
          tmNum[6] = 1;
          break;
        }
      }
      else
      {
        if (Tb > Ta)
        {
          xunhuiCnt = 0;
          tmNum[6] = 1;
          break;
        }
      }
      KeyV = 0;
      break;
    case 1: // B温度
      tmNum[0] = 0xB;
      xunhuiCnt++;
      if (Tb < TMIN)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Tb > TMAX)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(mmTb, 1, 0);
      }
      // shValue(Tb,1,0);
      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = 3;
        KeyV = 0;
        lCnt1 = 0;
        break;
      }
      if (KeyV == 4 && lCnt4 >= 40)
      {
        KeyV = 0;
        if (macFlag[16] == 0)
          macFlag[16] = 1;
        else
          macFlag[16] = 0;
        break;
      } // 巡回
      if (KeyV == 4 && Key4ReadNone)
      {
        tmNum[6] = 0;
        KeyV = 0;
        if (macFlag[15] == 0)
          macFlag[15] = 1;
        else
          macFlag[15] = 0;
        break;
      } // 消音
      if (KeyV == 2)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 1;
        else
          macFlag[12] = 0;
        break;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 2;
        else
          macFlag[12] = 0;
        break;
      }
      if (macFlag[16] == 0) // 巡回
      {
        if (xunhuiCnt >= XHTIME)
        {
          xunhuiCnt = 0;
          tmNum[6] = 2;
          break;
        }
      }
      else
      {
        if (Tc > Tb)
        {
          xunhuiCnt = 0;
          tmNum[6] = 2;
          break;
        }
      }
      break;
    case 2: // C温度
      tmNum[0] = 0xC;
      xunhuiCnt++;
      if (Tc < TMIN)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Tc > TMAX)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(mmTc, 1, 0);
      }

      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = 3;
        KeyV = 0;
        lCnt1 = 0;
        break;
      }
      if (KeyV == 4 && lCnt4 >= 40)
      {
        KeyV = 0;
        if (macFlag[16] == 0)
          macFlag[16] = 1;
        else
          macFlag[16] = 0;
        break;
      } // 巡回
      if (KeyV == 4 && Key4ReadNone)
      {
        tmNum[6] = 0;
        KeyV = 0;
        if (macFlag[15] == 0)
          macFlag[15] = 1;
        else
          macFlag[15] = 0;
        break;
      } // 消音
      if (KeyV == 2)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 1;
        else
          macFlag[12] = 0;
        break;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
        if (macFlag[12] == 0)
          macFlag[12] = 2;
        else
          macFlag[12] = 0;
        break;
      }
      if (macFlag[16] == 0) // 巡回
      {
        if (xunhuiCnt >= XHTIME)
        {
          xunhuiCnt = 0;
          tmNum[6] = 0;
          break;
        }
      }
      else
      {
        if (Ta > Tc)
        {
          xunhuiCnt = 0;
          tmNum[6] = 0;
          break;
        }
      }
      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单**PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P20 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 3: // P XX    在这里跳转编辑功能
      TestModeFlag = 0;
      tmNum[0] = 28; // 显示P

      shValue(Pcnt, 0, 2);

      if (LoopCnt < 40)
      {
        KeyV = 0;
        LoopCnt++;
      }
      if (KeyV == 1 && LoopCnt >= 40)
      {
        LoopCnt = 0;
        if (Pcnt == 1)
          tmNum[6] = 4;
        if (Pcnt == 2)
          tmNum[6] = 7;
        if (Pcnt == 3)
          tmNum[6] = 40; // 10
        if (Pcnt == 4)
          tmNum[6] = 16;
        if (Pcnt == 5)
          tmNum[6] = 24;
        if (Pcnt == 6)
          tmNum[6] = 26;
        if (Pcnt == 8)
          tmNum[6] = 30;
        if (Pcnt == 9)
          tmNum[6] = 36;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        if (Pcnt < 9)
          Pcnt++;
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        if (Pcnt > 1)
          Pcnt--;
        KeyV = 0;
      }

      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史**P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 4:           // P 01下面的断电前温度查看
      tmNum[0] = 0xA; // 显示A
      shValue(pTa, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 5;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }

      break;
    case 5:           // P 01下面的断电前温度查看
      tmNum[0] = 0xb; // 显示b
      shValue(pTb, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 6;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }

      break;
    case 6:           // P 01下面的断电前温度查看
      tmNum[0] = 0xC; // 显示C
      shValue(pTc, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }

      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时**P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 7:          // P 02 风机定时
      tmNum[0] = 28; // 显示P
      tmNum[1] = 29; //-
      tmNum[2] = 0xF;
      tmNum[3] = 0xd;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = 8;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }

      break;
    case 8:          // P 02 风机定时小时
      tmNum[0] = 31; // 显示H
      shValue(FenH, 0, 2);
      if (KeyV == 1)
      {
        tmNum[6] = 9;
        KeyV = 0;
        writeAT24C16(addrFenH, (unsigned char *)&FenH, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        if (FenH < 24)
          FenH++;
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        if (FenH > 0)
          FenH--;
        KeyV = 0;
      }

      break;

    case 9:          // P 02 风机定时分钟
      tmNum[0] = 30; // 显示h
      shValue(FenM, 0, 2);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        writeAT24C16(addrFenM, (unsigned char *)&FenM, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        if (FenM < 60)
          FenM++;
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        if (FenM > 0)
          FenM--;
        KeyV = 0;
      }

      break;

      // 主页      00.A20.0  01.B20.0   02.C20.0
      // 菜单  PXX 03.P0X
      // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
      // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
      // 计算**P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
      // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
      // 测试  P05 24.h-CH-  25.h-30.0
      // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
      // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
      // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0

    case 10: // P 04 温度公式计算
      TestModeFlag = 1;
      MnTest = 0;
      tmNum[0] = 0xA;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 11;
        KeyV = 0;
        xa1 = AdcValue[2];
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 11: // P 04 温度公式计算
      tmNum[0] = 0xB;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 12;
        KeyV = 0;
        xb1 = AdcValue[0];
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 12: // P 04 温度公式计算
      tmNum[0] = 0xC;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 13;
        KeyV = 0;
        xc1 = AdcValue[1];
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;
    case 13: // P 04 温度公式计算
      tmNum[0] = 0xA;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 14;
        KeyV = 0;
        xa2 = AdcValue[2];
        k1 = 2000.0 / (xa2 - xa1);
        b1 = k1 * xa1;
        writeAT24C16(addrk1, (unsigned char *)&k1, 4);
        writeAT24C16(addrb1, (unsigned char *)&b1, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;
    case 14: // P 04 温度公式计算
      tmNum[0] = 0xB;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 15;
        KeyV = 0;
        xb2 = AdcValue[0];
        k2 = 2000.0 / (xb2 - xb1);
        b2 = k2 * xb1;
        writeAT24C16(addrk2, (unsigned char *)&k2, 4);
        writeAT24C16(addrb2, (unsigned char *)&b2, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 15: // P 04 温度公式计算
      tmNum[0] = 0xC;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        xc2 = AdcValue[1];
        k3 = 2000.0 / (xc2 - xc1);
        b3 = k3 * xc1;
        writeAT24C16(addrk3, (unsigned char *)&k3, 4);
        writeAT24C16(addrb3, (unsigned char *)&b3, 4);
        TestModeFlag = 0;
        MnTest = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设**P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 16:         // P 04 温度设定
      tmNum[0] = 30; // 显示h-A1-
      tmNum[1] = 29; //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x1;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = 17;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 17: // 风机启动温度
      tmNum[0] = 30;
      shValue(Tset1, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 18;
        KeyV = 0;
        writeAT24C16(addrTset1, (unsigned char *)&Tset1, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset1 < TsetValHig)
          Tset1++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset1 > TsetValLow)
          Tset1--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset1 < TsetValHig)
          Tset1++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset1 > TsetValLow)
          Tset1--;
      }

      break;
    case 18:         //
      tmNum[0] = 30; // 显示h-A2-
      tmNum[1] = 29; //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x2;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = 19;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 19: // 风机停止温度
      tmNum[0] = 30;
      shValue(Tset2, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 20;
        KeyV = 0;
        writeAT24C16(addrTset2, (unsigned char *)&Tset2, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset2 < TsetValHig)
          Tset2++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset2 > TsetValLow)
          Tset2--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset2 < TsetValHig)
          Tset2++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset2 > TsetValLow)
          Tset2--;
      }

      break;
    case 20:         //
      tmNum[0] = 30; // 显示h-A3-
      tmNum[1] = 29; //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x3;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = 21;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 21: // 超温
      tmNum[0] = 30;
      shValue(Tset3, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 22;
        KeyV = 0;
        writeAT24C16(addrTset3, (unsigned char *)&Tset3, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset3 < TsetValHig)
          Tset3++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset3 > TsetValLow)
          Tset3--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset3 < TsetValHig)
          Tset3++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset3 > TsetValLow)
          Tset3--;
      }

      break;
    case 22:         //
      tmNum[0] = 30; // 显示h-A4-
      tmNum[1] = 29; //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x4;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = 23;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 23: // 风机停止温度
      tmNum[0] = 30;
      shValue(Tset4, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        writeAT24C16(addrTset4, (unsigned char *)&Tset4, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset4 < TsetValHig)
          Tset4++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset4 > TsetValLow)
          Tset4--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset4 < TsetValHig)
          Tset4++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset4 > TsetValLow)
          Tset4--;
      }

      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
    // 测试**P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 24:          // P 05 模拟测试
      tmNum[0] = 30;  // 显示h-CH-
      tmNum[1] = 29;  //-
      tmNum[2] = 0xC; // C
      tmNum[3] = 31;  // H
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = 25;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 25: // 模拟测试
      TestModeFlag = 1;
      tmNum[0] = 30;
      shValue(MnTest, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        TestModeFlag = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
        TestModeFlag = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (MnTest < MNValHig)
          MnTest++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (MnTest > MNValLow)
          MnTest--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (MnTest < MNValHig)
          MnTest++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (MnTest > MNValLow)
          MnTest--;
      }
      if (KeyV == 2 && lCnt2 >= 80)
      {
        if (MnTest < MNValHig - 10)
          MnTest = MnTest + 10;
      }
      if (KeyV == 3 && lCnt3 >= 80)
      {
        if (MnTest > MNValLow + 10)
          MnTest = MnTest - 10;
      }

      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口**P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 26:          // P 06 串口
      tmNum[0] = 30;  // 显示h-dd-
      tmNum[1] = 29;  //-
      tmNum[2] = 0xd; // d
      tmNum[3] = 0xd; // d
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = 27;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 27: // 地址
      tmNum[0] = 30;
      shValue(addr, 0, 3);
      if (KeyV == 1)
      {
        tmNum[6] = 28;
        KeyV = 0;
        writeAT24C16(addraddr, (unsigned char *)&addr, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (addr < AddrHig)
          addr++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (addr > AddrLow)
          addr--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (addr < AddrHig)
          addr++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (addr > AddrLow)
          addr--;
      }
      if (KeyV == 2 && lCnt2 >= 80)
      {
        if (addr < AddrHig - 10)
          addr = addr + 10;
      }
      if (KeyV == 3 && lCnt3 >= 80)
      {
        if (addr > AddrLow + 10)
          addr = addr - 10;
      }

      break;

    case 28: // 奇偶校验
      tmNum[0] = 30;
      shValue(jojy, 0, 2);
      if (KeyV == 1)
      {
        tmNum[6] = 29;
        KeyV = 0;
        writeAT24C16(addrjojy, (unsigned char *)&jojy, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (jojy < 2)
          jojy++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (jojy > 0)
          jojy--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (jojy < 2)
          jojy++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (jojy > 0)
          jojy--;
      }

      break;

    case 29: // 波特率
      tmNum[0] = 30;
      shValue(btlv, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        writeAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (btlv < 4)
          btlv++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (btlv > 0)
          btlv--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (btlv < 4)
          btlv++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (btlv > 0)
          btlv--;
      }

      break;
    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差**P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 30:          // P 08 回差
      tmNum[0] = 30;  // 显示h-FH- 除湿
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 31;  // H
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = 31;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 31: //
      tmNum[0] = 30;
      shValue(cshuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 32;
        KeyV = 0;
        readAT24C16(addrcshuiCha, (unsigned char *)&cshuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (cshuiCha < HCHig)
          cshuiCha++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (cshuiCha > HCLow)
          cshuiCha--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (cshuiCha < HCHig)
          cshuiCha++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (cshuiCha > HCLow)
          cshuiCha--;
      }

      break;

    case 32:          // P 08 回差
      tmNum[0] = 30;  // 显示h-FH- 除湿
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 0x3; // 3
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = 33;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 33: //
      tmNum[0] = 30;
      shValue(cwhuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 34;
        KeyV = 0;
        readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (cwhuiCha < HCHig)
          cwhuiCha++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (cwhuiCha > HCLow)
          cwhuiCha--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (cwhuiCha < HCHig)
          cwhuiCha++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (cwhuiCha > HCLow)
          cwhuiCha--;
      }

      break;

    case 34:          // P 08 回差
      tmNum[0] = 30;  // 显示h-F4- 跳匝
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 0x4; // 4
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = 35;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 35: //
      tmNum[0] = 30;
      shValue(tzhuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (tzhuiCha < HCHig)
          tzhuiCha++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (tzhuiCha > HCLow)
          tzhuiCha--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (tzhuiCha < HCHig)
          tzhuiCha++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (tzhuiCha > HCLow)
          tzhuiCha--;
      }

      break;

    // 主页      00.A20.0  01.B20.0   02.C20.0
    // 菜单  PXX 03.P0X
    // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
    // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
    // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
    // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0
    // 测试  P05 24.h-CH-  25.h-30.0
    // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
    // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
    // 补偿**P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case 36:         // P 03 温度补偿
      tmNum[0] = 28; // 显示P
      tmNum[1] = 29; //-
      tmNum[2] = 0xC;
      tmNum[3] = 0xd;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = 37;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        KeyV = 0;
      }
      if (KeyV == 3)
      {
        KeyV = 0;
      }
      break;

    case 37: // 温度补偿a
      tmNum[0] = 0xA;
      shValue(Tcala1, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = 38;
        KeyV = 0;
        writeAT24C16(addrTcala1, (unsigned char *)&Tcala1, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tcala1 < BcValHig)
          Tcala1++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tcala1 > BcValLow)
          Tcala1--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tcala1 < BcValHig)
          Tcala1++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tcala1 > BcValLow)
          Tcala1--;
      }
      break;

    case 38: // 温度补偿b
      tmNum[0] = 0xb;
      shValue(Tcala2, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = 39;
        KeyV = 0;
        writeAT24C16(addrTcala2, (unsigned char *)&Tcala2, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tcala2 < BcValHig)
          Tcala2++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tcala2 > BcValLow)
          Tcala2--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tcala2 < BcValHig)
          Tcala2++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tcala2 > BcValLow)
          Tcala2--;
      }
      break;

    case 39: // 温度补偿C
      tmNum[0] = 0xC;
      shValue(Tcala3, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = 3;
        KeyV = 0;
        writeAT24C16(addrTcala3, (unsigned char *)&Tcala3, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tcala3 < BcValHig)
          Tcala3++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tcala3 > BcValLow)
          Tcala3--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tcala3 < BcValHig)
          Tcala3++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tcala3 > BcValLow)
          Tcala3--;
      }

      break;
    case 40: // 密码
      TestModeFlag = 0;
      tmNum[0] = 29; // 显示-
      shValue(Pcnt, 0, 2);
      if (LoopCnt < 40)
      {
        KeyV = 0;
        LoopCnt++;
      }
      if (KeyV == 1 && LoopCnt >= 40) // 确定按钮
      {
        LoopCnt = 0;
        if (Pcnt == 18)
          tmNum[6] = 10; // 密码18 跳转到10
        else
          tmNum[6] = 3; // 返回
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = 0;
        KeyV = 0;
      } // 退出按钮
      if (KeyV == 2)
      {
        if (Pcnt < 19)
          Pcnt++;
        KeyV = 0;
      } // 上键
      if (KeyV == 3)
      {
        if (Pcnt > 1)
          Pcnt--;
        KeyV = 0;
      } // 下键

      break;

    default:
      break;
    }
    SendData();
  }
}
