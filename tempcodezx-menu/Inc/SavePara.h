#ifndef __SAVEPARA_H
#define __SAVEPARA_H	

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"
 
#define SAVEPAGEADDR    0x800F800
#define SAVEPAGEADDRCOE 0x800FC00//保存系数的那一页
extern unsigned short SaveCalaCoefficient[16][6];
extern unsigned short MenuCoefficient[6];

typedef struct 
{
	unsigned char Awarm;//0
        unsigned char Bwarm;//1
        unsigned char Cwarm;//2
        unsigned char DcWarm;//3直流报警开关
        unsigned char DacOut;//4是否输出直流补偿
        unsigned char DacVal;//5补偿电压的值
        unsigned short DcHig;//6直流过压值
        unsigned short DcLow;//7直流欠压值
        unsigned short DcRig;//8直流触发
        unsigned short  AcHig;//9直流过压值
        unsigned char  AcLow;//10直流欠压值
        unsigned char Bdrate;//11波特率
        unsigned char Addr;//12地址
        

}SAVEPARA;
#define VcoeMax 4000
#define VcoeMin 2000
#define VcoeZ   3000
#define IcoeMax 4000
#define IcoeMin 2000
#define IcoeZ   3700
#define WarnOnOffMax  1
#define WarnOnOffMin  0
#define WarnOnOffZ    1
#define DacValMax   50  
#define DacValMin   0
#define DacValZ     50
#define DcHigMax    589 
#define DcHigMin    565
#define DcHigZ      569
#define DcLowMax    520   
#define DcLowMin    470
#define DcLowZ      495
#define DcRigMax    510     
#define DcRigMin    440
#define DcRigZ      475
#define AcHigMax    280
#define AcHigMin    230
#define AcHigZ      265
#define AcLowMax    190
#define AcLowMin     90
#define AcLowZ      175
#define BdrateMax     1
#define BdrateMin     1
#define BdrateZ       1
#define AddrMax      15
#define AddrMin	      0
#define AddrZ         1

typedef struct 
{
  unsigned short DcV;
  unsigned char  SysSta;
}MAINPARA;
extern MAINPARA MainPara;
extern SAVEPARA SavePara;
extern void FLASH_PageErase(uint32_t PageAddress);
extern unsigned char FlashUpdate(void);
extern unsigned char FlashUpdateCoe(void);
extern unsigned char FlashUpdateBrate(void);
extern void LoadSavePara(void);
#endif