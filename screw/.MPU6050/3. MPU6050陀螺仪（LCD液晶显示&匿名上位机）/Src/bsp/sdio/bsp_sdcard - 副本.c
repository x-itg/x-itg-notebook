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
DMA_HandleTypeDef hdma_sdcard;

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
  hsdcard.Init.ClockDiv = 2;
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
    
    /* ʹ��GPIO�˿�ʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* ʹ��DMA������ʱ�� */
    __HAL_RCC_DMA2_CLK_ENABLE();
    
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

    /* SDIO����DMA���� */
    hdma_sdcard.Instance = DMA2_Channel4;
//    hdma_sdcard.Init.Direction = DMA_MEMORY_TO_PERIPH; // �ڴ浽���裬������SD����������
    hdma_sdcard.Init.Direction = DMA_PERIPH_TO_MEMORY; // ���赽�ڴ棬���ڴ�SD����ȡ����
    hdma_sdcard.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdcard.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdcard.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdcard.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdcard.Init.Mode = DMA_NORMAL;
    hdma_sdcard.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    HAL_DMA_Init(&hdma_sdcard);

    /* һЩ����DMA���ָ��ͬһ��DMA�����һ��ʱ��ֻ����Ӧһ��DMAͨ������ */
    /* ȷ���ڵ���HAL_SD_ReadBlocks_DMA �� HAL_SD_WriteBlocks_DMA �ı�DMA���䷽��*/
    __HAL_LINKDMA(hsd,hdmarx,hdma_sdcard);
    __HAL_LINKDMA(hsd,hdmatx,hdma_sdcard);

    /* SDIO�����ж����� */
    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
    
    /* DMA�ж����� */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 1);
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

    /* ����ʼ��DMA*/
    HAL_DMA_DeInit(hsd->hdmarx);
    HAL_DMA_DeInit(hsd->hdmatx);

    /* �ر��ж� */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Channel4_5_IRQn);
  }
} 
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
