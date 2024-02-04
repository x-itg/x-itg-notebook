#include "stm32f0xx_hal.h"
#include "control.h"
#include "ds18b20.h"
#include "TM1640.h"
#include "DHT.h"
#include "24c02.h"
#include "param.h"
#include "tlc5615.h"
// √ P03 取消 A2  A3 校验
// √ P04 增加 湿度 25  26端子   - A5（湿度40%）， 可调节 15  16 端子   -A6（60℃） 判断，可调节
// P09 A，项不变 ，B, C项温度补偿改为下面显示窗的 第一项 第二项补偿，
// K1 K2 K3动作时 通道1的灯指的是 左的温湿度超了 通道2的灯指的是右边的温湿度超了这个意思吗
unsigned int sysms = 0;
unsigned int willBeepTime = 0;
char sst1[6] = {0}; // 上面的数码管显示的字符串
char sst2[6] = {0}; // 下面左边的数码管显示的字符串
char sst3[6] = {0}; // 下面右边的数码管显示的字符串

signed int Ta = 162; // 三个温度
signed int Tb = 167;
signed int Tc = 166;
signed int pTa = 0; // 断电前三个温度
signed int pTb = 0;
signed int pTc = 0;

unsigned int FenH = 0;    // 风机定时  每隔多少小时
unsigned int FenM = 0;    // 风机定时  持续转几分钟
signed int Tcala1 = 0;    // 温度补偿
signed int Tcala2 = 0;    // 温度补偿
signed int Tcala3 = 0;    // 温度补偿
signed int Tcala4 = 0;    // 湿度补偿
signed int Tcala5 = 0;    // 湿度补偿
signed int Tset1 = 1000;  // A1设置项 风机启动温度阈值  JDQ1_H
signed int Tset2 = 800;   // A2设置项 风机关闭温度阈值  JDQ1_L
signed int Tset3 = 1300;  // A3设置项 超温报警温度阈值  JDQ3_H
signed int Tset4 = 1500;  // A4设置项 跳匝报警温度阈值  JDQ4_H
signed int Tset5 = 400;   // A5设置项 除湿湿度启动阈值  qd1_H
signed int Tset6 = 600;   // A6设置项 除湿湿度启动阈值  qd2_H
signed int Tset7 = 600;   // A7设置项 除湿温度启动阈值  qd3_H
signed int cshuiCha = 50; // 除湿回差 三个回差值
signed int cwhuiCha = 50; // 超温报警回差
signed int tzhuiCha = 50; // 跳闸回差
signed int MnTest = 10;   // 测试 温度值

unsigned int addr = 1;
unsigned int jojy = 0; // 0无 1偶 2奇
unsigned int btlv = 0; // 0 1200 1 2400 2 4800 3 9600 4 19200

