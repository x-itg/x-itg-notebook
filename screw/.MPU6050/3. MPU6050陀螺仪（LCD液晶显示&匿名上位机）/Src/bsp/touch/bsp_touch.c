/**
  ******************************************************************************
  * �ļ�����: bsp_touch.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���败���ײ���������
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
#include "lcd/bsp_lcd.h"
#include "usart/bsp_debug_usart.h"
#include "spiflash/bsp_spiflash.h"
#include "touch/bsp_touch.h"
#include <stdio.h> 
#include <string.h>

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
XPT2046_Calibration cal_value={0};

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ���败�����GPIO��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
 void Touch_Init_GPIO(void)
{ 
  GPIO_InitTypeDef GPIO_InitStruct;

  /* ����GPIOʱ�� */
  XPT2046_EXTI_GPIO_CLK_ENABLE();
  XPT2046_SPI_GPIO_CLK_ENABLE();

  /* ģ��SPI GPIO��ʼ�� */
  GPIO_InitStruct.Pin = XPT2046_SPI_CLK_PIN|XPT2046_SPI_MOSI_PIN|XPT2046_SPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(XPT2046_SPI_PORT, &GPIO_InitStruct);


  GPIO_InitStruct.Pin  = XPT2046_SPI_MISO_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XPT2046_SPI_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = XPT2046_EXTI_PIN;
  HAL_GPIO_Init(XPT2046_EXTI_PORT, &GPIO_InitStruct);
   
  /* ����Ƭѡ��ѡ��XPT2046 */
  XPT2046_CS_LOW();
}

/**
  * ��������: us ������ʱ�����Ǻܾ�ȷ
  * �������: cnt����ʱʱ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //����ֵΪ12����Լ��1΢��  	      
		while ( uc -- );     //��1΢��	
	}	
}

/**
  * ��������: д����
  * �������: CHX 	0x90 	//ͨ��Y+��ѡ������� 
  *           CHY   0xd0	//ͨ��X+��ѡ�������
  * �� �� ֵ: ��
  * ˵    ������
  */
static void XPT2046_WriteCMD(uint8_t ucCmd)
{
	uint8_t i;
  
	XPT2046_MOSI_LOW();	
	XPT2046_CLK_LOW();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? XPT2046_MOSI_HIGH() : XPT2046_MOSI_LOW();		
	  XPT2046_DelayUS ( 5 );		
		XPT2046_CLK_HIGH();
	  XPT2046_DelayUS ( 5 );
		XPT2046_CLK_LOW();
	}	
}

/**
  * ��������: ������
  * �������: ��
  * �� �� ֵ: short������������
  * ˵    ������
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;

	XPT2046_MOSI_LOW();
	XPT2046_CLK_HIGH();
	for ( i=0;i<12;i++ ) 
	{
		XPT2046_CLK_LOW();	
		usTemp = XPT2046_MISO_READ();		
		usBuf |= usTemp << ( 11 - i );	
		XPT2046_CLK_HIGH();		
	}	
	return usBuf;
}

/**
  * ��������: ѡ��һ��ģ��ͨ��������ADC��������ADC�������
  * �������: _ucCh = 0x90 ��ʾYͨ����
  *                   0xd0 ��ʾXͨ��
  * �� �� ֵ: ��
  * ˵    ������
  */
uint16_t XPT2046_ReadAdc(uint8_t _ucCh)
{
  XPT2046_WriteCMD(_ucCh);
	return 	XPT2046_ReadCMD();
}

/**
  * ��������: У������ʱ��ʮ��ר�� 
  * �������: x��ʮ���е�x��
  *           y��ʮ���е�y��
  * �� �� ֵ: ��
  * ˵    ������
  */
void LCD_DrawCross(uint16_t x,uint16_t y)
{
  LCD_DrawLine(x-10,y,x+10,y,RED);
  LCD_DrawLine(x,y-10,x,y+10,RED);
}

