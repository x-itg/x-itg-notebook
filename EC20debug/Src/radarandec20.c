#include "EC20.h"
#include "oled.h"
#pragma pack(1)

unsigned int CpuID[3];
unsigned int SendID;
unsigned char GpsRecFlag = 0;
GPS_DATA gspdata;
GSM_init GSMinit;
unsigned char CloudtriggerFlag = 0;
unsigned int CloudtriggerTime = 0;
// EC20
unsigned short PollTimeCnt = 0;             // �ں����ж��мӼ�
unsigned char RxBufferDMA[DMARXBUFFERSIZE]; // DMA���յ��Ļ���
unsigned short DMARxLenU2 = 0;              // DMA������ճ���
unsigned char rsRxBuf[LASTSZIE];            // ��������
unsigned int rsRxIndexLen = 0;              // ������Ȼ���DMA�����ж��и���
unsigned int rsRxLen = 0;                   // ���ճ���
unsigned int rsRxTime = 0;                  // ��ʱʱ�����
unsigned char rsPackFlag = 0;               // ���ڽ��յı��
unsigned char rsRecFlag = 0;                // ���ձ�־
unsigned char ProSta = 0;
unsigned char linkflagSta = 0;
unsigned char CurStaSendCnt = 0;
unsigned int SendCntRecClear = 0;
unsigned int SendGPSCnt = 0;
unsigned char GPSStatFlag = 0;
Ec20SendData ec20send, *pec20;
Ec20HEART ec20heart, *pech20;
// radar
unsigned char u3rx[u3mx];
unsigned short RxLenU3;

unsigned char last_Res_6 = 0;
unsigned char radar_ID = 0;
unsigned char frame_num = 0;
unsigned char warning_cfg = 0;
unsigned int msint = 0;
char trigger_order = 0x01;
unsigned char lifetime_count[4];
unsigned char danger_keep = 0;
unsigned char ready_2_warning = 0;
unsigned char waringtimeflag = 0;
unsigned char waringspeedflag = 0;
unsigned int waringtime = 0;
unsigned char warning_ID = 0;
unsigned short ms10 = 0;

unsigned char warmSend[4] = {0, 0, 0, 0};
unsigned char sendbuf[255];
unsigned char warmnobeep = 0;
unsigned char linkflag = 0;
unsigned char sendingflag = 0;
unsigned int simCnt = 0;
const unsigned char *psend;
GPIO_PinState readsxtioflag = GPIO_PIN_RESET; // ����ͷ����״̬
unsigned int POLLTime = NORMARLTIME;

#define InitEC2Index 0         // ��ͷ��ʼ��ʼ��
#define InitTCPIndex 6         // ������TCP��ʼ��ʼ��
#define ATCodeSnd 11           // ����TCP���ݵ�����ATָ�����
#define ATTcpReadIndex 12      // TCP��ȡ��ATָ��������
#define ATCodeLastNum 12       // ���һ����ȡGPS���ݵ�ATָ�����  15
#define ATCodeNum 12           // ��������ATָ������ 16
#define SendButNoRecCnt 50     // ���͵���EC20û����Ӧ
#define SendRecButNotExpCnt 10 // ����EC20����Ӧ���ǽ��յ����ݸ�Ԥ�����ݲ�����
#define SendRecButGpsNotExp 3  // gps���ݲ���Ԥ�ڷ�Χ�ļ���
// �����б�
const unsigned char *SendList[] = {
    "AT\r\n\0",                // 0
    "ATE0\r\n\0",              // 1  ���Թر�
    "AT+CPIN?\r\n\0",          // 2 ��������
    "AT+CSQ\r\n\0",            // 3 �ź�������ѯ
    "AT+CREG?\r\n\0",          // 4 ����ע���ѯ
    "AT+CGATT?\r\n\0",         // 5 ���������ѯ
    "AT+QIACT=1\r\n",          // 6 �����ƶ����� ���� context profile
    "AT+QIACT?\r\n\0",         // 7 IP��ַ��ѯ   QIACT: 1,1,1,"10.23.163.28"  ��һ��1context�ţ�1-16��  0-1���Ӻ� 1
    "AT+QIDEACT=1\r\n\0",      // 8  Deactivate a PDP Context
    "AT+QICLOSE=0,2000\r\n\0", // 9 �ر�connect 0 ���������ȹر�һ��
#if FacIP == 0
    "AT+QIOPEN=1,0,\"TCP\",\"139.198.162.94\",997\r\n\0", // 10 ����context 1��   connect0�� TCP ip���˿�47.103.104.119   47.97.25.70
#else
    "AT+QIOPEN=1,0,\"TCP\",\"47.103.104.119\",999\r\n\0", // 10 ����context 1��   connect0�� TCP ip���˿�47.103.104.119   47.97.25.70
#endif
    "AT+QISENDEX=0,\"313233343536373839\"\r\n\0", // 11 ��������
    "AT+QIRD=0,1500\r\n\0",                       // 12  ����
    "AT+QGPSCFG=\"outport\",\"uartdebug\"\r\n\0", // 13  ����gnss�Ķ˿�----------------------------------------
    "AT+QGPS=1\r\n\0",                            // 14����GNNS����   ʹ�ô��ڷ���
    "AT+QGPSLOC=0\r\n\0",                         // 15��ȡλ����Ϣ����

};

