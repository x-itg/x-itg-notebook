#include "stm32f0xx_hal.h"
#include "control.h"
#include "ds18b20.h"
#include "TM1640.h"
 #include "stm32f0xx_hal_tim.h"
unsigned  int sec_time=0;
unsigned int sysms=0;
signed short ReadTemputer=0;
unsigned int willBeepTime=0;
unsigned short sTa=0;
unsigned short sTb=0;
unsigned short sTc=0;
unsigned short sTd=0;
unsigned short sTe=0;



#define DHT11_Dout_PORT                           GPIOB
#define DHT11_Dout_PIN                            GPIO_PIN_12
#define DHT11_Dout_LOW()                          HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_RESET) 
#define DHT11_Dout_HIGH()                         HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_SET)
#define DHT11_Data_IN()                           HAL_GPIO_ReadPin(DHT11_Dout_PORT,DHT11_Dout_PIN)



extern void Delay_us(unsigned short us);  


void DHT11_IO_IN(void){     //IO�ڷ�������Ϊ����
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void DHT11_IO_OUT(void){    //IO�ڷ�������Ϊ���
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}

static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_ReadByte(void);

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
typedef struct
{
  uint8_t  humi_high8bit;               
  uint8_t  humi_low8bit;                 
  uint8_t  temp_high8bit;               
  uint8_t  temp_low8bit;              
  uint8_t  check_sum;                   
  double    humidity;         
  double    temperature;      
} DHT11_Data_TypeDef;


DHT11_Data_TypeDef  dhdata;
/**
  * ��������: DHT11 ��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void DHT11_Init ( void )
{

  DHT11_Mode_Out_PP();
        
  DHT11_Dout_HIGH();  // ����GPIO
}

/**
  * ��������: ʹDHT11-DATA���ű�Ϊ��������ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
        
}

extern TIM_HandleTypeDef htim1;

static void DHT11_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);      
}
#if 0
void Delay_us(unsigned short us)  
{    
  uint16_t differ = 0xffff-us-5;  

  htim1.Instance->CNT =differ;
    __HAL_TIM_SET_COUNTER(&htim1,differ);   //�趨TIM7��������ʼֵ
    HAL_TIM_Base_Start(&htim1);     //������ʱ�� 

    while(differ < 0xffff-5){   //�ж�
        differ = __HAL_TIM_GET_COUNTER(&htim1);     //��ѯ�������ļ���ֵ
    }
    HAL_TIM_Base_Stop(&htim1);
} 
#else
void Delay_us(uint16_t time)
{
  uint8_t i;
  
  while(time)
  {    
    for (i = 0; i < 1; i++)
    {
      
    }
    time--;
  }
}
#endif
void Delay_ms(unsigned int ms)
{
  unsigned int i=0;
  for(i=0;i<ms;i++)
  {
    Delay_us(1000);
  }
}
/**
  * ��������: ��DHT11��ȡһ���ֽڣ�MSB����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static uint8_t DHT11_ReadByte ( void )
{
    uint8_t i, temp=0;
    uint16_t count;
    for(i=0;i<8;i++)    
    {         
        /*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
        while(DHT11_Data_IN()==GPIO_PIN_RESET)
        {
          count++;
          if(count>1000)  
            return 0;
          Delay_us(1); 
        }

        /*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
         *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
         */
        Delay_us(50); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��                     
        count = 0;
        if(DHT11_Data_IN()==GPIO_PIN_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
        {
              /* �ȴ�����1�ĸߵ�ƽ���� */
              while(DHT11_Data_IN()==GPIO_PIN_SET)
              {
                count++;
                if(count>1000)  
                  return 0;
                Delay_us(1); 
              }
              count = 0;
              temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
        }
        else         // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
        {                           
           temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
        }
    }
    return temp;
}

/**
  * ��������: һ�����������ݴ���Ϊ40bit����λ�ȳ�
  * �������: DHT11_Data:DHT11��������
  * �� �� ֵ: ERROR��  ��ȡ����
  *           SUCCESS����ȡ�ɹ�
  * ˵    ����8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
  */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
  uint8_t temp;
  uint16_t humi_temp;
  uint16_t count;

  /*���ģʽ*/
  DHT11_Mode_Out_PP();
  /*��������*/
  DHT11_Dout_LOW();
  /*��ʱ18ms*/
  Delay_ms(18);
  
  /*�������� ������ʱ30us*/
  DHT11_Dout_HIGH(); 

  Delay_us(30);   //��ʱ30us

  /*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
  DHT11_Mode_IPU();

//  /*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
  if(DHT11_Data_IN()==GPIO_PIN_RESET)
  {
    count=0;
    /*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
    while(DHT11_Data_IN()==GPIO_PIN_RESET)
    {
      count++;
      if(count>100)  
        return 0;
      Delay_us(1); 
    }
    count=0;
    /*��ѯֱ���ӻ������� 80us �ߵ�ƽ�����źŽ���*/
    while(DHT11_Data_IN()==GPIO_PIN_SET)
    {
      count++;
      if(count>100)  
        return 0;
      Delay_us(1); 
    }
    
    /*��ʼ��������*/   
    DHT11_Data->humi_high8bit= DHT11_ReadByte();
    DHT11_Data->humi_low8bit = DHT11_ReadByte();
    DHT11_Data->temp_high8bit= DHT11_ReadByte();
    DHT11_Data->temp_low8bit = DHT11_ReadByte();
    DHT11_Data->check_sum    = DHT11_ReadByte();

    /* �����ݽ��д��� */
    humi_temp=DHT11_Data->humi_high8bit*100+DHT11_Data->humi_low8bit;
    DHT11_Data->humidity =(double)humi_temp/100;
    
    humi_temp=DHT11_Data->temp_high8bit*100+DHT11_Data->temp_low8bit;
    DHT11_Data->temperature=(double)humi_temp/100;    
    
    /*����ȡ�������Ƿ���ȷ*/
    temp = DHT11_Data->humi_high8bit + DHT11_Data->humi_low8bit + 
           DHT11_Data->temp_high8bit+ DHT11_Data->temp_low8bit;
    
    if(DHT11_Data->check_sum==temp)
    { 
      return SUCCESS;
    }
    else 
      return ERROR;
  }        
  else
    return ERROR;
}




