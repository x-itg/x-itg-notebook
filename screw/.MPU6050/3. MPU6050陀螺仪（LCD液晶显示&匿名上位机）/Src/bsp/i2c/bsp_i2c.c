/**
  ******************************************************************************
  * �ļ�����: bsp_i2c.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: mpu6050��I2C��������
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
#define Delay HAL_Delay

/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000  //*

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2Cx_OWN_ADDRESS7      0X0A   

/* ˽�б��� ------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c_mpu6050;

/* ��չ���� ------------------------------------------------------------------*/
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;    

/* ������ --------------------------------------------------------------------*/
/********************************* Prototypes *********************************/
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue);
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue);
/*******************************  Function ************************************/

//������������꣬ʹ��Ӳ��iic������ʹ�����iic���������bsp_i2c.h�ļ�����
//ʹ��Ӳ��IICʱ������Һ�������ã���ΪFSMC��NADV��IIC1��SDA ��ͬһ�����ţ�����Ӱ����

#ifdef HARD_IIC

#define ST_Sensors_I2C_WriteRegister  ST_Hard_Sensors_I2C_WriteRegister
#define ST_Sensors_I2C_ReadRegister   ST_Hard_Sensors_I2C_ReadRegister

#else
//
#define Soft_I2C_SDA_STATE   	HAL_GPIO_ReadPin(Soft_I2C_PORT, Soft_I2C_SDA)
#define Soft_I2C_DELAY 				Soft_I2C_Delay(100000)
#define Soft_I2C_NOP					Soft_I2C_Delay(10) 
//
#define Soft_I2C_READY		    0x00
#define Soft_I2C_BUS_BUSY	    0x01	
#define Soft_I2C_BUS_ERROR	  0x02
//
#define Soft_I2C_NACK	        0x00 
#define Soft_I2C_ACK		      0x01
//
static void Soft_I2C_Configuration(void);
static void Soft_I2C_Delay(uint32_t dly);
static uint8_t Soft_I2C_START(void);
static void Soft_I2C_STOP(void);
static void Soft_I2C_SendACK(void);
static void Soft_I2C_SendNACK(void);
static uint8_t Soft_I2C_SendByte(uint8_t anbt_i2c_data);
static uint8_t Soft_I2C_ReceiveByte_WithACK(void);
static uint8_t Soft_I2C_ReceiveByte(void);
static uint8_t Soft_DMP_I2C_Write(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf);
static uint8_t Soft_DMP_I2C_Read(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf);


#define ST_Sensors_I2C_WriteRegister  Soft_DMP_I2C_Write
#define ST_Sensors_I2C_ReadRegister   Soft_DMP_I2C_Read
#define I2C_Direction_Transmitter      ((uint8_t)0x00)
#define I2C_Direction_Receiver         ((uint8_t)0x01)

#endif

#ifdef HARD_IIC

/**
  * ��������: I2C�����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_I2C_MPU6050_Init(void)
{
  hi2c_mpu6050.Instance             = I2Cx;
  hi2c_mpu6050.Init.ClockSpeed      = I2C_Speed;
  hi2c_mpu6050.Init.DutyCycle       = I2C_DUTYCYCLE_2;
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
  if(hi2c->Instance==I2Cx)
  {  
    /* ʹ������ʱ�� */
    I2C_RCC_CLK_ENABLE();        
    I2C_GPIO_CLK_ENABLE();
    
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
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
  if(hi2c->Instance==I2Cx)
  {
    /* ��������ʱ�� */
    I2C_RCC_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(I2C_PORT,I2C_SCL_PIN|I2C_SDA_PIN);
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
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Configu(void)
{
  MX_I2C_MPU6050_Init();
}


/**
  * @brief  ��IIC�豸�ļĴ�������д������
  * @param  Address: IIC�豸��ַ
  * @param  RegisterAddr: �Ĵ�����ַ
  * @param  RegisterLen: Ҫд�����ݵĳ���
  * @param  RegisterValue: Ҫָ��д�����ݵ�ָ��
  * @retval 0��������0�쳣
  */
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050,Address, (uint16_t)RegisterAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)RegisterValue, RegisterLen, I2CTimeout); 

  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  }        
  return status;
}


