

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

signed int Ta = 161; // 三个温度
signed int Tb = 162;
signed int Tc = 163;
signed int Td = 164;
signed int Te = 165;
signed int Th = 700;

signed int pTa = 0; // 断电前三个温度
signed int pTb = 0;
signed int pTc = 0;
signed int pTd = 0;
signed int pTe = 0;

unsigned int FenH = 0; // 风机定时  每隔多少小时
unsigned int FenM = 0; // 风机定时  持续转几分钟

signed int Tset1 = 1000; // 温度阈值
signed int Tset2 = 800;
signed int Tset3 = 1300;
signed int Tset4 = 1500;

signed int Tjdqk[6] = {900, 900, 900, 900, 900};
signed int Tjdqb[6] = {1000, 1000, 1000, 1000, 1000};
signed int cshuiCha = 50; // 除湿回差 三个回差值
signed int cwhuiCha = 50; // 超温报警回差
signed int tzhuiCha = 50; // 跳闸回差
signed int MnTest = 0;    // 测试 温度值

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

unsigned short AdDataOrig[SUMC][SUMCHAN];
unsigned short AdcValue[SUMCHAN];
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
signed int Tcala1 = 0; // 温度补偿
signed int Tcala2 = 0;
signed int Tcala3 = 0;
signed int Tcala4 = 0;
signed int Tcala5 = 0;

