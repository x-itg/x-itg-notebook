/**
  ******************************************************************************
  * �ļ�����: bsp_sdcard.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: SD���ײ�����ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "sdio/bsp_sdcard.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
SD_HandleTypeDef hsdcard;
HAL_SD_CardInfoTypedef SDCardInfo;
DMA_HandleTypeDef hdma_sdio;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: SD����ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  
  HAL_SD_WideBusOperation_Config(&hsdcard, SDIO_BUS_WIDE_4B); // ����Ϊ4bit SDIO
}

/**
  * ��������: SDIO�����ʼ������
  * �������: hsd��SDIO�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hsd->Instance==SDIO)
  {
    /* ʹ��SDIO����ʱ��*/
    __HAL_RCC_SDIO_CLK_ENABLE();
    
    /* Enable DMA2 clocks */
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    /* ʹ��GPIO�˿�ʱ�� */
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
    
    /* ����DMA���䣺��SD������ʱʹ�� */  
    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_sdio);
    
    /* ����SDIO��DMA���� */
    __HAL_LINKDMA(hsd,hdmarx,hdma_sdio);
    
    /* SDIO�����ж����� */
    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
    
    /* DMA�ж����� */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
  }
}

/**
  * ��������: SDIO���跴��ʼ������
  * �������: hsd��SDIO�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_SD_MspDeInit(SD_HandleTypeDef* hsd)
{
  if(hsd->Instance==SDIO)
  {
    /* ����SDIOʱ�� */
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
    
    /* �ر��ж� */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
  }
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
