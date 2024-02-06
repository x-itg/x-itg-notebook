#ifndef __DAC_7512_H__
#define __DAC_7512_H__
 
#include "stm32f0xx.h"

#define  SYNC_HIGH        (GPIOC->BSRR=1<<13)
#define  SYNC_LOW         (GPIOC->BRR=1<<13)
#define  DAC_SCK_HIGH     (GPIOA->BSRR=1<<8)
#define  DAC_SCK_LOW      (GPIOA->BRR=1<<8)
#define  DAC_DATA_HIGH    (GPIOA->BSRR=1<<11)
#define  DAC_DATA_LOW     (GPIOA->BRR=1<<11)

void DAC_7512_Write( unsigned short data );
extern  void delay_us(volatile uint16_t us);

#endif