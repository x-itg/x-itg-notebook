/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "main.h"
#include "stm32f1xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "oled.h"
#include "LCD_ZK.h"
#include "bsp_spiflash.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#include "stdlib.h"
FATFS fs;/* FatFs文件系统对象 */
FIL file;/* 文件对象 */
FRESULT f_res;   
unsigned int  fnum;  //文件成功读写数量 
volatile unsigned char keyvalue=0;
volatile unsigned int timecnt=0;
extern void delay_ms(unsigned int ms);
unsigned char nextflag=1;
unsigned char str[30];


void keyinit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//输入
  GPIO_InitStruct.Pull=GPIO_PULLUP;//上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

 

unsigned char readkey(void)
{
  volatile unsigned short tmp1=GPIOA->IDR;
  volatile unsigned short tmp2=GPIOA->IDR;
  volatile unsigned short tmp3=GPIOA->IDR;
  tmp1=(tmp1&(1<<8));
  tmp2=(tmp2&(1<<9));
  tmp3=(tmp3&(1<<10));
  if(tmp1==0)
  {
    return 1;
  }
  else if(tmp2==0)
  {
    return 2;
  }
  else if(tmp3==0)
  {
    return 3;
  }
  return 0;
  
  
}
#define Copyaddr 0x400000
#define Copyend  0x7FFFFF
unsigned int size;
//将文件读到flash
unsigned char copyflash(unsigned char nextf)
{  
 
  unsigned char readtmp=0;
  unsigned int i;
  
  if(nextf==1)
  {
    size=file.fsize;
  }else
  {
    size=0;
  }

  for(i=0;i<size;i++)
  {
    if((i%0x1000)==0)
    {
      SPI_FLASH_SectorErase(i+Copyaddr);//写入前先删除
    }
    f_res=f_read(&file,&readtmp,1,&fnum); //一次读一个字节直到读完全部文件信息
    SPI_FLASH_BufferWrite( &readtmp ,i+Copyaddr,1);
    if(f_eof(&file))
    {
      f_close(&file);
      return 1;
    }else if(f_res!=FR_OK)
    {
      f_close(&file);
      sprintf((char *)str,"挂载失败%d",f_res);
      display_GB2312_string(2,1,str);
      return 1;
    }
    //SPI_FLASH_BufferRead( &readjup ,i+Copyaddr,1);
  }
  
  return 0;
}
unsigned int index=0;
unsigned char bufs[16];
unsigned char bufx[16];
unsigned char buffer[33]={0};
unsigned char ReadBuffer(unsigned int addrindex,unsigned char xs)
{  

  
  unsigned int addr;
  unsigned char snumread;
  unsigned char xnumread;
  if(xs==2)
  {
    if(index>31)
    {
      index=addrindex-32;//不管上次显示了多少个一律上翻32个
    }
    if(index<30)index=0;
    addr=addrindex+Copyaddr;
    
  }else
  {
    addr=index+Copyaddr;
  }
  //上面
  SPI_FLASH_BufferRead(bufs,addr,16);
  if(bufs[14]>=0x20&&bufs[14]<=0x7e&&(bufs[15]<0x20||bufs[15]>0x7e))
  {
    bufs[15]=' ';
    snumread=15;//最后一个中文显示不下
  }else
  {
    snumread=16;//正常读了16个
  }
  //下面
  SPI_FLASH_BufferRead(bufx,addr+snumread,16);
  if(bufx[14]>=0x20&&bufx[14]<=0x7e&&(bufx[15]<0x20||bufx[15]>0x7e))
  {
    bufx[15]=' ';
    xnumread=15;//最后一个中文显示不下
  }else
  {
    xnumread=16;//正常读了16个
  }  
 
  if(xs==1)
  {
    index=index+snumread+xnumread;//一共读到的个数
  }
  memcpy(buffer,bufs,16);
  memcpy(&buffer[16],bufx,16);
  buffer[32]=0;
  return 1;
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  FLASH_SPI_CS_DISABLE();//刚开机不能片选 不然影响驱动
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();

  /* USER CODE BEGIN 2 */
  //HAL_FLASH_Unlock();
  keyinit();
  initial_rom();
  
  OLED_Init();
  
  OLED_Clear();
  display_GB2312_string(2,1,"----欢迎使用----");
  delay_ms(2000); 

  //加载驱动
  if(nextflag==1)
  {
    OLED_Clear();
    if(FATFS_LinkDriver(&USER_Driver, USER_Path) == 0)// 注册一个FatFS设备：串行FLASH 
    {
      nextflag=1;
    }else
    {
      nextflag=0;
      display_GB2312_string(2,1,"FATFS驱动加载失败！");
    }
  }
  //挂载文件系统
  if(nextflag==1)
  { 
    OLED_Clear();
    f_res =f_mount(&fs,(TCHAR const*)USER_Path,1);
    if(f_res==FR_OK)
    {
      nextflag=1;
      display_GB2312_string(2,1,"文件系统挂载成功！");
    }else
    {
      nextflag=0;
      sprintf((char *)str,"挂载失败%d",f_res);
      display_GB2312_string(2,1,str);
    }
  }
   
  unsigned char TXTPath[9];
  TXTPath[0]=USER_Path[0];
  TXTPath[1]=USER_Path[1];
  TXTPath[2]=USER_Path[2];
  TXTPath[3]='1';
  TXTPath[4]='.';
  TXTPath[5]='t';
  TXTPath[6]='x';
  TXTPath[7]='t';
  TXTPath[8]=0;
  //打开文件
  if(nextflag==1)
  {
    OLED_Clear();
    if(FR_OK ==f_open(&file, (const char *)TXTPath, FA_OPEN_EXISTING|FA_READ))
    {
      nextflag=1;
      sprintf((char *)str,"TXT大小:%d",file.fsize);
      display_GB2312_string(2,1,str);
      
    }else
    {
      nextflag=0;
      sprintf((char *)str,"打开失败%d",f_res);
      display_GB2312_string(2,1,str);
    }
  }
  copyflash(nextflag);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
  
  /* USER CODE BEGIN 3 */
    if(keyvalue==1||keyvalue==2)
    { 
 Lgto: 
      OLED_Clear();
      if(index<size+32)
      {
        if(ReadBuffer(index,keyvalue)==1)
        {
          display_GB2312_string(2,1,buffer);
        }
      }else
      {
        display_GB2312_string(2,1,"-------完-------");
        index=0;
      }
      keyvalue=0;
    }else if(keyvalue==3)
    {
      static unsigned char flagrest=0;
      
      keyvalue=0;
      index=0;
      if(flagrest==0)
      {
        flagrest=1;
        OLED_Clear();
      }else if(flagrest==1)
      {
        flagrest=0;
        goto Lgto;
      }
    }
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|SPISI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPICK_Pin|SPICS_Pin|SCL_Pin|SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPISI_Pin */
  GPIO_InitStruct.Pin = SPISI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPISI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPISO_Pin */
  GPIO_InitStruct.Pin = SPISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SPISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPICK_Pin SPICS_Pin SCL_Pin SDA_Pin */
  GPIO_InitStruct.Pin = SPICK_Pin|SPICS_Pin|SCL_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
