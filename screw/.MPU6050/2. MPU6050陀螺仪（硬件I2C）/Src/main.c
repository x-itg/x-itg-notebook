/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: MPU6050�����ǣ�Ӳ��I2C��
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
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "usart/bsp_debug_usart.h"
#include "i2c/bsp_i2c.h"
#include "mpu6050/mpu6050.h"
#include "led/bsp_led.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define TASK_ENABLE 0

/* ˽�б��� ------------------------------------------------------------------*/             
/* ��չ���� ------------------------------------------------------------------*/
extern unsigned int Task_Delay[NumOfTask];

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;           // 9��Ƶ���õ�72MHz��ʱ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;      // ϵͳʱ�ӣ�72MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ�72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                // APB1ʱ�ӣ�36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                // APB2ʱ�ӣ�72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    --> 1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 --> 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 --> 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{  
  short Accel[3];
	short Gyro [3];
	short Temp;
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();
  /*LED��ʼ��*/
  LED_GPIO_Init();
  /*I2C��ʼ��*/
  MX_I2C_MPU6050_Init();
  
  /*MPU6050��ʼ��*/
  MPU6050_Init();

  /* ���ø�ʽ�����������ӡ������� */
	//���MPU6050
	if (MPU6050ReadID() == 0)
	{	
    printf("��ⲻ��MPU6050��ͣ��");
	  while(1);
  }
  /* ����ѭ�� */
  while (1)
  {
    if(Task_Delay[0]==TASK_ENABLE)
    {
      LED1_TOGGLE;
      Task_Delay[0]=1000;
    }
    
    if(Task_Delay[1]==0)
    {
      MPU6050ReadAcc(Accel);			
      printf("\r\n���ٶȣ� %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
      MPU6050ReadGyro(Gyro);
      printf("�����ǣ� %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
      
      MPU6050_ReturnTemp(&Temp);
      printf("�¶ȣ� %d",Temp);
      
      Task_Delay[1]=100;//��ֵÿ1ms���1������0�ſ������½����������ִ�е�������100ms
    }
  }
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