// �����ڴ��б�
const unsigned char *RecExpected[] = {
    "OK",      // 0     OK
    "OK",      // 1     OK
    "READY",   // 2     +CPIN: READY   �������ERROR
    "CSQ",     // 3     +CSQ: 28,0
    "0,1",     // 4     +CREG: 0,1   0,5   1,1  1,5 ��Щ������
    "1",       // 5     +CGATT: 1
    "O",       // 6     OK ���� ERROR �ظ�����ΪERROR
    "OK",      // 7     OK
    "OK",      // 8     OK
    "\r",      // 9     OK
    "\r",      // 10
    "OK",      // 11    ����
    "+QIRD",   // 12    ����
    "\r",      // 13
    "\r",      // 14
    "QGPSLOC", // 15
    "ERROR"};

// �������
void Clear_Buffer(void) // ��ջ���
{
  int i;
  for (i = 0; i < LASTSZIE; i++)
  {
    rsRxBuf[i] = 0; // ����
  }
  rsRxLen = 0;
}
// ȫ����ͬ����1
unsigned char ucharCmp(unsigned char *p1, unsigned char *p2, unsigned char len)
{
  unsigned char i;
  for (i = 0; i < len; i++)
  {
    if ((*p1++) != (*p2++))
    {
      return 0;
    }
  }
  return 1; // ȫ�����
}

// �ⲿ����ȫ�����������оͷ���1 ��Ȼ������
unsigned char findStrCmp(unsigned char *allPstr, unsigned char alllen, unsigned char *partPstr, unsigned char partlen)
{
  unsigned char i = 0;
  for (i = 0; i < alllen - partlen + 1; i++)
  {
    if (ucharCmp(allPstr + i, partPstr, partlen) == 1)
    {
      return 1; // �ҵ���ȫ��ͬ
    }
  }
  return 0;
}

// AT����TCP����ʱ����Ҫ����һ��
void HexStrConvert(unsigned char *Inp, unsigned char len, unsigned char *Outp)
{
  unsigned char hexH = 0;
  unsigned char hexL = 0;
  unsigned char i = 0;

  for (i = 0; i < len; i++)
  {
    hexH = (*Inp) / 16;
    hexL = (*Inp) % 16;
    Inp++;
    if (hexH >= 10)
      hexH = hexH - 10 + 'A';
    else
      hexH = hexH + '0';
    if (hexL >= 10)
      hexL = hexL - 10 + 'A';
    else
      hexL = hexL + '0';
    *Outp++ = hexH;
    *Outp++ = hexL;
  }
}
// ��ʱ ���������Ҫ�ѱ����Ż��ĳ�NONE����
void delay_ms(unsigned int n)
{
  msint = 0;
  while (n > msint)
    ;
}

