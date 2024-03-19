/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @brief          : Memory management layer
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"
/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_STORAGE 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */ 
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Defines
  * @{
  */ 

//#define Test_0STM_1SST_2 1//0��RAM  1��STMFLASH  2��SPIFLASH


#if Test_0STM_1SST_2==0//����ģʽ��RAM ģ��

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10
#define STORAGE_BLK_SIZ                  0x800
uint8_t msc_data[STORAGE_BLK_NBR][STORAGE_BLK_SIZ]={0};

#elif  Test_0STM_1SST_2==1//�ڲ�FLASH
#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x40
#define STORAGE_BLK_SIZ                  0x800
#define STMFLASHOFFSET                   0x8020000 //128K��ַ��Ϊ�ļ�ϵͳ��ʼ��ַ

#elif Test_0STM_1SST_2==2//SPIfalsh
#include "SST25VF016B.h"
#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x200
#define STORAGE_BLK_SIZ                  0x1000
#endif

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */
  
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Macros
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN INQUIRY_DATA_FS */ 
/* USB Mass storage Standard Inquiry Data */
const int8_t  STORAGE_Inquirydata_FS[] = {/* 36 */
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1',                     /* Version      : 4 Bytes */
}; 
/* USER CODE END INQUIRY_DATA_FS */ 

/* USER CODE BEGIN PRIVATE_VARIABLES */
 

 
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */ 
 
/** @defgroup USBD_STORAGE_IF_Exported_Variables
  * @{
  */ 
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 
  
/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @{
  */
static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun, 
                           uint32_t *block_num, 
                           uint16_t *block_size);
static int8_t  STORAGE_IsReady_FS (uint8_t lun);
static int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */ 
  
USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS,
};

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : STORAGE_Init_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Init_FS (uint8_t lun)
{
  /* USER CODE BEGIN 2 */ 
  HAL_FLASH_Unlock();
  return (USBD_OK);
  /* USER CODE END 2 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_GetCapacity_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */   
  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsReady_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsReady_FS (uint8_t lun)
{
  /* USER CODE BEGIN 4 */ 
  return (USBD_OK);
  /* USER CODE END 4 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsWriteProtected_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  /* USER CODE BEGIN 5 */ 
  return (USBD_OK);
  /* USER CODE END 5 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_Read_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Read_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,                       
                        uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */ 
  uint32_t n,i;
  for(n=0;n<blk_len;n++)
  {
#if Test_0STM_1SST_2==0//����ģʽ��RAM ģ��
    for(i=0;i<STORAGE_BLK_SIZ;i++)
    {
      buf[n*STORAGE_BLK_SIZ+i]=msc_data[blk_addr][i];
    }
#elif Test_0STM_1SST_2==1
    for(i=0;i<STORAGE_BLK_SIZ;i+=4)//�ĸ��ֽڶ�ȡһ��
    {
      *(unsigned int *)&buf[n*STORAGE_BLK_SIZ+i]=(*(unsigned int *)(STMFLASHOFFSET + blk_addr*STORAGE_BLK_SIZ+i)); 
    }
#elif Test_0STM_1SST_2==2
    for(i=0;i<STORAGE_BLK_SIZ;i++)
    {
      SSTF016B_RD(blk_addr*STORAGE_BLK_SIZ+i, &buf[n*STORAGE_BLK_SIZ+i] ,1);
    }
    
#endif
  }
  return (USBD_OK);
  /* USER CODE END 6 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_Write_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Write_FS (uint8_t lun, 
                         uint8_t *buf, 
                         uint32_t blk_addr,
                         uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */ 
 
  uint32_t n,i;
  
  
  
  
  for(n=0;n<blk_len;n++)//n���  ��
  {
#if Test_0STM_1SST_2==0//����ģʽ��RAM ģ��
    for(i=0;i<STORAGE_BLK_SIZ;i++)//����0x800
    {  
       msc_data[blk_addr][i]=buf[n*STORAGE_BLK_SIZ+i];
    }
#elif Test_0STM_1SST_2==1
    //����ɾ��
    HAL_StatusTypeDef flashsta=HAL_OK;
    if(flashsta!=HAL_OK){flashsta=HAL_OK;}
    uint32_t page_error=0;
    FLASH_EraseInitTypeDef page;
    page.NbPages=1;
    page.TypeErase=FLASH_TYPEERASE_PAGES;
    page.PageAddress=STMFLASHOFFSET + blk_addr*STORAGE_BLK_SIZ;//ҳ��ַ=8020000 + ���߼����ַ x һ������ֽ�����
    flashsta=HAL_FLASHEx_Erase(&page,&page_error);
    for(i=0;i<STORAGE_BLK_SIZ;i+=4)//����д��
    {
      //��ʼ��ַ+���ַ һ��֮�� 4��4��д��
      flashsta=HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                 page.PageAddress+i, //��ַ=ҳ��ַ+i
                                 *(unsigned int *)(&buf[n*STORAGE_BLK_SIZ+i]));  //�൱��buf[n*STORAGE_BLK_SIZ+i];      
    }
#elif Test_0STM_1SST_2==2
    SSTF016B_Erase(blk_addr,blk_addr);//д��ǰ��ɾ��
    for(i=0;i<STORAGE_BLK_SIZ;i++)
    {
      SSTF016B_WR(blk_addr*STORAGE_BLK_SIZ+i, &buf[n*STORAGE_BLK_SIZ+i] ,1);
    }
    
#endif
  }  
  return (USBD_OK);
  /* USER CODE END 7 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_GetMaxLun_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetMaxLun_FS (void)
{
  /* USER CODE BEGIN 8 */ 
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */ 
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */ 

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
