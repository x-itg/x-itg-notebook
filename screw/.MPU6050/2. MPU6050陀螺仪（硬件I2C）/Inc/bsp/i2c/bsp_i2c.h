#ifndef __BSP_I2C_H__
#define	__BSP_I2C_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define I2C_OWN_ADDRESS                            0x0A              // stm32本机I2C地址
#define I2C_SPEEDCLOCK                             200000            // I2C通信速率(最大为400K)
#define I2C_DUTYCYCLE                              I2C_DUTYCYCLE_2   // I2C占空比模式：1/2 

#define MPU6050_I2Cx                                I2C1
#define MPU6050_I2C_RCC_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define MPU6050_I2C_RCC_CLK_DISABLE()               __HAL_RCC_I2C1_CLK_DISABLE()

#define MPU6050_I2C_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define MPU6050_I2C_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE()   
#define MPU6050_I2C_GPIO_PORT                       GPIOB   
#define MPU6050_I2C_SCL_PIN                         GPIO_PIN_6
#define MPU6050_I2C_SDA_PIN                         GPIO_PIN_7

/* 扩展变量 ------------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c_mpu6050;

/* 函数声明 ------------------------------------------------------------------*/
void               MX_I2C_MPU6050_Init(void);
void               I2C_MPU6050_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value);
HAL_StatusTypeDef  I2C_MPU6050_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
uint8_t            I2C_MPU6050_ReadData(uint16_t Addr, uint8_t Reg);
HAL_StatusTypeDef  I2C_MPU6050_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
HAL_StatusTypeDef  I2C_MPU6050_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

#endif /* __BSP_I2C_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