/**
  * ��������: ��ȡTP x y ��ADֵ(12bit�������4096)
  * �������: x������ADֵ
  *           y������ADֵ
  * �� �� ֵ: ��
  * ˵    ������
  */
void XPT2046_ReadAdc_XY(int16_t *sX_Ad,int16_t *sY_Ad)  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 	
	
	sX_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
	XPT2046_DelayUS(10); 
	sY_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_Y); 		
	* sY_Ad = sX_Ad_Temp; 
	* sX_Ad = sY_Ad_Temp;	
}

/**
  * ��������: �õ����˲�֮���X Y
  * �������: pTouch_AD_x���������X+��Ӧ��ADֵ
  *           pTouch_AD_y���������Y+��Ӧ��ADֵ
  *           wait���ɿ��ȴ�ʹ��
  *            ���� 1��ʹ�ܵȴ��ɿ�
  *                 0������ȴ��ɿ�
  * �� �� ֵ: ��
  * ˵    ��������Ӧ��ʵ��ר��,���Ǻܾ�׼�������ٶȱȽϿ�
  */
static uint8_t XPT2046_ReadAdc_Smooth_XY(int32_t *pTouch_AD_x,int32_t *pTouch_AD_y,uint8_t wait)
{
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray[2][10]={{0},{0}};  //����X��Y���ж�β���	
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  while(ucCount<10)
  {
    if(XPT2046_EXTI_Read() == XPT2046_EXTI_ActiveLevel)
    {
      XPT2046_ReadAdc_XY(&sAD_X,&sAD_Y);      
      sBufferArray[0][ucCount]=sAD_X;  
      sBufferArray[1][ucCount]=sAD_Y;
      ucCount ++;     
    }
    else if(wait==0)
      break;
  }
  
  while(wait)
  {
    if(XPT2046_EXTI_Read() != XPT2046_EXTI_ActiveLevel)break;
  }
  
	if(ucCount==10)
  {
    lX_Max=lX_Min=sBufferArray[0][0];
    lY_Max=lY_Min=sBufferArray[1][0];       
    for(i=1;i<10;i++)
    {
      if(sBufferArray[0][i]<lX_Min)
        lX_Min=sBufferArray[0][i];
      else if(sBufferArray[0][i]>lX_Max)
        lX_Max=sBufferArray[0][i];
    }
    for(i=1;i<10;i++)
    {
      if(sBufferArray[1][i]<lY_Min)
        lY_Min=sBufferArray[1][i];			
      else if(sBufferArray[1][i]>lY_Max)
        lY_Max=sBufferArray[1][i];
    }		
    
    /*ȥ����Сֵ�����ֵ֮����ƽ��ֵ*/
    *pTouch_AD_x=(sBufferArray[0][0]+sBufferArray[0][1]+sBufferArray[0][2]+sBufferArray[0][3]+sBufferArray[0][4]+
               sBufferArray[0][5]+sBufferArray[0][6]+sBufferArray[0][7]+sBufferArray[0][8]+sBufferArray[0][9]-lX_Min-lX_Max)>>3;
    *pTouch_AD_y=(sBufferArray[1][0]+sBufferArray[1][1]+sBufferArray[1][2]+sBufferArray[1][3]+sBufferArray[1][4]+
               sBufferArray[1][5]+sBufferArray[1][6]+sBufferArray[1][7]+sBufferArray[1][8]+sBufferArray[1][9]-lX_Min-lX_Max)>>3;
    return 0;
  }
  *pTouch_AD_x=-1;
  *pTouch_AD_y=-1;
  return 1;
}

/**
  * ��������: ������У׼�㷨ʵ��
  * �������: XPT2046_Calibration�ṹ��ָ��
  * �� �� ֵ: 0������ɹ���1���޷�����
  * ˵    ������
  */
static uint8_t perform_calibration(XPT2046_Calibration *cal) 
{
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

// Get sums for matrix
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) {
		n += 1.0;
		x += (float)cal->touch_x[j];
		y += (float)cal->touch_y[j];
		x2 += (float)(cal->touch_x[j]*cal->touch_x[j]);
		y2 += (float)(cal->touch_y[j]*cal->touch_y[j]);
		xy += (float)(cal->touch_x[j]*cal->touch_y[j]);
	}

