/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			SST25VF016B.h
** Last modified Date: 	2010-10-29
** Last Version: 		1.0
** Descriptions: 		SPI flash �������� 
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		AVRman
** Created date: 		2010-10-29
** Version: 1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 	
** Modified date: 	
** Version:
** Descriptions:  	
**
********************************************************************************************************/

#ifndef __SST25VF016B_H 
#define __SST25VF016B_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#define SPI_FLASH_ID 0x00BF2541

/* Private typedef -----------------------------------------------------------*/
/* �������SST25VF016B��������� */
typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;  
typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;

extern SPI_HandleTypeDef hspi1;
#define Dummy_Byte                      0xFF

/* Private define ------------------------------------------------------------*/
#define MAX_ADDR		0x1FFFFF	/* ����оƬ�ڲ�����ַ */
#define	SEC_MAX     	         511         /* ������������� */
#define SEC_SIZE		0x1000      /* ������С	*/
#define SPI_FLASH_REBUILD           0
#define SPI_FLASH_SECTOR_SIZE    4096    // ����Flash������С
#define SPI_FLASH_START_SECTOR   0    // ����Flash�ļ�ϵͳFatFSƫ����
#define SPI_FLASH_SECTOR_COUNT   511    // ����Flash�ļ�ϵͳFatFSռ����������   

/* ��,���ڶ���SST25VF016B��Ƭѡ���� */

#define SPI_FLASH_CS_LOW()       GPIOB->BRR=1<<0  //GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SPI_FLASH_CS_HIGH()      GPIOB->BSRR=1<<0 //GPIO_SetBits(GPIOB, GPIO_Pin_0)

/* Private function prototypes -----------------------------------------------*/
void  SPI_FLASH_Init(void);
uint8_t SSTF016B_RD(uint32_t Dst, uint8_t* RcvBufPt ,uint32_t NByte);
uint8_t SSTF016B_RdID(idtype IDType,uint32_t* RcvbufPt);
uint8_t SSTF016B_WR(uint32_t Dst, uint8_t* SndbufPt,uint32_t NByte);

uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2);
void SPI_FLASH_Test(void);
void df_write_open(uint32_t addr);
void df_write(uint8_t *buf,uint16_t size);
void df_read(uint8_t *buf,uint16_t size);
void df_read_open(uint32_t addr);
void df_write_close(void);
uint32_t SPI_FLASH_ReadID(void);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

