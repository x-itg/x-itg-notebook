signed int Ta;//三个温度
  signed int Tb;
  signed int Tc;
  signed int pTa;//断电前三个温度
  signed int pTb;
  signed int pTc;
  
  signed int FenH;//风机定时  每隔多少小时
  signed int FenM;//风机定时  持续转几分钟
  signed int Tcala1;//温度补偿
  signed int Tcala2;
  signed int Tcala3;
  signed int Tset1;//温度阈值
  signed int Tset2;
  signed int Tset3;
  signed int Tset4;
  signed int cshuiCha;//除湿回差 三个回差值
  signed int cwhuiCha;//超温报警回差
  signed int tzhuiCha;//跳闸回差
  signed int MnTest;//测试 温度值

  signed int addr;//0 无 1 偶  2 奇
  signed int jojy;
  signed int btlv;
  
  signed int k1;
  signed int k2;
  signed int k3;
  signed int b1;
  signed int b2;
  signed int b3;
  unsigned char macFlag[17];