// Get determinant of matrix -- check if determinant is too small
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
//		printf("ts_calibrate: determinant is too small -- %f\n\r",det);
		return 1;
	}

// Get elements of inverse matrix
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

// Get sums for x calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->lcd_x[j];
		zx += (float)(cal->lcd_x[j]*cal->touch_x[j]);
		zy += (float)(cal->lcd_x[j]*cal->touch_y[j]);
	}

// Now multiply out to get the calibration for framebuffer x coord
	cal->adjust[0] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
	cal->adjust[1] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
	cal->adjust[2] = (int32_t)((c*z + f*zx + i*zy)*(scaling));

//	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
//				(b*z + e*zx + f*zy),
//				(c*z + f*zx + i*zy));

// Get sums for y calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->lcd_y[j];
		zx += (float)(cal->lcd_y[j]*cal->touch_x[j]);
		zy += (float)(cal->lcd_y[j]*cal->touch_y[j]);
	}

// Now multiply out to get the calibration for framebuffer y coord
	cal->adjust[3] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
	cal->adjust[4] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
	cal->adjust[5] = (int32_t)((c*z + f*zx + i*zy)*(scaling));

//	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
//				(b*z + e*zx + f*zy),
//				(c*z + f*zx + i*zy));

// If we got here, we're OK, so assign scaling to a[6] and return
	cal->adjust[6] = (int32_t)scaling;
	return 0;
}
 
/**
  * ��������: ������У������
  * �������: ��
  * �� �� ֵ: 0��У���ɹ�
  *           1��У��ʧ��
  * ˵    ������
  */
uint8_t XPT2046_Touch_Calibrate(void)
{  
  uint8_t i;  

  uint16_t usTest_x=0,usTest_y=0;  
  
  /* �趨��ʮ���ֽ��������� */ 
  cal_value.lcd_x[0]=20;
  cal_value.lcd_y[0]=20;
  
  cal_value.lcd_x[1]=20;
  cal_value.lcd_y[1]=LCD_DEFAULT_HEIGTH-20;
  
  cal_value.lcd_x[2]=LCD_DEFAULT_WIDTH-20;
  cal_value.lcd_y[2]=cal_value.lcd_y[1];
  
  cal_value.lcd_x[3]=cal_value.lcd_x[2];
  cal_value.lcd_y[3]=cal_value.lcd_y[0];	
  
  cal_value.lcd_x[4]=LCD_DEFAULT_WIDTH/2;
  cal_value.lcd_y[4]=LCD_DEFAULT_HEIGTH/2;	
  
  for(i=0; i<5; i++)
  {        
    LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND);        
    LCD_DispString_EN(50,120,"Touch Calibrate...",BACKGROUND,BLUE,USE_FONT_24);	         
    LCD_DispChar_EN(150, 80, i+'1',BACKGROUND,RED,USE_FONT_24);
 
    /* �ʵ�����ʱ���б�Ҫ */        
    XPT2046_DelayUS(200000);    
    LCD_DrawCross(cal_value.lcd_x[i],cal_value.lcd_y[i]); //��ʾУ���õġ�ʮ����    
    XPT2046_ReadAdc_Smooth_XY(&cal_value.touch_x[i],&cal_value.touch_y[i],1);
  }
  
  if(perform_calibration(&cal_value)==1) 
  {
//		printf("Calibration failed.\n\r");
    return 1;
	}

  /* ��ԭʼ��������� ԭʼ�����������ת��ϵ���� */
  for(i=0; i<2; i++)
  {    
    int xtemp,ytemp,usGap_x,usGap_y;       
    xtemp=cal_value.touch_x[2*i];
    ytemp=cal_value.touch_y[2*i];
//    printf("before Calibration x=(%d)-> %d y=(%d)-> %d\n\r",cal_value.lcd_x[2*i],xtemp,cal_value.lcd_y[2*i],ytemp);
    
		usTest_x=(int)((cal_value.adjust[0]+cal_value.adjust[1]*xtemp+cal_value.adjust[2]*ytemp)/cal_value.adjust[6]);
		usTest_y=(int)((cal_value.adjust[3]+cal_value.adjust[4]*xtemp+cal_value.adjust[5]*ytemp)/cal_value.adjust[6]);
//	  printf("after Calibration x = %d y=%d\n\r",usTest_x,usTest_y);
    
    usGap_x=(usTest_x>cal_value.lcd_x[2*i])?(usTest_x-cal_value.lcd_x[2*i]):(cal_value.lcd_x[2*i]-usTest_x);   //ʵ��X�������������ľ��Բ�
    usGap_y=(usTest_y>cal_value.lcd_y[2*i])?(usTest_y-cal_value.lcd_y[2*i]):(cal_value.lcd_y[2*i]-usTest_y);   //ʵ��Y�������������ľ��Բ�
  
    if((usGap_x>10)||(usGap_y>10))
    {
      LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND);   
      LCD_DispString_EN(80,100,"Calibrate fail",BACKGROUND,RED,USE_FONT_24);
      LCD_DispString_EN(110,130,"try again",BACKGROUND,RED,USE_FONT_16);
      XPT2046_DelayUS(1000000);   
      return 1;   
    }      
  }
  
  cal_value.cal_flag = 0xAA55;
  SPI_FLASH_SectorErase(XPT2046_SPIFLASH_ADDR);
  SPI_FLASH_BufferWrite((uint8_t *)&cal_value,XPT2046_SPIFLASH_ADDR, sizeof(XPT2046_Calibration));
  
  LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND); 
  LCD_DispString_EN(50,100,"Calibrate Succed",BACKGROUND,BLUE,USE_FONT_24);
  XPT2046_DelayUS(1000000); 
  LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND); 
  return 0;    
}