void ReadDh(void)
{
  DHT11_Read_TempAndHumidity(&dhdata);
}

void ReadKey(void)
{

  uint16_t temperature;
  uint16_t humidity;
  
  if(sec_time>2000)
  {
    sec_time=0;
    extern void stopADC(void);
    extern void startADC(void);
    stopADC();
    Th=(signed short)(dhdata.humidity*10);
    ReadDh();
    startADC();
   // DHT11_Read_Data(&temperature,&humidity);

  }
  if(Key1Read)
  {
    //JDQ1_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ1_L;
    
  }
  
  if(Key2Read)
  {
    //JDQ2_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ2_L;
  }
  
  if(Key3Read)
  {
    //JDQ3_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ3_L;
  }
  
  if(Key4Read)
  {
    //JDQ4_H;
    //willBeepTime=10;
  }
  else 
  {
    //JDQ4_L;
  }
  
  if(willBeepTime!=0)
  {
    BeepOn;
  }
  if(willBeepTime==0)
  {
    BeepOf;
  }
  UpateShData();
  
  if(sysms>10)
  { 
    sysms=0;
    sTa=(unsigned short)((float)Ta*0.5115);
    sTb=(unsigned short)((float)Tb*0.5115);
    sTc=(unsigned short)((float)Tc*0.5115);
    sTd=(unsigned short)((float)Td*0.5115);
    sTe=(unsigned short)((float)Te*0.5115);
    if(Ta<=0)sTa=0;
    if(Tb<=0)sTb=0;
    if(Tc<=0)sTc=0;
    if(Td<=0)sTd=0;
    if(Te<=0)sTe=0;
    if(sTa>1023)sTa=1023;
    if(sTb>1023)sTb=1023;
    if(sTc>1023)sTc=1023;
    if(sTd>1023)sTd=1023;
    if(sTe>1023)sTe=1023;
#if TeStFlag==1
    sTa=1023;
    sTb=1023;
    sTc=1023;
    sTd=1023;
    sTe=1023;
#endif
    write_data_to_TLC5615(0,sTa);
    write_data_to_TLC5615(1,sTb);
    write_data_to_TLC5615(2,sTc);
    write_data_to_TLC5615(3,sTd);
    write_data_to_TLC5615(4,sTe);
    //ReadTemputer=DS18B20_GetTemperature();
  }
}

void JDQ_Hx(unsigned char x)
{
  hcreg=hcreg|(0x0001<<x);
  LED595SendData(hcreg);
}

void JDQ_Lx(unsigned char x)
{
  hcreg=hcreg&(~(0x0001<<x));
  LED595SendData(hcreg);
}

extern void delay_us(unsigned short T_Dly);
void write_data_to_TLC5615(unsigned char n,unsigned short data)
{
    //data=560;
    u8 i = 0;     //һ������ 10λ ����λ
    CLK0; //cs��Ҫ�� clkΪ��ʱ��ʹ��

    JDQ_Lx(n+10);;//csΪ�ͣ��������ݲ��ܱ�������λ�Ĵ���
    delay_us(1000);
    for(i = 0; i < 10; i++) //һ��ʮλ ��Ч����λ
    {
        if(data & 0x200) //�������� ��λ����ʮλ
        {
            MOSI1; //����bit = 1    
        }    
        else
        {
            MOSI0; //����bit = 0    
        }

        CLK1; //ʱ�� �� 0 ~ 1 �������ݴ���

        CLK0; //ʱ�ӻָ� 0

        data <<= 1;    //��������
    }

    //���� ����λ ���λ
    MOSI0;
    CLK1; //ʱ�� �� 0 ~ 1 �������ݴ���

    CLK0; //ʱ�ӻָ� 0

    CLK1; //ʱ�� �� 0 ~ 1 �������ݴ���

    CLK0; //ʱ�ӻָ� 0

    JDQ_Hx(n+10); //�ָ� cs �� 1

}

/*
void write_data_to_TLC5615(unsigned char n,unsigned short data)
{
  unsigned char  i;
  
  CLK0;	
  JDQ_Lx(n+10);
  for(i=12;i>0;i--)	//�Ӹ�λ��ʼ������ȡ��12λ����
  {
    if(data&0x0200)
      MOSI1;
    else
      MOSI0;

    CLK1;	//��ʱ�����������ν������ͳ�
    data<<=1;			
    CLK0;	//���͵ȴ�
  }
  JDQ_Hx(n+10);
  CLK0;
}
*/