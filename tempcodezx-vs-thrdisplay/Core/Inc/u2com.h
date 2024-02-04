#ifndef __U2COM_H
#define __U2COM_H
#define MAXRXU2 500
#include "main.h"
#include "control.h"
#include "tm1640.h"
#include "param.h"
#include "control.h"
#define ADCPause HAL_ADC_Stop_DMA(&hadc)
#define ADCContinue HAL_ADC_Start_DMA(&hadc, (uint32_t *)&AdDataOrig, ADDTNUM)
#pragma pack(1)

typedef struct
{
    signed int Ta; //
    signed int Tb;
    signed int Tc;
    signed int pTa; //
    signed int pTb;
    signed int pTc;

    signed int FenH;   //
    signed int FenM;   //
    signed int Tcala1; //
    signed int Tcala2;
    signed int Tcala3;
    signed int Tset1;    //
    signed int Tset2;    //
    signed int Tset3;    //
    signed int Tset4;    //
    signed int cshuiCha; //
    signed int cwhuiCha; //
    signed int tzhuiCha; //
    signed int MnTest;   //
    signed int addr;     //
    signed int jojy;
    signed int btlv;

    signed int k1;
    signed int k2;
    signed int k3;
    signed int b1;
    signed int b2;
    signed int b3;
    unsigned char macFlag[17];
} RUDATA;
extern unsigned char RxBufU2[MAXRXU2];
extern unsigned short RxLenU2;
extern unsigned char TxBufU2[200];
extern unsigned short rsRxIndex;
extern RUDATA sRundata;
extern void u2RxIt(UART_HandleTypeDef *huart);
#endif