/**
  * ��������: ��ȡ XPT2046 �����㣨У׼�󣩵�����
  * �������: pLCD_x��У׼��x������
  *           pLCD_y��У׼��y������
  * �� �� ֵ: ��
  * ˵    ������
  */
void XPT2046_Get_TouchedPoint(uint16_t *pLCD_x,uint16_t *pLCD_y)
{
  int xtemp,ytemp;
  
  if(XPT2046_ReadAdc_Smooth_XY(&xtemp,&ytemp,0)==0)
  {
    *pLCD_x=(uint16_t)((cal_value.adjust[0]+cal_value.adjust[1]*xtemp+cal_value.adjust[2]*ytemp)/cal_value.adjust[6]);
    *pLCD_y=(uint16_t)((cal_value.adjust[3]+cal_value.adjust[4]*xtemp+cal_value.adjust[5]*ytemp)/cal_value.adjust[6]);
  }
  else
  {
    *pLCD_x=0xFFFF;
    *pLCD_y=0xFFFF;
  }
  
} 

static void draw_one_point(uint16_t usX, uint16_t usY,uint16_t usColor)
{
  LCD_OpenWindow(usX,usY,1,1);
  LCD_WRITE_CMD(0x2C);
  LCD_WRITE_DATA(usColor);	
}

/**
  * ��������: ��LCDָ��λ�û�һ�����(����9��С��)
  * �������: x��x����
  *           y��y����
  *           usColor�������ɫ
  * �� �� ֵ: ��
  * ˵    ������
  */
void Palette_draw_point(uint16_t x, uint16_t y,uint16_t usColor)
{  
  draw_one_point(x-1, y-1,usColor);
  draw_one_point(  x, y-1,usColor);
  draw_one_point(x+1, y-1,usColor);
  draw_one_point(x-1,   y,usColor);  
  draw_one_point(  x,   y,usColor);  
  draw_one_point(x+1,   y,usColor);
  draw_one_point(x-1, y+1,usColor);  
  draw_one_point(  x, y+1,usColor);  
  draw_one_point(x+1, y+1,usColor);
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
