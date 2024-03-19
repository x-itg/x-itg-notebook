/**
  ******************************************************************************
  * �ļ�����: bsp_i2c.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: mpu6050ʹ��i2c��������Ӳ����
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
#include "i2c/bsp_i2c.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define EVAL_I2Cx_TIMEOUT_MAX                   3000

/* ˽�б��� ------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c_mpu6050;
uint32_t I2cxTimeout = EVAL_I2Cx_TIMEOUT_MAX;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: I2C�����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_I2C_MPU6050_Init(void)
{
  hi2c_mpu6050.Instance             = MPU6050_I2Cx;
  hi2c_mpu6050.Init.ClockSpeed      = I2C_SPEEDCLOCK;
  hi2c_mpu6050.Init.DutyCycle       = I2C_DUTYCYCLE;
  hi2c_mpu6050.Init.OwnAddress1     = 0;
  hi2c_mpu6050.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  hi2c_mpu6050.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c_mpu6050.Init.OwnAddress2     = 0;
  hi2c_mpu6050.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c_mpu6050.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c_mpu6050);
}

/**
  * ��������: I2C����Ӳ����ʼ������
  * �������: hi2c��I2C�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==MPU6050_I2Cx)
  {  
    /* ʹ������ʱ�� */
    MPU6050_I2C_RCC_CLK_ENABLE();        
    MPU6050_I2C_GPIO_CLK_ENABLE();
    
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = MPU6050_I2C_SCL_PIN|MPU6050_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MPU6050_I2C_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * ��������: I2C����Ӳ������ʼ������
  * �������: hi2c��I2C�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==MPU6050_I2Cx)
  {
    /* ��������ʱ�� */
    MPU6050_I2C_GPIO_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(MPU6050_I2C_GPIO_PORT, MPU6050_I2C_SCL_PIN|MPU6050_I2C_SDA_PIN);
  }
}


/**
  * ��������: I2Cͨ�Ŵ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: һ����I2Cͨ�ų�ʱʱ���øú���
  */
static void I2C_MPU6050_Error (void)
{
  /* ����ʼ��I2Cͨ������ */
  HAL_I2C_DeInit(&hi2c_mpu6050);
  
  /* ���³�ʼ��I2Cͨ������*/
  MX_I2C_MPU6050_Init();
  printf("MPU6050 I2Cͨ�ų�ʱ������ ��������I2C...\n");
}

/**
  * ��������: ͨ��I2Cд��һ��ֵ��ָ���Ĵ�����
  * �������: Addr��I2C�豸��ַ
  *           Reg��Ŀ��Ĵ���
  *           Value��ֵ
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void I2C_MPU6050_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2cxTimeout);
  
  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  }
}

/**
  * ��������: ͨ��I2Cд��һ�����ݵ�ָ���Ĵ�����
  * �������: Addr��I2C�豸��ַ
  *           Reg��Ŀ��Ĵ���
  *           RegSize���Ĵ����ߴ�(8λ����16λ)
  *           pBuffer��������ָ��
  *           Length������������
  * �� �� ֵ: HAL_StatusTypeDef���������
  * ˵    ��: ��ѭ�����������һ����ʱʱ��
  */
HAL_StatusTypeDef I2C_MPU6050_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout); 

  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  }        
  return status;
}


/**
  * ��������: ͨ��I2C��ȡһ��ָ���Ĵ�������
  * �������: Addr��I2C�豸��ַ
  *           Reg��Ŀ��Ĵ���
  * �� �� ֵ: uint8_t���Ĵ�������
  * ˵    ��: ��
  */
uint8_t I2C_MPU6050_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  
  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2cxTimeout);
 
  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  
  }
  return value;
}

/**
  * ��������: ͨ��I2C��ȡһ�μĴ������ݴ�ŵ�ָ���Ļ�������
  * �������: Addr��I2C�豸��ַ
  *           Reg��Ŀ��Ĵ���
  *           RegSize���Ĵ����ߴ�(8λ����16λ)
  *           pBuffer��������ָ��
  *           Length������������
  * �� �� ֵ: HAL_StatusTypeDef���������
  * ˵    ��: ��
  */
HAL_StatusTypeDef I2C_MPU6050_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout);
  
  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  }        
  return status;
}

/**
  * ��������: ���I2C�豸�Ƿ���׼���ÿ���ͨ��״̬
  * �������: DevAddress��I2C�豸��ַ
  *           Trials�����Բ��Դ���
  * �� �� ֵ: HAL_StatusTypeDef���������
  * ˵    ��: ��
  */
HAL_StatusTypeDef I2C_MPU6050_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&hi2c_mpu6050, DevAddress, Trials, I2cxTimeout));
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