// ����EC20�ĵ�Դ���������ܿ��ƿ��ص�Դ
void ec20power(unsigned char onoffflag)
{
  rstHig;
  powerHig;

  if (onoffflag) // ��
  {
    powerLow;
    delay_oledms(500);
    rstLow;
  }
  else
  {
    rstHig;
    powerHig;
  }
}
char txb[20];

// ��ȡΨһID
unsigned int GetIDCode(void)
{
  // ��ȡCPUΨһID
#if 0 // stm32f407
  CpuID[0]=*(unsigned int *)(0x1fff7a10);
  CpuID[1]=*(unsigned int *)(0x1fff7a14);
  CpuID[2]=*(unsigned int *)(0x1fff7a18);
#endif
#if 1 // stm32f103
  CpuID[0] = *(unsigned int *)(0x1FFFF7E8);
  CpuID[1] = *(unsigned int *)(0x1FFFF7EC);
  CpuID[2] = *(unsigned int *)(0x1FFFF7F0);
#endif
  SendID = (CpuID[0] >> 1) + (CpuID[1] >> 2) + (CpuID[2] >> 3);
  //  SendID=0x15A0;
  //  SendID=0x15B1;
  //  SendID=0x15B2;
  //  SendID=0x15C0;
  //  SendID=0x15D1;
  //  SendID=0x15D2;
  SendID = 0x0001;
  SendID = 0x0002;
  SendID = 0x0003;
  SendID = 0x0004;
  SendID = 0x0005;
  SendID = 0x0007;

  sprintf(txb, "id:%x\r\n\0", SendID);
  SendTxDebug((unsigned char *)txb);

  return SendID;
}
void DataSendPre(void)
{

  unsigned short i = 0;
  unsigned char cnt = 0;
  static unsigned int indx = 0;
  if (waringtime > 0)
  {
    pec20 = &ec20send;
    ec20send.Len = sizeof(Ec20SendData);
    ec20send.ID = GetIDCode();
    ec20send.Send_Index = indx++;
    ec20send.Radar1_Cnt = lifetime_count[0];
    ec20send.Radar2_Cnt = lifetime_count[1];
    ec20send.Radar3_Cnt = lifetime_count[2];
    ec20send.Radar4_Cnt = lifetime_count[3];
    ec20send.Radar1_Warm = warmSend[0]; // ����
    ec20send.Radar2_Warm = warmSend[1]; // ���������ͷ��������ط�Ҳ��1
    ec20send.Radar3_Warm = warmSend[2];
    ec20send.Radar4_Warm = warmSend[3];
    if (ec20send.Radar1_Warm == 1)
      cnt++;
    if (ec20send.Radar2_Warm == 1)
      cnt++;
    if (ec20send.Radar3_Warm == 1)
      cnt++;
    if (ec20send.Radar4_Warm == 1)
      cnt++;
    ec20send.Alarm_Cnt = cnt;
    ec20send.Alarm_Enable = warmnobeep;
    ec20send.GPS_State = GPSStatFlag;
    if (GpsRecFlag == 0) // δ�����ܵ�gps����
    {
      memcpy((char *)&(pec20->Time[0]), "000000.000", 10);
      memcpy((char *)&(pec20->Latitude[0]), "0000.0000N", 10);
      memcpy((char *)&(pec20->longitude[0]), "00000.0000E", 11);
    }
    else
    {
      GpsRecFlag = 0;
    }
    sprintf((char *)&(pec20->Latitude[0]), "2517.2831N");
    sprintf((char *)&(pec20->longitude[0]), "117.242314E");
    ec20send.End = 0xffff0000;
    i = 0;
    sprintf((char *)&sendbuf[i], "AT+QISENDEX=0,\"\0"); // AT+QISENDEX=0,"
    i = i + strlen("AT+QISENDEX=0,\"\0");
    HexStrConvert((unsigned char *)&ec20send, sizeof(Ec20SendData), &sendbuf[i]);
    i = i + sizeof(Ec20SendData) * 2;
    sprintf((char *)&sendbuf[i], "\"\r\n\0");                                // 3�ֽ�
    HAL_UART_Transmit(&huart2, sendbuf, 2 * sizeof(Ec20SendData) + 18, 800); // 69*2+18=156һ������156�ֽ�
  }
  else
  {
    ec20heart.Len = sizeof(Ec20HEART);
    ec20heart.ID = GetIDCode();
    i = 0;
    sprintf((char *)&sendbuf[i], "AT+QISENDEX=0,\"\0"); // AT+QISENDEX=0,"
    i = i + strlen("AT+QISENDEX=0,\"\0");
    HexStrConvert((unsigned char *)&ec20heart, sizeof(Ec20HEART), &sendbuf[i]);
    i = i + sizeof(Ec20HEART) * 2;
    sprintf((char *)&sendbuf[i], "\"\r\n\0");                             // 3�ֽ�
    HAL_UART_Transmit(&huart2, sendbuf, 2 * sizeof(Ec20HEART) + 18, 800); // 69*2+18=156һ������156�ֽ�
  }
}
unsigned char sendedflag = 0;
unsigned char warmonceflag = 0;
// ���ͽ���sendingflag=0;//���ͺ���
// radar
void ec20ProcessFun(void)
{

  char numstr[3];
  unsigned short i = 0;
  // ����
  if (rsPackFlag == 1 && rsRxTime > 50) // �ϰ� ��ֹ��Щ��λ������������ݲ�����
  {
    rsPackFlag = 0;
    rsRxTime = 0;
    rsRxLen = rsRxIndexLen;
    rsRxBuf[rsRxLen] = 0; // Ϊ�˴�ӡʱ��������ֶ���ĩβ׷��һ����
    rsRxIndexLen = 0;
    rsRecFlag = 1;

    // COM_Get_Callback(GSMinit.status,rsRxBuf,rsRxLen);//״̬���������ݡ��������ݵĳ���
    HAL_UART_Receive_DMA(&huart2, RxBufferDMA, DMARXBUFFERSIZE); // �ٴο���DMA����
  }

  if (ProSta < InitTCPIndex)
  {
    POLLTime = ALARMTIME; // δ����TCPǰ�����Ա����ٶȽ���
  }
  else
  {

    if (warmonceflag == 0) // ��δ����
    {
      POLLTime = NORMARLTIME;
    }
    if (waringspeedflag == 1 && sendedflag == 1) // �й����� ���Ѿ����ͻָ�����
    {
      sendedflag = 0;
      waringspeedflag = 0;
      POLLTime = NORMARLTIME;
    }
  }
  if (PollTimeCnt > POLLTime) // 0.5��ִ��һ��
  {
    PollTimeCnt = 0;
    // Ӧ�����  ʶ�����
    if (SendCntRecClear > SendButNoRecCnt) // ���ͼ���  �յ��κζ��������
    {
      ProSta = InitEC2Index;
      SendCntRecClear = 0;
      GPSStatFlag = 0;
      SendGPSCnt = 0;
      CurStaSendCnt = 0;
      ec20power(1); // ��λһ��
      linkflag = 0;
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "                  ", 16);
      PrintHZ(0, 0, "RST ERROR             ", 0, 1, 0);
#else
      SendTxDebug((unsigned char *)"EC20��Ԥ�ڴ���\0");
#endif
    }
    // ���� �н��� ����Ԥ�ڵļ��� ʶ�����
    if (CurStaSendCnt > SendRecButNotExpCnt && ProSta != ATCodeLastNum)
    {
      ProSta = InitEC2Index;
      CurStaSendCnt = 0;
      SendCntRecClear = 0;
      linkflag = 0;
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "                  ", 16);
      PrintHZ(0, 0, "EXP ERROR             ", 0, 1, 0);
#else
      SendTxDebug((unsigned char *)"EC20��Ԥ�ڴ���\0");
#endif
    }

    // ���һ������ �н��� ����Ԥ�ڵļ�����
    if (SendGPSCnt > SendRecButGpsNotExp && ProSta == ATCodeLastNum)
    {
      ProSta = InitTCPIndex; // ���¿�ʼ
      CurStaSendCnt = 0;
      SendGPSCnt = 0;
      GPSStatFlag = 0; // GPSδ��������
    }

    if (ProSta <= ATCodeNum)
    {
      if (rsRecFlag == 0) // ����
      {

        if (ProSta == ATCodeSnd) // ���͵�������Ϣ
        {

          DataSendPre();
        }
        else
        {
          SendTxBuf((unsigned char *)SendList[ProSta]);
          psend = SendList[ProSta];
        }

        if (ProSta == ATCodeLastNum)
        {
          SendGPSCnt++; // ��ȡGPS����
        }
        SendCntRecClear++;
        sprintf((char *)numstr, "%02d\0", ProSta);
#if DEBUGMODE == 0
        OLED_ShowString(0, 0, (char *)numstr, 2);

#else
        SendTxDebug((unsigned char *)numstr);
        if (ProSta == ATCodeSnd) // ���͵�������Ϣ
        {
          SendTxDebug(sendbuf);
        }
        else
        {
          SendTxDebug((unsigned char *)SendList[ProSta]);
        }

#endif

        if (ProSta == 6)
        {
#if DEBUGMODE == 0
          OLED_ShowString(0, 0, "                  ", 16);
          PrintHZ(0, 0, "��������               ", 0, 1, 0);
#else
          SendTxDebug((unsigned char *)"��������\0");
#endif
        }

        // togLEDstatus;
        CurStaSendCnt++; // �յ�Ԥ�ڻ�����
      }
      else // �������ݵ�ʶ����
      {
        rsRecFlag = 0;
        SendCntRecClear = 0;
        sprintf((char *)numstr, "%02d\0", ProSta);
#if DEBUGMODE == 0
        OLED_ShowString(0, 0, "                   \0", 8);
        OLED_ShowString(0, 2, "                   \0", 8);
        OLED_ShowString(0, 0, (char *)numstr, 2);
        OLED_ShowString(24, 0, (char *)rsRxBuf, 8);
#else
        SendTxDebug((unsigned char *)numstr);
        SendTxDebug((unsigned char *)rsRxBuf);
#endif

        if (ProSta == 2 && strstr((const char *)rsRxBuf, (const char *)RecExpected[16]) != NULL)
        {
#if DEBUGMODE == 0
          OLED_ShowString(0, 0, "                  ", 16);
          PrintHZ(0, 0, "SIM ERROR             ", 0, 1, 0);
#else
          SendTxDebug((unsigned char *)"�绰��������\0");
#endif
          linkflag = 0;
        }

        if (ProSta == 12) // TCP���ݷ���
        {
          linkflag = 1;
          if (waringtimeflag)
          {
            sendedflag = 1; // �����ѷ���
          }
#if DEBUGMODE == 0
          OLED_ShowString(0, 0, "                  ", 16);
          PrintHZ(0, 0, "���ӳɹ�              ", 0, 1, 0);
#else
          SendTxDebug((unsigned char *)"���ӳɹ�\0");
#endif
        }

        if (strstr((const char *)rsRxBuf, (const char *)RecExpected[ProSta]) != NULL ||
            (ProSta == 2 && strstr((const char *)rsRxBuf, (const char *)"1,1") != NULL) ||
            (ProSta == 2 && strstr((const char *)rsRxBuf, (const char *)"1,5") != NULL) ||
            (ProSta == 2 && strstr((const char *)rsRxBuf, (const char *)"0,5") != NULL) ||
            (ProSta == 11 && strstr((const char *)rsRxBuf, (const char *)"recv") != NULL) ||
            (ProSta == 11 && strstr((const char *)rsRxBuf, (const char *)",0") != NULL)) // ����Ԥ��  �ٽ��ܵ����ֽ����ҵ�
        {

          if (ProSta == ATTcpReadIndex) // ��ȡ����TCP����ֵ
          {
            if (rsRxLen > 11)
            {
              SendTxDebug("_���յ��Ļظ�:\0");
              SendTxDebug((unsigned char *)rsRxBuf); // �Խ��յ��ϴ�
            }

            if (findStrCmp((unsigned char *)&rsRxBuf[11], 30, (unsigned char *)"ALARM", 5) != 0)
            {
              WarmHig;
              CloudtriggerFlag = 1;
            }
            if (findStrCmp((unsigned char *)&rsRxBuf[11], 30, (unsigned char *)"NONEA", 5) != 0)
            {
              WarmLow;
            }
#if 0 // ��Э��
            for(i=0;i<rsRxLen;i++)
            {
              if(rsRxBuf[i]<'9'&&rsRxBuf[i]>'0')//���һ������
              {
                break;
              }
            }
            if(rsRxLen>=19)
            {
              
              if(rsRxBuf[i+9]==1)
              {
                warmSend[0]=0;
                warmSend[1]=0;
                warmSend[2]=0;
                warmSend[3]=0;
              }
              if(rsRxBuf[i+10]==0)warmnobeep=0;
              if(rsRxBuf[i+10]==1)warmnobeep=1;
              if(rsRxBuf[i+11]==1)lifetime_count[0]=0;
              if(rsRxBuf[i+12]==1)lifetime_count[1]=0;
              if(rsRxBuf[i+13]==1)lifetime_count[2]=0;
              if(rsRxBuf[i+14]==1)lifetime_count[3]=0;
            }
#endif
          }
#if 0
          if(ProSta==ATCodeLastNum)//gps�Ķ�ȡ  ���� QGPSLOC��ȡ��ȷ
          {
            SendGPSCnt=0;//�յ�gpsԤ������
            GPSStatFlag=1;//gps����״̬����
            pec20=&ec20send;
            for(i=0;i<rsRxLen;i++)
            {
              if(rsRxBuf[i]<'9'&&rsRxBuf[i]>'0')//���һ������
              {
                break;
              }
            }
            memcpy((char *)&(pec20->Time[0]),&rsRxBuf[i],10);
            for(;i<rsRxLen;i++)
            {
              if(rsRxBuf[i]==',')//���һ������
              {
                break;
              }
            }
            memcpy((char *)&(pec20->Latitude[0]),&rsRxBuf[i+1],10);
            i++;//������һ������
            for(;i<rsRxLen;i++)
            {
              if(rsRxBuf[i]==',')//���һ������
              {
                break;
              }
            }
            memcpy((char *)&(pec20->longitude[0]),&rsRxBuf[i+1],11);
            GpsRecFlag=1;
          }
#endif
          GpsRecFlag = 1;   // ������γ�ȵ�ʶ��
          if (ProSta == 11) // ���ͳɹ���
          {
            linkflagSta = 1;
            SendGPSCnt = 0;
          }

          ProSta++;          // ����Ԥ��������
          CurStaSendCnt = 0; // �յ�Ԥ������
        }
        else
        {
          if (ProSta == 11) // ���Ͳ��ɹ�
          {
            linkflagSta = 0;
            ProSta = 6; // ����û���ɹ��ͱ������ ֱ�����¿�ʼ
          }
        }
        Clear_Buffer();
      }
    }
    else // ���������������� �ӵھŲ�[�ر�TCP�����ⲽ����ִ��]��ʼ
    {

      if (linkflagSta)
      {
        ProSta = 11; // ���·���
      }
      else
      {
        ProSta = 6;
      }
      sendingflag = 0; // δ����
    }
  }
}

