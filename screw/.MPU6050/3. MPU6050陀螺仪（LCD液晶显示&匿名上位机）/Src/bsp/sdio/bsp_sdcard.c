/**
  ******************************************************************************
  * 文件名程: bsp_sdcard.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: SD卡底层驱动实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "sdio/bsp_sdcard.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
SD_HandleTypeDef hsdcard;
HAL_SD_CardInfoTypedef SDCardInfo;
DMA_HandleTypeDef hdma_sdio;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: SD卡初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void MX_SDIO_SD_Init(void)
{
  hsdcard.Instance = SDIO;
  hsdcard.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsdcard.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsdcard.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsdcard.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsdcard.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsdcard.Init.ClockDiv = 4;
  HAL_SD_Init(&hsdcard, &SDCardInfo);
  
  HAL_SD_WideBusOperation_Config(&hsdcard, SDIO_BUS_WIDE_4B); // 设置为4bit SDIO
}

/**
  * 函数功能: SDIO外设初始化配置
  * 输入参数: hsd：SDIO句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hsd->Instance==SDIO)
  {
    /* 使能SDIO外设时钟*/
    __HAL_RCC_SDIO_CLK_ENABLE();
    
    /* Enable DMA2 clocks */
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    /* 使能GPIO端口时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /**SDIO GPIO Configuration    
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* 配置DMA传输：读SD卡内容时使用 */  
    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_sdio);
    
    /* 链接SDIO的DMA接收 */
    __HAL_LINKDMA(hsd,hdmarx,hdma_sdio);
    
    /* SDIO外设中断配置 */
    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
    
    /* DMA中断配置 */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
  }
}

/**
  * 函数功能: SDIO外设反初始化配置
  * 输入参数: hsd：SDIO句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_SD_MspDeInit(SD_HandleTypeDef* hsd)
{
  if(hsd->Instance==SDIO)
  {
    /* 禁用SDIO时钟 */
    __HAL_RCC_SDIO_CLK_DISABLE();
  
    /**SDIO GPIO Configuration    
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    
    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hsd->hdmarx);
    
    /* 关闭中断 */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
  }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