unsigned short AdDataOrig[ADDTNUM];
unsigned short AdcValue;
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
unsigned short MENUINDEX = 0;       // 菜单目录
unsigned char FengJiSW = 0;         // 风机开关
unsigned char MacFailed = 0;        // 故障
unsigned char OverTemp = 0;         // 超温报警
unsigned char JumpZha = 0;          // 跳闸
unsigned char DoorJin = 0;          // 门禁
unsigned char TempASta = 0;         // A温度状态0正常 1过低 2过高
unsigned char TempBSta = 0;         // B温度状态0正常 1过低 2过高
unsigned char TempCSta = 0;         // C温度状态0正常 1过低 2过高
unsigned char PowrASta = 0;         // 相电A状态 0正常 1没电
unsigned char PowrBSta = 0;         // 相电B状态 0正常 1没电
unsigned char MODAutoManuONOFF = 0; // 风机模式Mode    0自动 1手动开 2手动关
unsigned char disBeep = 0;          // 0不消音   1消音
unsigned char goBeep = 0;           // 0不叫 1叫
unsigned char MODLoopShow = 0;      // 显示模式ShowMod 0巡回 1最大温度显示
unsigned char chuShiLevel = 0;      // 除湿等级
void ReadKey(void)
{
  static unsigned char state = 0;
  UpateShData();

  if (sysms > 800)
  {
    sysms = 0;
    switch (state)
    {
    case 0:
      state = 1;
      THb = DHT_getData(&THbCONFIG);
      THb.hum = THb.hum * 10 + Tcala2;
      THb.temp = THb.temp * 10 + Tcala3;
      break;

    default:
      state = 0;
      THc = DHT_getData(&THcCONFIG);
      THc.hum = THc.hum * 10 + Tcala4;
      THc.temp = THc.temp * 10 + Tcala5;
      break;
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{

  unsigned int i;
  unsigned int tmp;
  static unsigned int cnt = 0;
  static unsigned int max = 0;

  tmp = 0;
  for (i = 0; i < ADDTNUM; i++)
  {
    tmp = tmp + AdDataOrig[i];
  }
  AdcValue = tmp / ADDTNUM;

  tmp = 0;

  cnt++;

  if (cnt > 1000)
  {
    cnt = 0;

    if (TestModeFlag == 0) // 正常模式
    {
      if (JDQSTAT == 0)
      {
        Ta = (signed int)((k1 * (float)max - b1) + (float)Tcala1);
      }
    }
    else // 测试模式 直接显示
    {
      Ta = MnTest;
      Tb = MnTest;
      Tc = MnTest;
    }
    max = 0;
  }
  else
  {
    if (max < AdcValue)
      max = AdcValue;
  }

  return;
}

SmgType LedLeft;  // 左边的四个灯+左下的两个灯
SmgType LedRight; // 右边的四个灯

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
  static unsigned char qThbThc = 0;

  LedLeft.lled.manu = 0; // 手动
  LedLeft.lled.autt = 0; // 自动
  LedLeft.lled.baoj = 0; // 报警
  LedLeft.lled.tiao = 0; // 跳匝
  LedLeft.lled.ch1 = 0;  // 通道1
  LedLeft.lled.ch2 = 0;  // 通道2
  LedLeft.lled.ch3 = 0;
  LedLeft.lled.ch4 = 0;

  LedRight.rled.fault = 0;  // 故障
  LedRight.rled.heatk1 = 0; // 加热K1 两个温湿度传感器任意一个湿度超过40%时K1动作
  LedRight.rled.heatk2 = 0; // 加热K2 两个温湿度传感器任意一个湿度超过60%时K2动作
  LedRight.rled.heatk3 = 0; // 加热K3 两个温湿度传感器任意一个温度超过60度时K3动作
  LedRight.rled.free1 = 0;
  LedRight.rled.free2 = 0;
  LedRight.rled.free3 = 0;
  LedRight.rled.free4 = 0;

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
    FengJiSW = 1; // 转
  }
  if (Ta < Tset2 && Tb < Tset2 && Tc < Tset2) // 停止温度
  {
    FengJiSW = 0; // 不转
  }

  if (MODAutoManuONOFF == 1) // 手动开
  {
    FengJiSW = 1; // 转
  }
  if (MODAutoManuONOFF == 2) // 手动关
  {
    FengJiSW = 0; // 不转
  }
  if (MODAutoManuONOFF > 2) // 自动 手动开 手动关
  {
    MODAutoManuONOFF = 0;
  }
  if (MODAutoManuONOFF == 0 && oneMinuteCnt > FenH * 60 && oneMinuteCnt < (FenH * 60 + FenM))
  {
    if (FenH != 0 && FenM != 0)
    {
      FengJiSW = 1; // 转
    }
  }
  if (oneMinuteCnt > (FenH * 60 + FenM))
  {
    oneMinuteCnt = 0;
  }

  // 取得故障类型
  if (Ta < TMIN || Ta > TMAX)
  {
    if (Ta < TMIN)
      TempASta = 1;
    if (Ta > TMAX)
      TempASta = 2;
  }
  else
  {
    TempASta = 0;
  }
  // 取得故障类型
  if (Tb < TMIN || Tb > TMAX)
  {
    if (Tb < TMIN)
      TempBSta = 1;
    if (Tb > TMAX)
      TempBSta = 2;
  }
  else
  {
    TempBSta = 0;
  }
  // 取得故障类型
  if (Tc < TMIN || Tc > TMAX)
  {
    if (Tc < TMIN)
      TempCSta = 1;
    if (Tc > TMAX)
      TempCSta = 2;
  }
  else
  {
    TempCSta = 0;
  }
  if (UReadNone)
  {
    PowrASta = 1;
  }
  else
  {
    PowrASta = 0;
  }
  if (VReadNone)
  {
    PowrBSta = 1;
  }
  else
  {
    PowrBSta = 0;
  }
  // 故障报警
  // 温度超上下限
  // ab项没电
  if (Ta < TMIN || Ta > TMAX) //||UReadNone||VReadNone))
  {
    MacFailed = 1; // 故障报警
    goBeep = 1;    // 叫
  }
  if (Ta >= TMIN && Ta <= TMAX) //&&URead&&VRead)
  {
    MacFailed = 0; // 不故障报警
  }

  // 超温报警
  if (Ta > Tset3) // 湿度40%时K1动作 湿度60%时K2动作 温度60度时K3动作
  {
    OverTemp = 1; // 超温报警
    goBeep = 1;   // 叫
  }
  if (Ta < (Tset3 - cwhuiCha)) // 停止温度
  {
    OverTemp = 0; // 没有超温报警
  }

  // 跳闸
  if (Ta > Tset4)
  {
    JumpZha = 1; // 跳闸
    goBeep = 1;  // 叫
  }
  if (Ta < (Tset4 - tzhuiCha)) // 停止温度
  {
    JumpZha = 0; // 不跳闸
  }
  chuShiLevel = 0;
  if (THb.hum > Tset5 || THc.hum > Tset5) // 一级除湿
  {
    chuShiLevel = chuShiLevel | 0x01;
  }
  if (THb.hum < Tset5 - 10 && THc.hum < Tset5 - 10) // 一级除湿停止湿度设置一定回差值
  {
    chuShiLevel = chuShiLevel & 0xFE;
  }
  if (THb.hum > Tset6 || THc.hum > Tset6) // 二级除湿
  {
    chuShiLevel = chuShiLevel | 0x02;
  }
  if (THb.hum < Tset6 - 10 && THc.hum < Tset6 - 10) // 二级除湿停止湿度设置一定回差值
  {
    chuShiLevel = chuShiLevel & 0xFD;
  }
  if (THb.temp > Tset7 || THc.temp > Tset7) // 三级除湿
  {
    chuShiLevel = chuShiLevel | 0x04;
  }
  if (THb.temp < Tset7 - 10 && THc.temp < Tset7 - 10) // 三级除湿停止湿度设置一定回差值
  {
    chuShiLevel = chuShiLevel & 0xFB;
  }
  if (chuShiLevel & 0x01) // 一级除湿
  {
    qK1_H;
    LedRight.rled.heatk1 = 1;
  }
  else
  {
    qK1_L;
    LedRight.rled.heatk1 = 0;
  }
  if (chuShiLevel & 0x02) // 二级除湿
  {
    qK2_H;
    LedRight.rled.heatk2 = 1;
  }
  else
  {
    qK2_L;
    LedRight.rled.heatk2 = 0;
  }
  if (chuShiLevel & 0x04) // 三级除湿
  {
    qK3_H;
    LedRight.rled.heatk3 = 1;
  }
  else
  {
    qK3_L;
    LedRight.rled.heatk3 = 0;
  }

  // 门禁
  if (DoorError) // 门开 高电平
  {
    DoorJin = 1; // 门开报警
    goBeep = 1;  // 叫
  }
  else
  {
    DoorJin = 0; // 门关无报警
  }

  if (Ta < Tset3 && Tb < Tset3 && Tc < Tset3 && Ta > TMIN && Tb > TMIN && Tc > TMIN && DoorOK)
  {
    goBeep = 0; // 不叫
  }
  if (MODAutoManuONOFF == 0) // 模式 0 自动 1 手动开 2 手动关
  {
    LedLeft.lled.manu = 0;
    LedLeft.lled.autt = 1;
  }
  if (MODAutoManuONOFF == 1) // 手动开
  {
    LedLeft.lled.manu = 1;
    LedLeft.lled.autt = 0;
  }
  if (MODAutoManuONOFF == 2)
  {
    LedLeft.lled.manu = 1;
    LedLeft.lled.autt = 0;
  }

  if (MODAutoManuONOFF > 2)
  {
    MODAutoManuONOFF = 0;
  }
  switch (JDQSTAT)
  {
  case 0:
    switch (FengJiSW)
    {
    case 0: // 不转
      if (JDQFLAG == 1)
      {
        JDQSTAT = 1;
      }
      JDQFLAG = 0;
      break;
    case 1: // 转
      if (JDQFLAG == 0)
      {
        JDQSTAT = 1;
      }
      JDQFLAG = 1;
      break;
    }
    JDQTIME = 0;
    break;
  case 1: // 决定动作 暂停采样
    if (JDQTIME > 100)
    {
      JDQSTAT = 2;
    }
    break;
  case 2: // 动继电器
    if (FengJiSW == 1)
    {
      JDQ1_H;
    }
    if (FengJiSW != 1)
    {
      JDQ1_L;
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

  // 故障 2
  if (MacFailed == 1) // 故障
  {
    LedRight.rled.fault = 1; // 故障的灯
  }
  if (MacFailed != 1)
  {
    LedRight.rled.fault = 0; // 无故障的灯
  }

  // 超温 3
  if (OverTemp == 1)
  {
    JDQ3_H;
    LedLeft.lled.baoj = 1; // 超温报警
  }
  if (OverTemp != 1)
  {
    JDQ3_L;
    LedLeft.lled.baoj = 0; // 超温报警
  }

  // 4 跳闸
  if (JumpZha == 1)
  {
    JDQ4_H;
    LedLeft.lled.tiao = 1; // 跳闸
  }                        // 跳闸 4
  if (JumpZha != 1)
  {
    JDQ4_L;
    LedLeft.lled.tiao = 0; // 跳闸
  }
  // 5 门禁
  if (DoorJin == 1)
  {

    JDQ5_H;
  }
  if (DoorJin != 1)
  {
    JDQ5_L;
  }

  if (goBeep == 1 && disBeep == 0) // 叫 且 不消音
  {
    BeepOn;
  }
  if (goBeep == 0 || disBeep == 1) // 不叫 或 消音
  {
    BeepOf;
  }

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

    switch (MENUINDEX)
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
      xunhuiCnt++;
      setShowsetDot(sst1, Ta);
      if (xunhuiCnt >= 100)
      {
        xunhuiCnt = 0;

        switch (qThbThc)
        {
        case 0:
          qThbThc = 1;
          break;
        case 1:
          qThbThc = 0;
          break;
        }
      }
      if (qThbThc == 0)
      {
        LedLeft.lled.ch1 = 1;
        LedLeft.lled.ch2 = 0;
        setShowsetDot(sst2, THb.temp);
        setShowsetDot(sst3, THb.hum);
      }
      else
      {
        LedLeft.lled.ch1 = 0;
        LedLeft.lled.ch2 = 1;
        setShowsetDot(sst2, THc.temp);
        setShowsetDot(sst3, THc.hum);
      }

      if (Ta < TMIN || Ta > TMAX)
      {
        if (Ta < TMIN)
          sprintf(sst1, "- L-"); // 右对齐，不足4位前面补空格
        if (Ta > TMAX)
          sprintf(sst1, "- H-"); // 右对齐，不足4位前面补空格
      }

      if (KeyV == 1 && lCnt1 >= 40)
      {
        MENUINDEX = 3;
        KeyV = 0;
        lCnt1 = 0;
        break;
      }

      if (KeyV == 2) // 手动/自动/+
      {
        KeyV = 0;
        if (disBeep == 0)
          MODAutoManuONOFF = 1;
        else
          MODAutoManuONOFF = 0;
        break;
      }
      if (KeyV == 3) // 手动开关
      {
        KeyV = 0;
        if (MODAutoManuONOFF == 0) // 自动
          MODAutoManuONOFF = 2;    // 手动关
        else
          MODAutoManuONOFF = 0; // 自动
        break;
      }
      if (KeyV == 4 && lCnt4 >= 40) // 消音/巡回/最高
      {
        KeyV = 0;
        if (disBeep == 0)
          disBeep = 1;
        else
          disBeep = 0;
        break;
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

      sprintf(sst1, "P %-2d", Pcnt);
      if (LoopCnt < 40)
      {
        KeyV = 0;
        LoopCnt++;
      }
      if (KeyV == 1 && LoopCnt >= 40)
      {
        LoopCnt = 0;
        if (Pcnt == 1)
          MENUINDEX = 4;
        if (Pcnt == 2)
          MENUINDEX = 7;
        if (Pcnt == 3)
          MENUINDEX = 40; // 10
        if (Pcnt == 4)
          MENUINDEX = 16;
        if (Pcnt == 5)
          MENUINDEX = 24;
        if (Pcnt == 6)
          MENUINDEX = 26;
        if (Pcnt == 8)
          MENUINDEX = 30;
        if (Pcnt == 9)
          MENUINDEX = 36;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 40: // 密码

      sprintf(sst1, "P-%02d", Pcnt);
      if (LoopCnt < 40)
      {
        KeyV = 0;
        LoopCnt++;
      }
      if (KeyV == 1 && LoopCnt >= 40) // 确定按钮
      {
        LoopCnt = 0;
        if (Pcnt == 18)
          MENUINDEX = 10; // 密码18 跳转到10
        else
          MENUINDEX = 3; // 返回
        KeyV = 0;
      }
      if (KeyV == 4) // 退出按钮
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      if (KeyV == 2) // 上键
      {
        if (Pcnt < 19)
          Pcnt++;
        KeyV = 0;
      }
      if (KeyV == 3) // 下键
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
    case 4: // P 01下面的断电前温度查看
      setShowsetDot(sst1, Ta);
      sst1[0] = 'A';
      if (KeyV == 1)
      {
        MENUINDEX = 5;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 5: // P 01下面的断电前温度查看
      setShowsetDot(sst1, Tb);
      sst1[0] = 'B';
      if (KeyV == 1)
      {
        MENUINDEX = 6;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 6: // P 01下面的断电前温度查看
      setShowsetDot(sst1, Tc);
      sst1[0] = 'C';
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 7: // P 02 风机定时
      sprintf(sst1, "P-Fd");
      if (KeyV == 1)
      {
        MENUINDEX = 8;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 8: // P 02 风机定时小时
      sprintf(sst1, "H %02d", FenH);
      if (KeyV == 1)
      {
        MENUINDEX = 9;
        KeyV = 0;
        writeAT24C16(addrFenH, (unsigned char *)&FenH, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

    case 9: // P 02 风机定时分钟
      sprintf(sst1, "h %02d", FenM);

      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        writeAT24C16(addrFenM, (unsigned char *)&FenM, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      MnTest = 0;
      sprintf(sst1, "A000");
      if (KeyV == 1)
      {
        MENUINDEX = 13;
        KeyV = 0;
        xa1 = AdcValue;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      sprintf(sst1, "A200");
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        xa2 = AdcValue;
        k1 = 2000.0 / (xa2 - xa1);
        b1 = k1 * xa1;
        writeAT24C16(addrk1, (unsigned char *)&k1, 4);
        writeAT24C16(addrb1, (unsigned char *)&b1, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      KeyV = 0;

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
    case 16: // P 04 温度设定
      sprintf(sst1, "h-A1");
      if (KeyV == 1)
      {
        MENUINDEX = 17;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }

      KeyV = 0;

      break;

    case 17: // 风机启动温度

      sprintf(sst1, "h%2d.%1d", Tset1 / 10, Tset1 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 18;
        KeyV = 0;
        writeAT24C16(addrTset1, (unsigned char *)&Tset1, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 18: //

      sprintf(sst1, "h-A2");
      if (KeyV == 1)
      {
        MENUINDEX = 19;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      sprintf(sst1, "h%2d.%1d", Tset2 / 10, Tset2 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 20;
        KeyV = 0;
        writeAT24C16(addrTset2, (unsigned char *)&Tset2, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 20: //

      sprintf(sst1, "h-A3");
      if (KeyV == 1)
      {
        MENUINDEX = 21;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }

      KeyV = 0;

      break;

    case 21: // 超温

      sprintf(sst1, "h%2d.%1d", Tset3 / 10, Tset3 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 22;
        KeyV = 0;
        writeAT24C16(addrTset3, (unsigned char *)&Tset3, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 22: //
      sprintf(sst1, "H-A4");
      if (KeyV == 1)
      {
        MENUINDEX = 23;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      sprintf(sst1, "h%2d.%1d", Tset4 / 10, Tset4 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 100;
        KeyV = 0;
        writeAT24C16(addrTset4, (unsigned char *)&Tset4, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 100: //
      sprintf(sst1, "H-A5");
      if (KeyV == 1)
      {
        MENUINDEX = 101;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      KeyV = 0;

      break;

    case 101: // 风机停止温度

      sprintf(sst1, "h%2d.%1d", Tset5 / 10, Tset5 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 102;
        KeyV = 0;
        writeAT24C16(addrTset5, (unsigned char *)&Tset5, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset5 < TsetValHig)
          Tset5++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset5 > TsetValLow)
          Tset5--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset5 < TsetValHig)
          Tset5++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset5 > TsetValLow)
          Tset5--;
      }

      break;

    case 102: //
      sprintf(sst1, "H-A6");
      if (KeyV == 1)
      {
        MENUINDEX = 103;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      KeyV = 0;

      break;

    case 103: // 风机停止温度

      sprintf(sst1, "h%2d.%1d", Tset6 / 10, Tset6 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 104;
        KeyV = 0;
        writeAT24C16(addrTset6, (unsigned char *)&Tset6, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset6 < TsetValHig)
          Tset6++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset6 > TsetValLow)
          Tset6--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset6 < TsetValHig)
          Tset6++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset6 > TsetValLow)
          Tset6--;
      }

      break;

    case 104: //
      sprintf(sst1, "H-A7");
      if (KeyV == 1)
      {
        MENUINDEX = 105;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      KeyV = 0;

      break;

    case 105: // 风机停止温度

      sprintf(sst1, "h%2d.%1d", Tset7 / 10, Tset7 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        writeAT24C16(addrTset7, (unsigned char *)&Tset7, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
        KeyV = 0;
      }
      if (KeyV == 2 && Key2ReadNone)
      {
        if (Tset7 < TsetValHig)
          Tset7++;
        KeyV = 0;
      }
      if (KeyV == 3 && Key3ReadNone)
      {
        if (Tset7 > TsetValLow)
          Tset7--;
        KeyV = 0;
      }
      if (KeyV == 2 && lCnt2 >= 40)
      {
        if (Tset7 < TsetValHig)
          Tset7++;
      }
      if (KeyV == 3 && lCnt3 >= 40)
      {
        if (Tset7 > TsetValLow)
          Tset7--;
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
    case 24: // P 05 模拟测试

      sprintf(sst1, "h-CH");
      if (KeyV == 1)
      {
        MENUINDEX = 25;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      if (MnTest >= 0)
        sprintf(sst1, "h-%2d.%1d", MnTest / 10, MnTest % 10);
      else
        sprintf(sst1, "h-%1d.%1d", -MnTest / 10, -MnTest % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        TestModeFlag = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 26: // P 06 串口

      sprintf(sst1, "h-dd");
      if (KeyV == 1)
      {
        MENUINDEX = 27;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      sprintf(sst1, "h-%d", addr);
      if (KeyV == 1)
      {
        MENUINDEX = 28;
        KeyV = 0;
        writeAT24C16(addraddr, (unsigned char *)&addr, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      sprintf(sst1, "h-%2d", jojy);
      if (KeyV == 1)
      {
        MENUINDEX = 29;
        KeyV = 0;
        writeAT24C16(addrjojy, (unsigned char *)&jojy, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      sprintf(sst1, "h-%2d", btlv);
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        writeAT24C16(addrbtlv, (unsigned char *)&btlv, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 30: // P 08 回差
      sprintf(sst1, "h-FH");
      if (KeyV == 1)
      {
        MENUINDEX = 31;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      if (cshuiCha >= 0)
        sprintf(sst1, "%2d.%1d", cshuiCha / 10, cshuiCha % 10);
      else
        sprintf(sst1, "-%1d.%1d", abs(cshuiCha) / 10, abs(cshuiCha) % 10);

      if (KeyV == 1)
      {
        MENUINDEX = 32;
        KeyV = 0;
        readAT24C16(addrcshuiCha, (unsigned char *)&cshuiCha, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

    case 32: // P 08 回差
      sprintf(sst1, "h-F3");
      if (KeyV == 1)
      {
        MENUINDEX = 33;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      if (cwhuiCha >= 0)
        sprintf(sst1, "h%2d.%1d", cwhuiCha / 10, cwhuiCha % 10);
      else
        sprintf(sst1, "h-%1d.%1d", -cwhuiCha / 10, -cwhuiCha % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 34;
        KeyV = 0;
        readAT24C16(addrcwhuiCha, (unsigned char *)&cwhuiCha, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

    case 34: // P 08 回差
      sprintf(sst1, "h-F4");
      if (KeyV == 1)
      {
        MENUINDEX = 35;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      if (tzhuiCha >= 0)
        sprintf(sst1, "h%2d.%1d", tzhuiCha / 10, tzhuiCha % 10);
      else
        sprintf(sst1, "h-%1d.%1d", -tzhuiCha / 10, -tzhuiCha % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        readAT24C16(addrtzhuiCha, (unsigned char *)&tzhuiCha, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
    case 36: // P 03 温度补偿

      sprintf(sst1, "P-Cd");
      if (KeyV == 1)
      {
        MENUINDEX = 37;
        KeyV = 0;
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
      if (Tcala1 >= 0)
        sprintf(sst1, "A%2d.%1d", Tcala1 / 10, Tcala1 % 10);
      else
        sprintf(sst1, "A-%1d.%1d", -Tcala1 / 10, -Tcala1 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 38;
        KeyV = 0;
        writeAT24C16(addrTcala1, (unsigned char *)&Tcala1, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      if (Tcala2 >= 0)
        sprintf(sst1, "B%2d.%1d", Tcala2 / 10, Tcala2 % 10);
      else
        sprintf(sst1, "B-%1d.%1d", -Tcala2 / 10, -Tcala2 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 39;
        KeyV = 0;
        writeAT24C16(addrTcala2, (unsigned char *)&Tcala2, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

      if (Tcala3 >= 0)
        sprintf(sst1, "C%2d.%1d", Tcala3 / 10, Tcala3 % 10);
      else
        sprintf(sst1, "C-%1d.%1d", -Tcala3 / 10, -Tcala3 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 201;
        KeyV = 0;
        writeAT24C16(addrTcala3, (unsigned char *)&Tcala3, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

    case 201: // 温度补偿D

      if (Tcala4 >= 0)
        sprintf(sst1, "d%2d.%1d", Tcala4 / 10, Tcala4 % 10);
      else
        sprintf(sst1, "d-%1d.%1d", -Tcala4 / 10, -Tcala4 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 202;
        KeyV = 0;
        writeAT24C16(addrTcala3, (unsigned char *)&Tcala4, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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
        if (Tcala3 > BcValLow)
          Tcala3--;
      }

      break;

    case 202: // 温度补偿C

      if (Tcala5 >= 0)
        sprintf(sst1, "A%2d.%1d", Tcala5 / 10, Tcala5 % 10);
      else
        sprintf(sst1, "A-%1d.%1d", -Tcala5 / 10, -Tcala5 % 10);
      if (KeyV == 1)
      {
        MENUINDEX = 3;
        KeyV = 0;
        writeAT24C16(addrTcala3, (unsigned char *)&Tcala5, 4);
      }
      if (KeyV == 4)
      {
        MENUINDEX = 0;
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

    default:
      break;
    }
    TaDACOut();
    getdisplaycode(smg1, sst1);
    getdisplaycode(smg2, sst2);
    getdisplaycode(smg3, sst3);
    SendDataLed(LedLeft.byte, LedRight.byte); // led显示
  }
}