// ����while(1)�ܵĺ���=EC20+RADAR
void PollFun(void)
{

  static unsigned char initflag = 0;

  if (initflag == 0)
  {
    initflag = 1; // �����һЩ��ʼ������
    ec20power(1);
    u1Conf(); // RS232
    u2Conf(); // EC20  ATָ��
    u3Conf(); // Radar
#if DEBUGMODE == 0
    OLED_Init();
    OLED_CLS();
    OLED_ShowString(0, 1, "Setup Complete", 16);
    delay_ms(1000);
    OLED_CLS();
#else
    SendTxDebug((unsigned char *)"��ʼ�����\0");
#endif
    lifetime_count[0] = 0;
    lifetime_count[1] = 0;
    lifetime_count[2] = 0;
    lifetime_count[3] = 0;
    warmSend[0] = 0;
    warmSend[1] = 0;
    warmSend[2] = 0;
    warmSend[3] = 0;
  }

  ec20ProcessFun();

#if FacSend == 1 // �������
  if (simCnt > 10000)
  {
    ready_2_warning = 1;
    warning_ID = 1;
    warmSend[0] = 1;
    warmSend[1] = 1;
    warmSend[2] = 1;
    warmSend[3] = 1;
    if (linkflag == 1 && sendingflag == 0)
    {
      sendingflag = 1;
      ProSta = 11; // ����׼������
    }
  }

  if (simCnt > 20000)
  {
    simCnt = 0;
    sendingflag = 0;
    ready_2_warning = 0;
    warning_ID = 0;
    warmSend[0] = 0;
    warmSend[1] = 0;
    warmSend[2] = 0;
    warmSend[3] = 0;
  }

#endif

  // �ϴ�1���0
  if (readsxtioflag == GPIO_PIN_SET && readSXTwarm == GPIO_PIN_RESET)
  {
    ready_2_warning = 1;
    warning_ID = 0;
    warmSend[0] = 1;
    warmSend[1] = 1;
  }

  readsxtioflag = readSXTwarm;

  if (ready_2_warning == 1)
  {
    danger_keep = 10;
    ready_2_warning = 0;

    if (linkflag == 1 && sendingflag == 0) // ������ δ����
    {
      sendingflag = 1;
      ProSta = 11; // ����׼������

      PollTimeCnt = POLLTime + 1;
    }
    if (warning_ID == 1)
    {
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "RADAR[1]DANGER!!", 16);
#else
      SendTxDebug((unsigned char *)"�״�1����\0");
#endif
    }
    else if (warning_ID == 2)
    {
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "RADAR[2]DANGER!!", 16);
#else
      SendTxDebug((unsigned char *)"�״�2����\0");
#endif
    }
    else if (warning_ID == 3)
    {
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "RADAR[3]DANGER!!", 16);
#else
      SendTxDebug((unsigned char *)"�״�3����\0");
