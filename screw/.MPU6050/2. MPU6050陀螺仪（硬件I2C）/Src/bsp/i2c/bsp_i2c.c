/**
  ******************************************************************************
  * 文件名程: bsp_i2c.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: mpu6050使用i2c驱动程序（硬件）
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
#define EVAL_I2Cx_TIMEOUT_MAX                   3000

/* 私有变量 ------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c_mpu6050;
uint32_t I2cxTimeout = EVAL_I2Cx_TIMEOUT_MAX;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: I2C外设初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: I2C外设硬件初始化配置
  * 输入参数: hi2c：I2C句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==MPU6050_I2Cx)
  {  
    /* 使能外设时钟 */
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
  * 函数功能: I2C外设硬件反初始化配置
  * 输入参数: hi2c：I2C句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==MPU6050_I2Cx)
  {
    /* 禁用外设时钟 */
    MPU6050_I2C_GPIO_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(MPU6050_I2C_GPIO_PORT, MPU6050_I2C_SCL_PIN|MPU6050_I2C_SDA_PIN);
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
  * 函数功能: 通过I2C写入一个值到指定寄存器内
  * 输入参数: Addr：I2C设备地址
  *           Reg：目标寄存器
  *           Value：值
  * 返 回 值: 无
  * 说    明: 无
  */
void I2C_MPU6050_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2cxTimeout);
  
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  }
}

/**
  * 函数功能: 通过I2C写入一段数据到指定寄存器内
  * 输入参数: Addr：I2C设备地址
  *           Reg：目标寄存器
  *           RegSize：寄存器尺寸(8位或者16位)
  *           pBuffer：缓冲区指针
  *           Length：缓冲区长度
  * 返 回 值: HAL_StatusTypeDef：操作结果
  * 说    明: 在循环调用是需加一定延时时间
  */
HAL_StatusTypeDef I2C_MPU6050_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&hi2c_mpu6050, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout); 

  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  }        
  return status;
}


/**
  * 函数功能: 通过I2C读取一个指定寄存器内容
  * 输入参数: Addr：I2C设备地址
  *           Reg：目标寄存器
  * 返 回 值: uint8_t：寄存器内容
  * 说    明: 无
  */
uint8_t I2C_MPU6050_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  
  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2cxTimeout);
 
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  
  }
  return value;
}

/**
  * 函数功能: 通过I2C读取一段寄存器内容存放到指定的缓冲区内
  * 输入参数: Addr：I2C设备地址
  *           Reg：目标寄存器
  *           RegSize：寄存器尺寸(8位或者16位)
  *           pBuffer：缓冲区指针
  *           Length：缓冲区长度
  * 返 回 值: HAL_StatusTypeDef：操作结果
  * 说    明: 无
  */
HAL_StatusTypeDef I2C_MPU6050_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&hi2c_mpu6050, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout);
  
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_MPU6050_Error();
  }        
  return status;
}

/**
  * 函数功能: 检测I2C设备是否处于准备好可以通信状态
  * 输入参数: DevAddress：I2C设备地址
  *           Trials：尝试测试次数
  * 返 回 值: HAL_StatusTypeDef：操作结果
  * 说    明: 无
  */
HAL_StatusTypeDef I2C_MPU6050_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&hi2c_mpu6050, DevAddress, Trials, I2cxTimeout));
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
