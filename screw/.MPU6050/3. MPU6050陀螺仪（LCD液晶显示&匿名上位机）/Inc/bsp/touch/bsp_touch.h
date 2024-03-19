#ifndef __BSP_XPT2046_H__
#define	__BSP_XPT2046_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
#pragma pack(4)     // 4�ֽڶ���
typedef struct 
{	
  int32_t cal_flag;
  int32_t lcd_x[5];
	int32_t lcd_y[5];
  int32_t touch_x[5];
  int32_t touch_y[5];
	int32_t adjust[7];
}XPT2046_Calibration;
#pragma pack()

/* �궨�� --------------------------------------------------------------------*/
/******************************* XPT2046 �������ж����Ŷ��� ***************************/
#define XPT2046_EXTI_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE() 
#define XPT2046_EXTI_PORT                            GPIOE
#define XPT2046_EXTI_PIN                             GPIO_PIN_2
#define XPT2046_EXTI_ActiveLevel                     0
#define XPT2046_EXTI_Read()                          HAL_GPIO_ReadPin(XPT2046_EXTI_PORT,XPT2046_EXTI_PIN )

/******************************* XPT2046 ������ģ��SPI���Ŷ��� ***************************/
#define XPT2046_SPI_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOE_CLK_ENABLE()
#define XPT2046_SPI_PORT		                         GPIOE
#define XPT2046_SPI_CS_PIN		                       GPIO_PIN_6
#define	XPT2046_SPI_CLK_PIN	                         GPIO_PIN_3
#define	XPT2046_SPI_MOSI_PIN	                       GPIO_PIN_4
#define	XPT2046_SPI_MISO_PIN	                       GPIO_PIN_5

#define XPT2046_CS_HIGT()                            HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_CS_PIN,GPIO_PIN_SET)
#define XPT2046_CS_LOW()                             HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_CS_PIN,GPIO_PIN_RESET)

#define XPT2046_CLK_HIGH()                           HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_CLK_PIN,GPIO_PIN_SET)
#define XPT2046_CLK_LOW()                            HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_CLK_PIN,GPIO_PIN_RESET)

#define XPT2046_MOSI_HIGH()                          HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_MOSI_PIN,GPIO_PIN_SET)
#define XPT2046_MOSI_LOW()                           HAL_GPIO_WritePin(XPT2046_SPI_PORT,XPT2046_SPI_MOSI_PIN,GPIO_PIN_RESET)

#define XPT2046_MISO_READ()                          HAL_GPIO_ReadPin(XPT2046_SPI_PORT,XPT2046_SPI_MISO_PIN)

/******************************* XPT2046 �������������� ***************************/
#define	XPT2046_CHANNEL_X 	                          0x90 	          //����YN YP������X+	
#define	XPT2046_CHANNEL_Y 	                          0xd0	          //����XN XP������Y+

#define XPT2046_SPIFLASH_ADDR                         (0)

/* ��չ���� ------------------------------------------------------------------*/
extern XPT2046_Calibration cal_value;

/* �������� ------------------------------------------------------------------*/
void Touch_Init_GPIO(void);
uint8_t XPT2046_Touch_Calibrate(void);
void XPT2046_Get_TouchedPoint(uint16_t *pLCD_x,uint16_t *pLCD_y);
void Palette_draw_point(uint16_t x, uint16_t y,uint16_t usColor);
#endif /* __BSP_XPT2046_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