/**
  * @brief  ��IIC�豸�ļĴ���������������
  * @param  Address: IIC�豸��ַ
  * @param  RegisterAddr: �Ĵ�����ַ
  * @param  RegisterLen: Ҫ��ȡ�����ݳ���
  * @param  RegisterValue: ָ��洢�������ݵ�ָ��
  * @retval 0��������0�쳣
  */
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue)
{  
  HAL_StatusTypeDef status = HAL_OK;
  I2CTimeout = I2CT_LONG_TIMEOUT;
  
  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Address, (uint16_t)RegisterAddr, I2C_MEMADD_SIZE_8BIT, RegisterValue, RegisterLen, I2CTimeout);
  
  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ������� */
    I2C_MPU6050_Error();
  }        
  return status;
}

#endif //endof #ifdef HARD_IIC


/************************���IIC��������****************************************/
#ifndef HARD_IIC
/**
  * ��������: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Soft_I2C_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	Soft_I2C_GPIO_CLK_ENABLE(); 
	//
	//
  GPIO_InitStruct.Pin = Soft_I2C_SCL|Soft_I2C_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(Soft_I2C_PORT, &GPIO_InitStruct);

  /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	Soft_I2C_SDA_0;
	Soft_I2C_SCL_1;
	Soft_I2C_DELAY;
	//
	//
	Soft_I2C_SCL_1; 
	Soft_I2C_SDA_1;
	Soft_I2C_DELAY; 
}

/**
  * ��������: I2C����λ�ӳ٣����400KHz
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Soft_I2C_Delay(uint32_t dly) 
{
	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

/**
  * ��������: CPU����I2C���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static uint8_t Soft_I2C_START(void)
{ 
	Soft_I2C_SDA_1; 
 	Soft_I2C_NOP;
  // 
 	Soft_I2C_SCL_1; 
 	Soft_I2C_NOP;    
	//
 	if(!Soft_I2C_SDA_STATE) return Soft_I2C_BUS_BUSY;
	//
 	Soft_I2C_SDA_0;
 	Soft_I2C_NOP;
  //
 	Soft_I2C_SCL_0;  
 	Soft_I2C_NOP; 
	//
 	if(Soft_I2C_SDA_STATE) return Soft_I2C_BUS_ERROR;
	//
 	return Soft_I2C_READY;
}

/**
  * ��������: CPU����I2C����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Soft_I2C_STOP(void)
{
 	Soft_I2C_SDA_0; 
 	Soft_I2C_NOP;
  // 
 	Soft_I2C_SCL_1; 
 	Soft_I2C_NOP;    
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_NOP;
}

/**
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Soft_I2C_SendACK(void)
{
 	Soft_I2C_SDA_0;
 	Soft_I2C_NOP;
 	Soft_I2C_SCL_1;
 	Soft_I2C_NOP;
 	Soft_I2C_SCL_0; 
 	Soft_I2C_NOP;  
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void Soft_I2C_SendNACK(void)
{
	Soft_I2C_SDA_1;
	Soft_I2C_NOP;
	Soft_I2C_SCL_1;
	Soft_I2C_NOP;
	Soft_I2C_SCL_0; 
	Soft_I2C_NOP;  
}

/**
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t Soft_I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;

	Soft_I2C_SDA_1;
	Soft_I2C_NOP;	   
	Soft_I2C_SCL_1;
	Soft_I2C_NOP;	 
	
	while(Soft_I2C_SDA_STATE)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Soft_I2C_STOP();
			return Soft_I2C_BUS_ERROR;
		}
	}
	Soft_I2C_SCL_0;//ʱ�����0 	   
	return 0;  
} 

/**
  * ��������: CPU��I2C�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
static uint8_t Soft_I2C_SendByte(uint8_t soft_i2c_data)
{
 	uint8_t i;
 	
	Soft_I2C_SCL_0;
 	for(i=0;i<8;i++)
 	{  
    if(soft_i2c_data&0x80) Soft_I2C_SDA_1;
    else Soft_I2C_SDA_0;
    //
    soft_i2c_data<<=1;
    Soft_I2C_NOP;
    //
    Soft_I2C_SCL_1;
    Soft_I2C_NOP;
    Soft_I2C_SCL_0;
    Soft_I2C_NOP; 
 	}
	return Soft_I2C_Wait_Ack();  
}

/**
  * ��������: CPU��I2C�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
static uint8_t Soft_I2C_ReceiveByte(void)
{
	uint8_t i,soft_i2c_data;
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_SCL_0; 
 	soft_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
    Soft_I2C_SCL_1;
    Soft_I2C_NOP; 
    soft_i2c_data<<=1;
    //
    if(Soft_I2C_SDA_STATE)	soft_i2c_data|=0x01; 

    Soft_I2C_SCL_0;  
    Soft_I2C_NOP;         
 	}
	Soft_I2C_SendNACK();
 	return soft_i2c_data;
}

/**
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
static uint8_t Soft_I2C_ReceiveByte_WithACK(void)
{
	uint8_t i,soft_i2c_data;
	//
 	Soft_I2C_SDA_1;
 	Soft_I2C_SCL_0; 
 	soft_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
    Soft_I2C_SCL_1;
    Soft_I2C_NOP; 
    soft_i2c_data<<=1;
    //
    if(Soft_I2C_SDA_STATE)	soft_i2c_data|=0x01; 

    Soft_I2C_SCL_0;  
    Soft_I2C_NOP;         
 	}
	Soft_I2C_SendACK();
 	return soft_i2c_data;
}

/**
  * ��������: ��I2C��������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static uint8_t Soft_DMP_I2C_Write(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf)
{		
  uint8_t i, result=0;
  Soft_I2C_START();
  result  = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Transmitter);	
  if(result != 0) return result;

  result = Soft_I2C_SendByte(soft_reg_addr);  
  if(result != 0) return result;

  for (i=0;i<soft_i2c_len;i++) 
  {
    result = Soft_I2C_SendByte(soft_i2c_data_buf[i]); 
    if (result != 0) return result;
  }
  Soft_I2C_STOP();
  return 0x00;
}

/**
  * ��������: ��I2C������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static uint8_t Soft_DMP_I2C_Read(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len,unsigned char *soft_i2c_data_buf)
{
  uint8_t result;

  Soft_I2C_START();
  result  = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Transmitter);			
  if(result != 0) return result;

  result = Soft_I2C_SendByte(soft_reg_addr); 
  if(result != 0) return result;

  Soft_I2C_START();
  result = Soft_I2C_SendByte(soft_dev_addr << 1 | I2C_Direction_Receiver);
  if(result != 0) return result;

  //
  while (soft_i2c_len)
  {
    if (soft_i2c_len==1) *soft_i2c_data_buf =Soft_I2C_ReceiveByte();  
    else *soft_i2c_data_buf =Soft_I2C_ReceiveByte_WithACK();
    soft_i2c_data_buf++;
    soft_i2c_len--;
  }
  Soft_I2C_STOP();
  return 0x00;
}

#endif //endof #ifndef HARD_IIC



/**
  * @brief  I2C �����ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_Bus_Init(void)
{	
	Set_I2C_Retry(5);
	
#ifdef HARD_IIC
	MPU_DEBUG("hard iic");	  
  I2C_Mode_Configu();	
#else	
  MPU_DEBUG("soft iic");
	Soft_I2C_Configuration();
#endif
}






/**
  * @brief  ��IIC�豸�ļĴ�������д�����ݣ�����ʱ�������ã���mpu�ӿڵ���
  * @param  Address: IIC�豸��ַ
  * @param  RegisterAddr: �Ĵ�����ַ
  * @param  RegisterLen: Ҫд�����ݵĳ���
  * @param  RegisterValue: Ҫָ��д�����ݵ�ָ��
  * @retval 0��������0�쳣
  */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len,
                                        const unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();