float k[5] = {11.0, 11.0, 11.0, 11.0, 11.0};
float b[5] = {2705.0, 2705.0, 2705.0, 2705.0, 2705.0};
float kk[5] = {0.4, 0.4, 0.4, 0.4, 0.4};
float bb[5] = {223.0, 223.0, 223.0, 223.0, 223.0};
float x1[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
float x2[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

unsigned char dehD = 1;
unsigned char dehE = 1;
unsigned char dehH = 1;
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
#define TMIN (-50)
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

#define addrk4 84
#define addrb4 88
#define addrk5 92
#define addrb5 96
#define addrTcala4 100
#define addrTcala5 104

#define addrjdqt0k 108 // 2
#define addrjdqt1k 110 // 2
#define addrjdqt2k 112 // 2
#define addrjdqt3k 114 // 2
#define addrjdqt4k 116 // 2
#define addrjdqt5k 118 // 2
#define addrjdqt0b 120 // 2
#define addrjdqt1b 122 // 2
#define addrjdqt2b 124 // 2
#define addrjdqt3b 126 // 2
#define addrjdqt4b 128 // 2
#define addrjdqt5b 130 // 2

#define addrkk1 132
#define addrkk2 136
#define addrkk3 140
#define addrkk4 144
#define addrkk5 148
#define addrbb1 152
#define addrbb2 156
#define addrbb3 160
#define addrbb4 164
#define addrbb5 170

#define addrdehD 175
#define addrdehE 176
#define addrdehH 177

#define addrTjdqk0 178
#define addrTjdqk1 180
#define addrTjdqk2 182
#define addrTjdqk3 184
#define addrTjdqk4 186
#define addrTjdqk5 188
#define addrTjdqb0 190
#define addrTjdqb1 192
#define addrTjdqb2 194
#define addrTjdqb3 196
#define addrTjdqb4 198
#define addrTjdqb5 200
void LoadPara(void)
{
  readAT24C16(addrTjdqk0, (unsigned char *)&Tjdqk[0], 2);
  if (Tjdqk[0] > 1500)
    Tjdqk[0] = 900;
  readAT24C16(addrTjdqk1, (unsigned char *)&Tjdqk[1], 2);
  if (Tjdqk[1] > 1500)
    Tjdqk[1] = 900;
  readAT24C16(addrTjdqk2, (unsigned char *)&Tjdqk[2], 2);
  if (Tjdqk[2] > 1500)
    Tjdqk[2] = 900;
  readAT24C16(addrTjdqk3, (unsigned char *)&Tjdqk[3], 2);
  if (Tjdqk[3] > 1500)
    Tjdqk[3] = 900;
  readAT24C16(addrTjdqk4, (unsigned char *)&Tjdqk[4], 2);
  if (Tjdqk[4] > 1500)
    Tjdqk[4] = 900;
  readAT24C16(addrTjdqk5, (unsigned char *)&Tjdqk[5], 2);
  if (Tjdqk[5] > 1500)
    Tjdqk[5] = 700;

  readAT24C16(addrTjdqb0, (unsigned char *)&Tjdqb[0], 2);
  if (Tjdqb[0] > 1500)
    Tjdqb[0] = 1000;
  readAT24C16(addrTjdqb1, (unsigned char *)&Tjdqb[1], 2);
  if (Tjdqb[1] > 1500)
    Tjdqb[1] = 1000;
  readAT24C16(addrTjdqb2, (unsigned char *)&Tjdqb[2], 2);
  if (Tjdqb[2] > 1500)
    Tjdqb[2] = 1000;
  readAT24C16(addrTjdqb3, (unsigned char *)&Tjdqb[3], 2);
  if (Tjdqb[3] > 1500)
    Tjdqb[3] = 1000;
  readAT24C16(addrTjdqb4, (unsigned char *)&Tjdqb[4], 2);
  if (Tjdqb[4] > 1500)
    Tjdqb[4] = 1000;
  readAT24C16(addrTjdqb5, (unsigned char *)&Tjdqb[5], 2);
  if (Tjdqb[5] > 1500)
    Tjdqb[5] = 950;

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
  readAT24C16(addrTcala4, (unsigned char *)&Tcala4, 4);
  if (Tcala4 < -1000 || Tcala4 > 1000)
    Tcala4 = 0;
  readAT24C16(addrTcala5, (unsigned char *)&Tcala5, 4);
  if (Tcala5 < -1000 || Tcala5 > 1000)
    Tcala5 = 0;

  readAT24C16(addrk1, (unsigned char *)&k[0], 4);
  readAT24C16(addrb1, (unsigned char *)&b[0], 4);
  readAT24C16(addrk2, (unsigned char *)&k[1], 4);
  readAT24C16(addrb2, (unsigned char *)&b[1], 4);
  readAT24C16(addrk3, (unsigned char *)&k[2], 4);
  readAT24C16(addrb3, (unsigned char *)&b[2], 4);
  readAT24C16(addrk4, (unsigned char *)&k[3], 4);
  readAT24C16(addrb4, (unsigned char *)&b[3], 4);
  readAT24C16(addrk5, (unsigned char *)&k[4], 4);
  readAT24C16(addrb5, (unsigned char *)&b[4], 4);

  readAT24C16(addrkk1, (unsigned char *)&kk[0], 4);
  readAT24C16(addrbb1, (unsigned char *)&bb[0], 4);
  readAT24C16(addrkk2, (unsigned char *)&kk[1], 4);
  readAT24C16(addrbb2, (unsigned char *)&bb[1], 4);
  readAT24C16(addrkk3, (unsigned char *)&kk[2], 4);
  readAT24C16(addrbb3, (unsigned char *)&bb[2], 4);
  readAT24C16(addrkk4, (unsigned char *)&kk[3], 4);
  readAT24C16(addrbb4, (unsigned char *)&bb[3], 4);
  readAT24C16(addrkk5, (unsigned char *)&kk[4], 4);
  readAT24C16(addrbb5, (unsigned char *)&bb[4], 4);

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
    cshuiCha = 50;
  readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
  if (cwhuiCha < HCLow || cwhuiCha > HCHig)
    cwhuiCha = 50;
  readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
  if (tzhuiCha < HCLow || tzhuiCha > HCHig)
    tzhuiCha = 50;

  readAT24C16(addraddr, (unsigned char *)&addr, 4);
  if (addr > 254)
    addr = 1;
  readAT24C16(addrjojy, (unsigned char *)&jojy, 4);
  if (jojy > 2)
    jojy = 0;
  readAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
  if (btlv > 4)
    btlv = 3;

  readAT24C16(addrdehD, (unsigned char *)&dehD, 1);
  if (dehD > 1)
    dehD = 1;
  readAT24C16(addrdehE, (unsigned char *)&dehE, 1);
  if (dehE > 1)
    dehE = 1;
  readAT24C16(addrdehH, (unsigned char *)&dehH, 1);
  if (dehH > 1)
    dehH = 1;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{

  unsigned int i, j;
  unsigned int tmp[5] = {0, 0, 0, 0, 0};
  static unsigned int cnt = 0;
  static unsigned int max[5] = {0, 0, 0, 0, 0};
  for (j = 0; j < SUMCHAN; j++)
  {
    tmp[j] = 0;
    for (i = 0; i < SUMC; i++)
    {
      tmp[j] = tmp[j] + AdDataOrig[i][j];
    }
    AdcValue[j] = tmp[j] / SUMC;

    tmp[j] = 0;
  }

  cnt++;

  if (cnt > 3000)
  {
    cnt = 0;

    if (TestModeFlag == 0)
    {
#if 1
      if (JDQSTAT == 0)
        Ta = (signed int)((k[0] * (float)max[0] - b[0]) + (float)Tcala1); //+(float)Tcala1;//
      if (JDQSTAT == 0)
        Tb = (signed int)((k[1] * (float)max[1] - b[1]) + (float)Tcala2); //+(float)Tcala2;//
      if (JDQSTAT == 0)
        Tc = (signed int)((k[2] * (float)max[2] - b[2]) + (float)Tcala3); //+(float)Tcala3;//
      if (JDQSTAT == 0)
        Td = (signed int)((k[3] * (float)max[3] - b[3]) + (float)Tcala4); //+(float)Tcala3;//
      if (JDQSTAT == 0)
        Te = (signed int)((k[4] * (float)max[4] - b[4]) + (float)Tcala5); //+(float)Tcala3;//
#if TeStFlag == 1
      Ta = -10;
      Tb = -10;
      Tc = -10;
      Td = -10;
      Te = -10;
#endif
#endif
    }
    else
    {
      Ta = MnTest;
      Tb = MnTest;
      Tc = MnTest;
      Td = MnTest;
      Te = MnTest;
    }
    max[0] = 0;
    max[1] = 0;
    max[2] = 0;
    max[3] = 0;
    max[4] = 0;
  }
  else
  {
    if (max[0] < AdcValue[0])
      max[0] = AdcValue[0];
    if (max[1] < AdcValue[1])
      max[1] = AdcValue[1];
    if (max[2] < AdcValue[2])
      max[2] = AdcValue[2];
    if (max[3] < AdcValue[3])
      max[3] = AdcValue[3];
    if (max[4] < AdcValue[4])
      max[4] = AdcValue[4];
  }

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

    if (shStr[0] == 0)
      tmNum[1] = 16; // 不显示任何东西
    if (shStr[1] == 0)
      tmNum[2] = 16; // 不显示任何东西
    if (shStr[2] == 0)
      tmNum[3] = 16; // 不显示任何东西
    if (shStr[3] == 0)
      tmNum[4] = 16; // 不显示任何东西

    if (mode == 0 && Num < 10 && Num > -10)
      tmNum[3] = 18; // 0.
    if (mode == 4 && Num < 10 && Num > -10 && dp == 1)
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
// 17 18 19 20 21 22
unsigned char macFlag[23] = {0}; // 最终执行的标价  macFlag[12] 0自动 1手动开 2手动关

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
  }
  if ((Ta >= TMIN && Ta <= TMAX) && (Tb >= TMIN && Tb <= TMAX) && (Tc >= TMIN && Tc <= TMAX)) //&&URead&&VRead)
  {
    macFlag[1] = 0; // 不故障报警
  }

  // 超温报警
  if ((Ta > Tset3 || Tb > Tset3 || Tc > Tset3))
  {
    macFlag[2] = 1;  // 超温报警
    macFlag[14] = 1; // 叫
  }
  if (Ta < (Tset3 - cwhuiCha) && Tb < (Tset3 - cwhuiCha) && Tc < (Tset3 - cwhuiCha)) // 停止温度
  {
    macFlag[2] = 0; // 没有超温报警
  }

  // 跳闸
  if ((Ta > Tset4 || Tb > Tset4 || Tc > Tset4))
  {
    macFlag[3] = 1;  // 跳闸
    macFlag[14] = 1; // 叫
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

  // 第5路 abc三路pt100
  if ((Ta > Tjdqb[0] || Tb > Tjdqb[0] || Tc > Tjdqb[0]))
  {
    macFlag[17] = 1;
  }

  if (Ta < Tjdqk[0] && Tb < Tjdqk[0] && Tc < Tjdqk[0]) // 停止温度
  {
    macFlag[17] = 0; // 没有超温报警
  }

  // 第6路 abc三路pt100
  if (Ta > Tjdqb[1] || Tb > Tjdqb[1] || Tc > Tjdqb[1])
  {
    macFlag[18] = 1;
  }
  if (Ta < Tjdqk[1] && Tb < Tjdqk[1] && Tc < Tjdqk[1]) // 停止温度
  {
    macFlag[18] = 0; // 没有超温报警
  }

  // 第7路与风机一样
  if (macFlag[0] == 1)
  {
    macFlag[19] = 1;
  }
  if (macFlag[0] != 1) // 停止温度
  {
    macFlag[19] = 0; // 没有超温报警
  }

  // 第8路 d路
  if (Td > Tjdqb[3])
  {
    macFlag[20] = 1;
  }
  if (Td < Tjdqk[3]) // 停止温度
  {
    macFlag[20] = 0; // 没有超温报警
  }

  // 第9路 e路
  if (Te > Tjdqb[4])
  {
    macFlag[21] = 1;
  }

  if (Te < Tjdqk[4]) // 停止温度
  {
    macFlag[21] = 0; // 没有超温报警
  }

  // 第10路 H湿度
  if (Th > Tjdqb[5])
  {
    macFlag[22] = 1;
  }
  if (Th < Tjdqk[5]) // 停止温度
  {
    macFlag[22] = 0; // 没有超温报警
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
        JDQ10_H;
      }
    }
    if (macFlag[0] != 1)
    {
      if (TestModeFlag == 0)
      {
        JDQ10_L;
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
  if (macFlag[17] == 1)
  {
    if (TestModeFlag == 0)
      JDQ5_H;
    else
      JDQ5_L;
  } // 0
  if (macFlag[17] != 1)
  {
    if (TestModeFlag == 0)
      JDQ5_L;
  }
  if (macFlag[18] == 1)
  {
    if (TestModeFlag == 0)
      JDQ6_H;
    else
      JDQ6_L;
  } // 1
  if (macFlag[18] != 1)
  {
    if (TestModeFlag == 0)
      JDQ6_L;
  }
  if (macFlag[19] == 1)
  {
    if (TestModeFlag == 0)
      JDQ7_H;
    else
      JDQ7_L;
  } // 2
  if (macFlag[19] != 1)
  {
    if (TestModeFlag == 0)
      JDQ7_L;
  }
  if (macFlag[20] == 1)
  {
    if (TestModeFlag == 0)
      JDQ8_H;
    else
      JDQ8_L;
  } // 3
  if (macFlag[20] != 1)
  {
    if (TestModeFlag == 0)
      JDQ8_L;
  }
  if (macFlag[21] == 1)
  {
    if (TestModeFlag == 0)
      JDQ9_H;
    else
      JDQ9_L;
  } // 4
  if (macFlag[21] != 1)
  {
    if (TestModeFlag == 0)
      JDQ9_L;
  }
  if (macFlag[22] == 1)
  {
    if (TestModeFlag == 0)
      JDQ10_H;
    else
      JDQ10_L;
  } // 5
  if (macFlag[22] != 1)
  {
    if (TestModeFlag == 0)
      JDQ10_L;
  }
  // if(macFlag[4]==1){if(TestModeFlag==0)JDQ5_H;else JDQ5_L;}//门禁
  // if(macFlag[4]!=1){if(TestModeFlag==0)JDQ5_L;}
  // if(macFlag[5]==1){if(TestModeFlag==0)JDQ6_H;else JDQ6_L;}
  // if(macFlag[5]!=1){if(TestModeFlag==0)JDQ6_L;}
  if (macFlag[14] == 1 && macFlag[15] == 0)
    willBeepTime = 100;
  if (macFlag[14] == 0 || macFlag[15] == 1)
    willBeepTime = 0;
  if (macFlag[16] == 1)
  {
    LedFlag = LedFlag | 0x20;
  } // 最大 0010 0000
  if (macFlag[16] != 1)
  {
    LedFlag = LedFlag & 0xDF;
  }                         // 巡回
  LedFlag = LedFlag | 0x20; //  fe dcba
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
    case indexMain_A: // 初始显示 A温度
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
        shValue(Ta, 1, 0);
      }
      // shValue(Ta,1,0);
      if (KeyV == 1 && lCnt1 >= 40)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_B;
          break;
        }
      }
      else
      {
        if (Tb > Ta)
        {
          xunhuiCnt = 0;
          tmNum[6] = indexMain_B;
          break;
        }
      }
      KeyV = 0;
      break;
    case indexMain_B: // B温度
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
        shValue(Tb, 1, 0);
      }
      // shValue(Tb,1,0);
      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_C;
          break;
        }
      }
      else
      {
        if (Tc > Tb)
        {
          xunhuiCnt = 0;
          tmNum[6] = indexMain_C;
          break;
        }
      }
      break;
    case indexMain_C: // C温度
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
        shValue(Tc, 1, 0);
      }

      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_D;
          break;
        }
      }
      else
      {
        if (Ta > Tc)
        {
          xunhuiCnt = 0;
          tmNum[6] = indexMain_D;
          break;
        }
      }
      break;

    case indexMain_D: // D温度
      tmNum[0] = 0xD;
      xunhuiCnt++;
      if (Td < TMIN)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Td > TMAX)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(Td, 1, 0);
      }
      if (dehD == 0)
      {
        xunhuiCnt = 0;
        tmNum[6] = indexMain_E;
      }

      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_E;
          break;
        }
      }
      else
      {
        if (Ta > Td)
        {
          xunhuiCnt = 0;
          tmNum[6] = indexMain_A;
          break;
        }
      }
      break;

    case indexMain_E: // E温度
      tmNum[0] = 0xE;
      xunhuiCnt++;
      if (Te < TMIN)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Te > TMAX)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(Te, 1, 0);
      }

      if (dehE == 0)
      {
        xunhuiCnt = 0;
        tmNum[6] = indexMain_H;
      }

      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_H;
          break;
        }
      }
      else
      {
        if (Ta > Te)
        {
          xunhuiCnt = 0;
          tmNum[6] = indexMain_A;
          break;
        }
      }
      break;

    case indexMain_H: // H湿度
      tmNum[0] = 30;  // h
      xunhuiCnt++;
      if (Th < -1000)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 32;
        tmNum[4] = 29; //-
      }
      else if (Th > 2000)
      {
        tmNum[1] = 29; //-
        tmNum[2] = 0x0;
        tmNum[3] = 31;
        tmNum[4] = 29; //-
      }
      else
      {
        shValue(Th, 1, 0);
      }

      if (dehH == 0)
      {
        xunhuiCnt = 0;
        tmNum[6] = indexMain_A;
      }

      if (KeyV == 1 && lCnt1 >= 60)
      {
        tmNum[6] = indexMain_S;
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
        tmNum[6] = indexMain_A;
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
          tmNum[6] = indexMain_A;
          break;
        }
      }
      break;
      // 主页      00.A20.0  01.B20.0   02.C20.0
      // 菜单**PXX 03.P0X
      // 历史  P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
      // 风时  P02 07.P-Fd-  08.HXXXX   09.hxxxx
      // 计算  P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
      // 温设  P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0 启动 关闭 报警 跳闸
      // 测试  P05 24.h-CH-  25.h-30.0
      // 串口  P06 26.h-dd-  27.h001    28.h01     29.h1
      // 回差  P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
      // 补偿  P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
    case indexMain_S: // P XX    在这里跳转编辑功能
      TestModeFlag = 0;
      tmNum[0] = 28; // 显示P
      shValue(Pcnt, 0, 4);
      if (LoopCnt < 40)
      {
        KeyV = 0;
        LoopCnt++;
      }
      if (KeyV == 1 && LoopCnt >= 40)
      {
        LoopCnt = 0;
        if (Pcnt == 1)
          tmNum[6] = indexHist_A;
        if (Pcnt == 2)
          tmNum[6] = indexFen_1;
        if (Pcnt == 3)
          tmNum[6] = indexCala0_A;
        if (Pcnt == 4)
          tmNum[6] = indexTSet_A10;
        if (Pcnt == 5)
          tmNum[6] = indexTest1;
        if (Pcnt == 6)
          tmNum[6] = indexCom1;
        if (Pcnt == 8)
          tmNum[6] = indexFH_1;
        if (Pcnt == 9)
          tmNum[6] = indexBC_1;
        if (Pcnt == 10)
          tmNum[6] = indexDEH_D;
        if (Pcnt == 11)
          tmNum[6] = indexa5;
        if (Pcnt == 12)
          tmNum[6] = indexa8;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      }
      if (KeyV == 2)
      {
        if (Pcnt < 12)
          Pcnt++;
        KeyV = 0;
        if (Pcnt == 7)
          Pcnt = 8;
      }
      if (KeyV == 3)
      {
        if (Pcnt > 1)
          Pcnt--;
        KeyV = 0;
        if (Pcnt == 7)
          Pcnt = 6;
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
    case indexHist_A: // P 01下面的断电前温度查看
      tmNum[0] = 0xA; // 显示A
      shValue(pTa, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexHist_B;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexHist_B: // P 01下面的断电前温度查看
      tmNum[0] = 0xb; // 显示b
      shValue(pTb, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexHist_C;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexHist_C: // P 01下面的断电前温度查看
      tmNum[0] = 0xC; // 显示C
      shValue(pTc, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_D;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexHist_D: // P 01下面的断电前温度查看
      tmNum[0] = 0xD; // 显示D
      shValue(pTd, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_E;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexHist_E: // P 01下面的断电前温度查看
      tmNum[0] = 0xE; // 显示D
      shValue(pTe, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexFen_1: // P 02 风机定时
      tmNum[0] = 28; // 显示P
      tmNum[1] = 29; //-
      tmNum[2] = 0xF;
      tmNum[3] = 0xd;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = indexFen_2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexFen_2: // P 02 风机定时小时
      tmNum[0] = 31; // 显示H
      shValue(FenH, 0, 2);
      if (KeyV == 1)
      {
        tmNum[6] = indexFen_3;
        KeyV = 0;
        writeAT24C16(addrFenH, (unsigned char *)&FenH, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexFen_3: // P 02 风机定时分钟
      tmNum[0] = 30; // 显示h
      shValue(FenM, 0, 2);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrFenM, (unsigned char *)&FenM, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala0_A: // P 04 温度公式计算
      TestModeFlag = 1;
      MnTest = 0;
      tmNum[0] = 0xA;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala0_B;
        KeyV = 0;
        x1[0] = AdcValue[0];
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala0_B: // P 04 温度公式计算
      tmNum[0] = 0xB;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala0_C;
        KeyV = 0;
        x1[1] = AdcValue[1];
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala0_C: // P 04 温度公式计算
      tmNum[0] = 0xC;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala0_D;
        KeyV = 0;
        x1[2] = AdcValue[2];
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala0_D: // P 04 温度公式计算
      tmNum[0] = 0xD;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala0_E;
        KeyV = 0;
        x1[3] = AdcValue[3];
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala0_E: // P 04 温度公式计算
      tmNum[0] = 0xE;
      tmNum[1] = 0x0;
      tmNum[2] = 0x0;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala1_A;
        KeyV = 0;
        x1[4] = AdcValue[4];
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    //-------------------------------------------------------------------------------------
    case indexCala1_A: // P 04 温度公式计算
      tmNum[0] = 0xA;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala1_B;
        KeyV = 0;
        x2[0] = AdcValue[0];
        k[0] = 2000.0 / (x2[0] - x1[0]);
        b[0] = k[0] * x1[0];
        writeAT24C16(addrk1, (unsigned char *)&k[0], 4);
        writeAT24C16(addrb1, (unsigned char *)&b[0], 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexCala1_B: // P 04 温度公式计算
      tmNum[0] = 0xB;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala1_C;
        KeyV = 0;
        x2[1] = AdcValue[1];
        k[1] = 2000.0 / (x2[1] - x1[1]);
        b[1] = k[1] * x1[1];
        writeAT24C16(addrk2, (unsigned char *)&k[1], 4);
        writeAT24C16(addrb2, (unsigned char *)&b[1], 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala1_C: // P 04 温度公式计算
      tmNum[0] = 0xC;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala1_D;
        KeyV = 0;
        x2[2] = AdcValue[2];
        k[2] = 2000.0 / (x2[2] - x1[2]);
        b[2] = k[2] * x1[2];
        writeAT24C16(addrk3, (unsigned char *)&k[2], 4);
        writeAT24C16(addrb3, (unsigned char *)&b[2], 4);
        TestModeFlag = 0;
        MnTest = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala1_D: // P 04 温度公式计算
      tmNum[0] = 0xD;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexCala1_E;
        KeyV = 0;
        x2[3] = AdcValue[3];
        k[3] = 2000.0 / (x2[3] - x1[3]);
        b[3] = k[3] * x1[3];
        writeAT24C16(addrk4, (unsigned char *)&k[3], 4);
        writeAT24C16(addrb4, (unsigned char *)&b[3], 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCala1_E: // P 04 温度公式计算
      tmNum[0] = 0xE;
      tmNum[1] = 0x0;
      tmNum[2] = 0x2;
      tmNum[3] = 0x0;
      tmNum[4] = 0x0;
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        x2[4] = AdcValue[4];
        k[4] = 2000.0 / (x2[4] - x1[4]);
        b[4] = k[4] * x1[4];
        writeAT24C16(addrk5, (unsigned char *)&k[4], 4);
        writeAT24C16(addrb5, (unsigned char *)&b[4], 4);
        TestModeFlag = 0;
        MnTest = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexTSet_A10: // P 04 温度设定
      tmNum[0] = 30;    // 显示h-A1-
      tmNum[1] = 29;    //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x1;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A11;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexTSet_A11: // 风机启动温度
      tmNum[0] = 30;
      shValue(Tset1, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A20;
        KeyV = 0;
        writeAT24C16(addrTset1, (unsigned char *)&Tset1, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexTSet_A20: //
      tmNum[0] = 30;    // 显示h-A2-
      tmNum[1] = 29;    //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x2;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A21;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexTSet_A21: // 风机停止温度
      tmNum[0] = 30;
      shValue(Tset2, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A30;
        KeyV = 0;
        writeAT24C16(addrTset2, (unsigned char *)&Tset2, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexTSet_A30: //
      tmNum[0] = 30;    // 显示h-A3-
      tmNum[1] = 29;    //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x3;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A31;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexTSet_A31: // 超温
      tmNum[0] = 30;
      shValue(Tset3, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A40;
        KeyV = 0;
        writeAT24C16(addrTset3, (unsigned char *)&Tset3, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexTSet_A40: //
      tmNum[0] = 30;    // 显示h-A3-
      tmNum[1] = 29;    //-
      tmNum[2] = 0xA;
      tmNum[3] = 0x3;
      tmNum[4] = 29; //-
      if (KeyV == 1)
      {
        tmNum[6] = indexTSet_A41;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexTSet_A41: // 风机停止温度
      tmNum[0] = 30;
      shValue(Tset4, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrTset4, (unsigned char *)&Tset4, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexTest1:  // P 05 模拟测试
      tmNum[0] = 30;  // 显示h-CH-
      tmNum[1] = 29;  //-
      tmNum[2] = 0xC; // C
      tmNum[3] = 31;  // H
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = indexTest2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexTest2: // 模拟测试
      TestModeFlag = 1;
      tmNum[0] = 30;
      shValue(MnTest, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        TestModeFlag = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexCom1:   // P 06 串口
      tmNum[0] = 30;  // 显示h-dd-
      tmNum[1] = 29;  //-
      tmNum[2] = 0xd; // d
      tmNum[3] = 0xd; // d
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = indexCom2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCom2: // 地址
      tmNum[0] = 30;
      shValue(addr, 0, 3);
      if (KeyV == 1)
      {
        tmNum[6] = indexCom3;
        KeyV = 0;
        writeAT24C16(addraddr, (unsigned char *)&addr, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCom3: // 奇偶校验
      tmNum[0] = 30;
      shValue(jojy, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = indexCom4;
        KeyV = 0;
        writeAT24C16(addrjojy, (unsigned char *)&jojy, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexCom4: // 波特率
      tmNum[0] = 30;
      shValue(btlv, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexFH_1:   // P 08 回差
      tmNum[0] = 30;  // 显示h-FH- 除湿
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 31;  // H
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = indexFH_2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexFH_2: //
      tmNum[0] = 30;
      shValue(cshuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexF3_1;
        KeyV = 0;
        readAT24C16(addrcshuiCha, (unsigned char *)&cshuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexF3_1:   // P 08 回差
      tmNum[0] = 30;  // 显示h-FH- 除湿
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 0x3; // 3
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = indexF3_2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexF3_2: //
      tmNum[0] = 30;
      shValue(cwhuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexF4_1;
        KeyV = 0;
        readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexF4_1:   // P 08 回差
      tmNum[0] = 30;  // 显示h-F4- 跳匝
      tmNum[1] = 29;  //-
      tmNum[2] = 0xf; // F
      tmNum[3] = 0x3; // 3
      tmNum[4] = 29;  //-
      if (KeyV == 1)
      {
        tmNum[6] = indexF4_2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexF4_2: //
      tmNum[0] = 30;
      shValue(tzhuiCha, 1, 0);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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
    case indexBC_1:  // P 03 温度补偿
      tmNum[0] = 28; // 显示P
      tmNum[1] = 29; //-
      tmNum[2] = 0xC;
      tmNum[3] = 0xd;
      tmNum[4] = 29; //-

      if (KeyV == 1)
      {
        tmNum[6] = indexBC_2;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexBC_2: // 温度补偿a
      tmNum[0] = 0xA;
      shValue(Tcala1, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexBC_3;
        KeyV = 0;
        writeAT24C16(addrTcala1, (unsigned char *)&Tcala1, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexBC_3: // 温度补偿b
      tmNum[0] = 0xb;
      shValue(Tcala2, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexBC_4;
        KeyV = 0;
        writeAT24C16(addrTcala2, (unsigned char *)&Tcala2, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexBC_4: // 温度补偿C
      tmNum[0] = 0xC;
      shValue(Tcala3, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexBC_5;
        KeyV = 0;
        writeAT24C16(addrTcala3, (unsigned char *)&Tcala3, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexBC_5: // 温度补偿d
      tmNum[0] = 0xd;
      shValue(Tcala4, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexBC_6;
        KeyV = 0;
        writeAT24C16(addrTcala4, (unsigned char *)&Tcala4, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tcala4 < BcValHig)
          Tcala4++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tcala4 > BcValLow)
          Tcala4--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tcala4 < BcValHig)
          Tcala4++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tcala4 > BcValLow)
          Tcala4--;
      }
      break;

    case indexBC_6: // 温度补偿e
      tmNum[0] = 0xe;
      shValue(Tcala5, 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrTcala5, (unsigned char *)&Tcala5, 4);
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tcala5 < BcValHig)
          Tcala5++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tcala5 > BcValLow)
          Tcala5--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tcala5 < BcValHig)
          Tcala5++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tcala5 > BcValLow)
          Tcala5--;
      }
      break;

      // 主页         00.A20.0  01.B20.0   02.C20.0
      // 菜单     PXX 03.P0X
      // 历史     P01 04.AXXXX  05.BXXXX   06.CXXXX历史最高温度查看
      // 风时     P02 07.P-Fd-  08.HXXXX   09.hxxxx
      // 计算     P03 10.A000   11.A200    12.B000    13.B200   14.C000    15.C200
      // 温设     P04 16.h-A1-  17.h80.0   18.h-A2-   19.100.0  20.h-A3-   21.130.0   22.h-A4-  23.150.0
      // 测试     P05 24.h-CH-  25.h-30.0
      // 串口     P06 26.h-dd-  27.h001    28.h01     29.h1
      // 回差     P08 30.h-FH-  31.h0.4    32.h-F3-   33.h0.4   34.h-F4-   35.h0.4  除湿 报警 跳闸
      // 补偿     P09 36.P-Cd-  37.A0.0    38.B0.0    39.C0.0
      // DEH选择  P10 D 0/1  E 0/1   H 0/1
    case indexDEH_D: // 温度D是否参数循环显示
      tmNum[0] = 0xd;
      shValue(dehD, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = indexDEH_E;
        KeyV = 0;
        writeAT24C16(addrdehD, (unsigned char *)&dehD, 1);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        dehD = 1;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        dehD = 0;
        KeyV = 0;
      }

      break;
    case indexDEH_E: // 温度E是否参与循环显示
      tmNum[0] = 0xE;
      shValue(dehE, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = indexDEH_H;
        KeyV = 0;
        writeAT24C16(addrdehE, (unsigned char *)&dehE, 1);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        dehE = 1;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        dehE = 0;
        KeyV = 0;
      }

      break;
    case indexDEH_H: // H是否参数循环显示
      tmNum[0] = 31;
      shValue(dehH, 0, 1);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrdehH, (unsigned char *)&dehH, 1);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        dehH = 1;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        dehH = 0;
        KeyV = 0;
      }

      break;
    case indexa5:      // A5__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x05;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0A;

      if (KeyV == 1)
      {
        tmNum[6] = indexa590;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexa590: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqk[0], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexb5;
        KeyV = 0;
        writeAT24C16(addrTjdqk0, (unsigned char *)&Tjdqk[0], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqk[0] < 1500)
          Tjdqk[0]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqk[0] > 200)
          Tjdqk[0]--;
        KeyV = 0;
      }

      break;

    case indexb5:      ////A5__B
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x05;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0b;

      if (KeyV == 1)
      {
        tmNum[6] = indexb5100;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexb5100: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqb[0], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexa6;
        KeyV = 0;
        writeAT24C16(addrTjdqb0, (unsigned char *)&Tjdqb[0], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqb[0] < 1500)
          Tjdqb[0]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqb[0] > 200)
          Tjdqb[0]--;
        KeyV = 0;
      }

      break;

    case indexa6:      // A6__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x06;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0A;

      if (KeyV == 1)
      {
        tmNum[6] = indexa690;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexa690: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqk[1], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexb6;
        KeyV = 0;
        writeAT24C16(addrTjdqk1, (unsigned char *)&Tjdqk[1], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqk[1] < 1500)
          Tjdqk[1]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqk[1] > 200)
          Tjdqk[1]--;
        KeyV = 0;
      }

      break;

    case indexb6:      ////A6__B
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x06;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0b;

      if (KeyV == 1)
      {
        tmNum[6] = indexb6100;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexb6100:
      tmNum[0] = 0xA;
      shValue(Tjdqb[1], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrTjdqb1, (unsigned char *)&Tjdqb[1], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqb[1] < 1500)
          Tjdqb[1]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqb[1] > 200)
          Tjdqb[1]--;
        KeyV = 0;
      }

      break;

      //---
    case indexa8:      // A8__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x08;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0A;

      if (KeyV == 1)
      {
        tmNum[6] = indexa890;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexa890: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqk[3], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexb8;
        KeyV = 0;
        writeAT24C16(addrTjdqk3, (unsigned char *)&Tjdqk[3], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqk[3] < 1500)
          Tjdqk[3]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqk[3] > 200)
          Tjdqk[3]--;
        KeyV = 0;
      }

      break;

    case indexb8:      // A5__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x08;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0b;

      if (KeyV == 1)
      {
        tmNum[6] = indexb8100;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexb8100: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqb[3], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexa9;
        KeyV = 0;
        writeAT24C16(addrTjdqb3, (unsigned char *)&Tjdqb[3], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqb[3] < 1500)
          Tjdqb[3]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqb[3] > 200)
          Tjdqb[3]--;
        KeyV = 0;
      }

      break;

    case indexa9:      // A9__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x09;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0A;

      if (KeyV == 1)
      {
        tmNum[6] = indexa990;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexa990: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqk[4], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexb9;
        KeyV = 0;
        writeAT24C16(addrTjdqk4, (unsigned char *)&Tjdqk[4], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqk[4] < 1500)
          Tjdqk[4]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqk[4] > 200)
          Tjdqk[4]--;
        KeyV = 0;
      }

      break;

    case indexb9:      // A9__B
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x09;
      tmNum[2] = 29; //-
      tmNum[3] = 29; //-
      tmNum[4] = 0x0B;

      if (KeyV == 1)
      {
        tmNum[6] = indexb9100;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexb9100: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqb[4], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexa10;
        KeyV = 0;
        writeAT24C16(addrTjdqb4, (unsigned char *)&Tjdqb[4], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqb[4] < 1500)
          Tjdqb[4]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqb[4] > 200)
          Tjdqb[4]--;
        KeyV = 0;
      }

      break;

    case indexa10:     // A10__A
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x01;
      tmNum[2] = 0x00; //-
      tmNum[3] = 29;   //-
      tmNum[4] = 0x0A;

      if (KeyV == 1)
      {
        tmNum[6] = indexa1090;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexa1090: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqk[5], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexb10;
        KeyV = 0;
        writeAT24C16(addrTjdqk5, (unsigned char *)&Tjdqk[5], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqk[5] < 1500)
          Tjdqk[5]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqk[5] > 200)
          Tjdqk[5]--;
        KeyV = 0;
      }

      break;

    case indexb10:     // A10__B
      tmNum[0] = 0x0A; // 显示P
      tmNum[1] = 0x01;
      tmNum[2] = 0x00; //-
      tmNum[3] = 29;   //-
      tmNum[4] = 0x0B;

      if (KeyV == 1)
      {
        tmNum[6] = indexb10100;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
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

    case indexb10100: // 温度E是否参与循环显示
      tmNum[0] = 0xA;
      shValue(Tjdqb[5], 1, 4);
      if (KeyV == 1)
      {
        tmNum[6] = indexMain_S;
        KeyV = 0;
        writeAT24C16(addrTjdqb5, (unsigned char *)&Tjdqb[5], 2);
      } // 确认
      if (KeyV == 4)
      {
        tmNum[6] = indexMain_A;
        KeyV = 0;
      } // 退出
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tjdqb[5] < 1500)
          Tjdqb[5]++;
        ;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tjdqb[5] > 200)
          Tjdqb[5]--;
        KeyV = 0;
      }

      break;
    default:
      break;
    }
    SendData();
  }
}
