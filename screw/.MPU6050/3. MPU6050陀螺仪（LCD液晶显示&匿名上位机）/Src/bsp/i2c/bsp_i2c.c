/**
  ******************************************************************************
  * 文件名程: bsp_i2c.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: mpu6050的I2C总线驱动
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
#include "i2c/bsp_i2c.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define Delay HAL_Delay

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   

/* 私有变量 ------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c_mpu6050;

/* 扩展变量 ------------------------------------------------------------------*/
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;    

/* 函数体 --------------------------------------------------------------------*/
/********************************* Prototypes *********************************/
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue);
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue);
/*******************************  Function ************************************/

//若定义了这个宏，使用硬件iic，否则使用软件iic。这个宏在bsp_i2c.h文件定义
//使用硬件IIC时不能与液晶屏共用，因为FSMC的NADV与IIC1的SDA 是同一个引脚，互相影响了

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
  * 函数功能: I2C外设初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: I2C外设硬件初始化配置
  * 输入参数: hi2c：I2C句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2Cx)
  {  
    /* 使能外设时钟 */
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
  * 函数功能: I2C外设硬件反初始化配置
  * 输入参数: hi2c：I2C句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2Cx)
  {
    /* 禁用外设时钟 */
    I2C_RCC_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(I2C_PORT,I2C_SCL_PIN|I2C_SDA_PIN);
  }
}

/**
  * 函数功能: I2C通信错误处理函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 一般在I2C通信超时时调用该函数
  */
static void I2C_MPU6050_Error (void)
{
  /* 反初始化I2C通信总线 */
  HAL_I2C_DeInit(&hi2c_mpu6050);
  
  /* 重新初始化I2C通信总线*/
  MX_I2C_MPU6050_Init();
  printf("MPU6050 I2C通信超时！！！ 重新启动I2C...\n");
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  MX_I2C_MPU6050_Init();
}


/**
  * @brief  向IIC设备的寄存器连续写入数据
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要写入数据的长度
  * @param  RegisterValue: 要指向写入数据的指针
  * @retval 0正常，非0异常
  */
unsigned long ST_Hard_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050,Address, (uint16_t)RegisterAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)RegisterValue, RegisterLen, I2CTimeout); 

  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  }        
  return status;
}


/**
  * @brief  向IIC设备的寄存器连续读出数据
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要读取的数据长度
  * @param  RegisterValue: 指向存储读出数据的指针
  * @retval 0正常，非0异常
  */
unsigned long ST_Hard_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue)
{  
  HAL_StatusTypeDef status = HAL_OK;
  I2CTimeout = I2CT_LONG_TIMEOUT;
  
  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Address, (uint16_t)RegisterAddr, I2C_MEMADD_SIZE_8BIT, RegisterValue, RegisterLen, I2CTimeout);
  
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  }        
  return status;
}

#endif //endof #ifdef HARD_IIC


/************************软件IIC驱动函数****************************************/
#ifndef HARD_IIC
/**
  * 函数功能: 配置I2C总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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

  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
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
  * 函数功能: I2C总线位延迟，最快400KHz
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void Soft_I2C_Delay(uint32_t dly) 
{
	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

/**
  * 函数功能: CPU发起I2C总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: CPU发起I2C总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
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
	Soft_I2C_SCL_0;//时钟输出0 	   
	return 0;  
} 

/**
  * 函数功能: CPU向I2C总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: CPU从I2C总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
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
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
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
  * 函数功能: 向I2C发送数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: 从I2C读数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * @brief  I2C 外设初始化
  * @param  无
  * @retval 无
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
  * @brief  向IIC设备的寄存器连续写入数据，带超时重试设置，供mpu接口调用
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要写入数据的长度
  * @param  RegisterValue: 要指向写入数据的指针
  * @retval 0正常，非0异常
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
  * @brief  向IIC设备的寄存器连续读出数据,带超时重试设置，供mpu接口调用
  * @param  Address: IIC设备地址
  * @param  RegisterAddr: 寄存器地址
  * @param  RegisterLen: 要读取的数据长度
  * @param  RegisterValue: 指向存储读出数据的指针
  * @retval 0正常，非0异常
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
  * @brief  设置iic重试时间
  * @param  ml_sec：重试的时间，单位毫秒
  * @retval 重试的时间，单位毫秒
  */
void Set_I2C_Retry(unsigned short ml_sec)
{
  RETRY_IN_MLSEC = ml_sec;
}

/**
  * @brief  获取设置的iic重试时间
  * @param  none
  * @retval none
  */
unsigned short Get_I2C_Retry(void)
{
  return RETRY_IN_MLSEC;
}


/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

