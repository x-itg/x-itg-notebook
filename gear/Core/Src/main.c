/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
signed int UPTIMEOUT = 50; // 50的话表示 50ms 主动上传 -1表示不主动上传
#define SUMC 100
#define SUMCHAN 4
unsigned short AdDataOrig[SUMC][SUMCHAN]; // ADC DMA缓存
unsigned short AdcValue[5];               // ADC的平均值结果
unsigned int poolcnt = 0;
unsigned short readmavalue = 0;
#define ssi_clk_h HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define ssi_clk_l HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define ssi_read_data HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET

void delay_ns(unsigned int ndelay)
{
  unsigned int i = 0;
  unsigned int j = 0;
  for (i = 0; i < 3; i++)
  {
    j++;
  }
}

void delay_us(unsigned int udelay)
{
  unsigned int i = 0;
  for (i = 0; i < udelay; i++)
  {
    delay_ns(600);
  }
}

void delay_ms(unsigned int nms)
{
  for (; nms > 0; nms--)
  {
    delay_us(1000);
  }
}

// 角度传感器 SSI 12bits

unsigned short readma710(void)
{
  unsigned short buf = 0;

  ssi_clk_h;
  delay_us(1);
  ssi_clk_l;
  delay_us(1); // 第一个虚拟时钟
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x8000;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x4000;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x2000;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x1000;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0800;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0400;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0200;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0100;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0080;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0040;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0020;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0010;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0008;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0004;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0002;
  ssi_clk_l;
  delay_us(1);
  ssi_clk_h;
  delay_us(1);
  if (ssi_read_data)
    buf = buf | 0x0001;
  ssi_clk_l;
  delay_us(1);

  return buf;
}

//  开启ADC转换
void startADC(void)
{
  HAL_ADC_Start_DMA(&hadc, (uint32_t *)&AdDataOrig, SUMC * SUMCHAN);
}

// 关闭ADC转换
void stopADC(void)
{
  HAL_ADC_Stop_DMA(&hadc);
}

// ADC转换完成的回调 hal中拷贝出来的  在这个回调中计算adc的平均值
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  unsigned int i, j;
  unsigned int tmp[6] = {0, 0, 0, 0, 0, 0};
  for (j = 0; j < 5; j++) // 通道数量
  {
    tmp[j] = 0;

    switch (j)
    {
    case 0: // 获取ADC数值
    case 1:
    case 2:
    case 3:
      for (i = 0; i < SUMC; i++)
      {
        tmp[j] = tmp[j] + AdDataOrig[i][j];
      }
      AdcValue[j] = tmp[j] / SUMC;
      break;
    case 4: // 获取角度值
      AdcValue[4] = readma710();
      break;
    }
    tmp[j] = 0;
  }
}

// 主动长传传感器数据 4个模拟量 1个角度数据
void AcUpload(unsigned short *PUPS)
{
  unsigned char i;
  char TxBufU1[20];
  i = 0;
  TxBufU1[i++] = 0xFA;
  TxBufU1[i++] = 0xF5;
  TxBufU1[i++] = PUPS[0] >> 0;
  TxBufU1[i++] = PUPS[0] >> 8;
  TxBufU1[i++] = PUPS[1] >> 0;
  TxBufU1[i++] = PUPS[1] >> 8;
  TxBufU1[i++] = PUPS[2] >> 0;
  TxBufU1[i++] = PUPS[2] >> 8;
  TxBufU1[i++] = PUPS[3] >> 0;
  TxBufU1[i++] = PUPS[3] >> 8;
  TxBufU1[i++] = PUPS[4] >> 0;
  TxBufU1[i++] = PUPS[4] >> 8;
  TxBufU1[i++] = 0xFA;
  TxBufU1[i++] = 0xFD;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
  HAL_UART_Transmit_DMA(&huart1, (const uint8_t *)TxBufU1, i); // DMA
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  startADC();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (poolcnt > UPTIMEOUT && UPTIMEOUT != -1)
    {
      poolcnt = 0;
      AcUpload(AdcValue); // 主动长传传感器数据 4个模拟量 1个角度数据
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief ADC Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
   */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
   */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
   */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
   */
  sConfig.Channel = ADC_CHANNEL_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
   */
  sConfig.Channel = ADC_CHANNEL_6;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS485EN_GPIO_Port, RS485EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SLK_OUT_15_GPIO_Port, SLK_OUT_15_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RS485EN_Pin */
  GPIO_InitStruct.Pin = RS485EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SSD_IN_1_Pin */
  GPIO_InitStruct.Pin = SSD_IN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SSD_IN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SLK_OUT_15_Pin */
  GPIO_InitStruct.Pin = SLK_OUT_15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SLK_OUT_15_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