tryWriteAgain:
  ret = 0;
  ret = ST_Sensors_I2C_WriteRegister( slave_addr, reg_addr, len, ( unsigned char *)data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;

    Delay(retry_in_mlsec);
    goto tryWriteAgain;
  }
  return ret;
}


/**
  * @brief  ��IIC�豸�ļĴ���������������,����ʱ�������ã���mpu�ӿڵ���
  * @param  Address: IIC�豸��ַ
  * @param  RegisterAddr: �Ĵ�����ַ
  * @param  RegisterLen: Ҫ��ȡ�����ݳ���
  * @param  RegisterValue: ָ��洢�������ݵ�ָ��
  * @retval 0��������0�쳣
  */
int Sensors_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len,
                                       unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();

tryReadAgain:
  ret = 0;
  ret = ST_Sensors_I2C_ReadRegister( slave_addr, reg_addr, len, ( unsigned char *)data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;

    Delay(retry_in_mlsec);
    goto tryReadAgain;
  }
  return ret;
}


static unsigned short RETRY_IN_MLSEC  = 55;

/**
  * @brief  ����iic����ʱ��
  * @param  ml_sec�����Ե�ʱ�䣬��λ����
  * @retval ���Ե�ʱ�䣬��λ����
  */
void Set_I2C_Retry(unsigned short ml_sec)
{
  RETRY_IN_MLSEC = ml_sec;
}

/**
  * @brief  ��ȡ���õ�iic����ʱ��
  * @param  none
  * @retval none
  */
unsigned short Get_I2C_Retry(void)
{
  return RETRY_IN_MLSEC;
}


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