#endif
    }
    else if (warning_ID == 4)
    {
      ;
#if DEBUGMODE == 0
      OLED_ShowString(0, 0, "RADAR[4]DANGER!!", 16);
#else
      SendTxDebug((unsigned char *)"�״�4����\0");
#endif
    }

    // if(warmnobeep==0)//
    {
      if (waringtimeflag == 0)
      {
        waringtimeflag = 1;
        waringspeedflag = 1;
        warmonceflag = 1;
        POLLTime = ALARMTIME;
        WarmHig; // ����  ����û�б�����
      }
    }
  }
  if (waringtime > 10000)
  {
    WarmLow;
#if DEBUGMODE == 0
    SendTxDebug((unsigned char *)"�ѱ���\0");
#else
    SendTxDebug((unsigned char *)"�ѱ���\0");
#endif
    waringtimeflag = 0;
    sendingflag = 0; // δ���� 20220527
    waringtime = 0;
    warmSend[0] = 0;
    if (readSXTwarm != GPIO_PIN_RESET)
      warmSend[1] = 0;
    warmSend[2] = 0;
    warmSend[3] = 0;
  }

  if (ms10 >= 6000) // 6��
  {
    ms10 = 0;
    for (int i = 0; i < 4; i++)
    {
      lifetime_count[i]++;
      if (lifetime_count[i] > 10) // 1����
      {
        // warmSend[i]=0;
        // printf("radar[%d] offline",i+1);
      }
    }

    danger_keep--;
    if (danger_keep == 0)
    {
#if DEBUGMODE == 0
      OLED_Clear();
#else
      SendTxDebug((unsigned char *)"�����Ļ\0");
#endif
      WarmLow;
    }
  }
}
