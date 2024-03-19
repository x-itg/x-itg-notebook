#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"

//����������꣬ʹ��Ӳ��iic������ʹ�����iic,
//ʹ��Ӳ��IICʱ������Һ�������ã���ΪFSMC��NADV��IIC1��SDA ��ͬһ�����ţ�����Ӱ����
//������FSMCʱ���Ƽ���Ӳ��IIC

//#define HARD_IIC 

#define MPU_INT_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define MPU_INT_GPIO_PIN                 GPIO_PIN_15
#define MPU_INT_GPIO                     GPIOG
#define MPU_INT_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */
#define MPU_INT_EXTI_IRQHandler          EXTI15_10_IRQHandler
#define MPU_INT_EXTI_IRQn                EXTI15_10_IRQn

/*********************���IICʹ�õĺ�****************************/

//
#define Soft_I2C_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define Soft_I2C_SDA 		                   GPIO_PIN_7
#define Soft_I2C_SCL 		                   GPIO_PIN_6
#define Soft_I2C_PORT 		                 GPIOB
//
#define Soft_I2C_SCL_0 		                 HAL_GPIO_WritePin(Soft_I2C_PORT,Soft_I2C_SCL,GPIO_PIN_RESET)    // ����͵�ƽ
#define Soft_I2C_SCL_1 		                 HAL_GPIO_WritePin(Soft_I2C_PORT,Soft_I2C_SCL,GPIO_PIN_SET)      // ����ߵ�ƽ
#define Soft_I2C_SDA_0 		                 HAL_GPIO_WritePin(Soft_I2C_PORT,Soft_I2C_SDA,GPIO_PIN_RESET)    // ����͵�ƽ
#define Soft_I2C_SDA_1   	                 HAL_GPIO_WritePin(Soft_I2C_PORT,Soft_I2C_SDA,GPIO_PIN_SET)      // ����ߵ�ƽ


/**************************I2C�������壬I2C1��I2C2********************************/
#define I2Cx                                I2C1
#define I2C_RCC_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE() 
#define I2C_RCC_CLK_DISABLE()               __HAL_RCC_I2C1_CLK_DISABLE()
#define I2C_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_PORT                            GPIOB   
#define I2C_SCL_PIN                         GPIO_PIN_6
#define I2C_SDA_PIN                         GPIO_PIN_7

/*�ȴ���ʱʱ��*/
#define I2CT_FLAG_TIMEOUT                   ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT                   ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*��Ϣ���*/
#define MPU_DEBUG_ON                        0
#define MPU_DEBUG_FUNC_ON                   0

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define MPU_DEBUG_FUNC()               do{\
                                         if(MPU_DEBUG_FUNC_ON)\
                                         printf("<<-MPU-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

void I2C_Bus_Init(void);
uint8_t I2C_ByteWrite(uint8_t pBuffer, uint8_t WriteAddr);
uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);																	
void Set_I2C_Retry(unsigned short ml_sec);
unsigned short Get_I2C_Retry(void);
int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, 
                                          unsigned short RegisterLen, unsigned char *RegisterValue);
int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, 
                                           unsigned short RegisterLen, const unsigned char *RegisterValue);
																					
#endif /* __BSP_I2C_H */

																					
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
