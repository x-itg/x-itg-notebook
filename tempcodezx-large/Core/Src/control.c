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


void DHT11_IO_IN(void){     //IO口方向设置为输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void DHT11_IO_OUT(void){    //IO口方向设置为输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}

static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_ReadByte(void);

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: DHT11 初始化函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void DHT11_Init ( void )
{

  DHT11_Mode_Out_PP();
        
  DHT11_Dout_HIGH();  // 拉高GPIO
}

/**
  * 函数功能: 使DHT11-DATA引脚变为上拉输入模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
    __HAL_TIM_SET_COUNTER(&htim1,differ);   //设定TIM7计数器起始值
    HAL_TIM_Base_Start(&htim1);     //启动定时器 

    while(differ < 0xffff-5){   //判断
        differ = __HAL_TIM_GET_COUNTER(&htim1);     //查询计数器的计数值
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
  * 函数功能: 从DHT11读取一个字节，MSB先行
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static uint8_t DHT11_ReadByte ( void )
{
    uint8_t i, temp=0;
    uint16_t count;
    for(i=0;i<8;i++)    
    {         
        /*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
        while(DHT11_Data_IN()==GPIO_PIN_RESET)
        {
          count++;
          if(count>1000)  
            return 0;
          Delay_us(1); 
        }

        /*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
         *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
         */
        Delay_us(50); //延时x us 这个延时需要大于数据0持续的时间即可                     
        count = 0;
        if(DHT11_Data_IN()==GPIO_PIN_SET)/* x us后仍为高电平表示数据“1” */
        {
              /* 等待数据1的高电平结束 */
              while(DHT11_Data_IN()==GPIO_PIN_SET)
              {
                count++;
                if(count>1000)  
                  return 0;
                Delay_us(1); 
              }
              count = 0;
              temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
        }
        else         // x us后为低电平表示数据“0”
        {                           
           temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
        }
    }
    return temp;
}

/**
  * 函数功能: 一次完整的数据传输为40bit，高位先出
  * 输入参数: DHT11_Data:DHT11数据类型
  * 返 回 值: ERROR：  读取出错
  *           SUCCESS：读取成功
  * 说    明：8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
  */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
  uint8_t temp;
  uint16_t humi_temp;
  uint16_t count;

  /*输出模式*/
  DHT11_Mode_Out_PP();
  /*主机拉低*/
  DHT11_Dout_LOW();
  /*延时18ms*/
  Delay_ms(18);
  
  /*总线拉高 主机延时30us*/
  DHT11_Dout_HIGH(); 

  Delay_us(30);   //延时30us

  /*主机设为输入 判断从机响应信号*/ 
  DHT11_Mode_IPU();

//  /*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
  if(DHT11_Data_IN()==GPIO_PIN_RESET)
  {
    count=0;
    /*轮询直到从机发出 的80us 低电平 响应信号结束*/  
    while(DHT11_Data_IN()==GPIO_PIN_RESET)
    {
      count++;
      if(count>100)  
        return 0;
      Delay_us(1); 
    }
    count=0;
    /*轮询直到从机发出的 80us 高电平标置信号结束*/
    while(DHT11_Data_IN()==GPIO_PIN_SET)
    {
      count++;
      if(count>100)  
        return 0;
      Delay_us(1); 
    }
    
    /*开始接收数据*/   
    DHT11_Data->humi_high8bit= DHT11_ReadByte();
    DHT11_Data->humi_low8bit = DHT11_ReadByte();
    DHT11_Data->temp_high8bit= DHT11_ReadByte();
    DHT11_Data->temp_low8bit = DHT11_ReadByte();
    DHT11_Data->check_sum    = DHT11_ReadByte();

    /* 对数据进行处理 */
    humi_temp=DHT11_Data->humi_high8bit*100+DHT11_Data->humi_low8bit;
    DHT11_Data->humidity =(double)humi_temp/100;
    
    humi_temp=DHT11_Data->temp_high8bit*100+DHT11_Data->temp_low8bit;
    DHT11_Data->temperature=(double)humi_temp/100;    
    
    /*检查读取的数据是否正确*/
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
    u8 i = 0;     //一共发送 10位 数据位
    CLK0; //cs需要在 clk为低时，使能

    JDQ_Lx(n+10);;//cs为低，串行数据才能被移入移位寄存器
    delay_us(1000);
    for(i = 0; i < 10; i++) //一共十位 有效数据位
    {
        if(data & 0x200) //发送数据 高位，第十位
        {
            MOSI1; //数据bit = 1    
        }    
        else
        {
            MOSI0; //数据bit = 0    
        }

        CLK1; //时钟 从 0 ~ 1 发送数据传递

        CLK0; //时钟恢复 0

        data <<= 1;    //数据左移
    }

    //传输 低两位 填充位
    MOSI0;
    CLK1; //时钟 从 0 ~ 1 发送数据传递

    CLK0; //时钟恢复 0

    CLK1; //时钟 从 0 ~ 1 发送数据传递

    CLK0; //时钟恢复 0

    JDQ_Hx(n+10); //恢复 cs 高 1

}

/*
void write_data_to_TLC5615(unsigned char n,unsigned short data)
{
  unsigned char  i;
  
  CLK0;	
  JDQ_Lx(n+10);
  for(i=12;i>0;i--)	//从高位开始，连续取出12位数据
  {
    if(data&0x0200)
      MOSI1;
    else
      MOSI0;

    CLK1;	//在时钟上升沿依次将数据送出
    data<<=1;			
    CLK0;	//拉低等待
  }
  JDQ_Hx(n+10);
  CLK0;
}
*/