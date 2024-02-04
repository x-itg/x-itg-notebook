#ifndef __PARAM_H
#define __PARAM_H
extern unsigned int addr;
extern unsigned int jojy;   // 0无 1偶 2奇
extern unsigned int btlv;   // 0 1200 1 2400 2 4800 3 9600 4 19200
extern unsigned int FenH;   // 风机定时  每隔多少小时
extern unsigned int FenM;   // 风机定时  持续转几分钟
extern signed int Tcala1;   // 温度补偿
extern signed int Tcala2;   // 温度补偿
extern signed int Tcala3;   // 温度补偿
extern signed int Tcala4;   // 湿度补偿
extern signed int Tcala5;   // 湿度补偿
extern signed int Tset1;    // 风机启动温度阈值  JDQ1_H
extern signed int Tset2;    // 风机关闭温度阈值  JDQ1_L
extern signed int Tset3;    // 超温报警温度阈值  JDQ3_H
extern signed int Tset4;    // 跳匝报警温度阈值  JDQ4_H
extern signed int Tset5;    // 除湿湿度阈值 qd1_H
extern signed int Tset6;    // 除湿湿度阈值 qd2_H
extern signed int Tset7;    // 除湿温度阈值 qd3_H
extern signed int cshuiCha; // 除湿回差 三个回差值
extern signed int cwhuiCha; // 超温报警回差
extern signed int tzhuiCha; // 跳闸回差
extern signed int MnTest;   // 测试 温度值
extern unsigned short AdcValue;
extern unsigned int readKeyTime;
extern unsigned int oneMinuteCnt;
extern unsigned int one1000msCnt;
extern unsigned int KeyV;
extern unsigned char JDQFLAG;
extern unsigned char JDQSTAT;
extern unsigned char JDQCHAN;
extern unsigned int JDQTIME;
extern unsigned char TestModeFlag;
extern unsigned int LogTime;
extern float k1;
extern float k2;
extern float k3;
extern float b1;
extern float b2;
extern float b3;
extern float xa1;
extern float xa2;
extern float xb1;
extern float xb2;
extern float xc1;
extern float xc2;

extern unsigned short MENUINDEX;       // 菜单目录
extern unsigned char FengJiSW;         // 风机开关
extern unsigned char MacFailed;        // 故障
extern unsigned char OverTemp;         // 超温报警
extern unsigned char JumpZha;          // 跳闸
extern unsigned char DoorJin;          // 门禁
extern unsigned char TempASta;         // A温度状态0正常 1过低 2过高
extern unsigned char TempBSta;         // B温度状态0正常 1过低 2过高
extern unsigned char TempCSta;         // C温度状态0正常 1过低 2过高
extern unsigned char PowrASta;         // 相电A状态 0正常 1没电
extern unsigned char PowrBSta;         // 相电B状态 0正常 1没电
extern unsigned char MODAutoManuONOFF; // 风机模式Mode    0自动 1手动开 2手动关
extern unsigned char disBeep;          // 0不消音   1消音
extern unsigned char goBeep;           // 0不叫 1叫
extern unsigned char MODLoopShow;      // 显示模式ShowMod 0巡回 1最大温度显示
#endif