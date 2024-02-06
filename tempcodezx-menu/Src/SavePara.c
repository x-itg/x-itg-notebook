#include "SavePara.h"


//����Type��Ϣ����Flash
unsigned short Type=0x1234;
unsigned short Look=0;
unsigned short SaveCalaCoefficient[16][6];
unsigned short MenuCoefficient[6];

SAVEPARA SavePara;//��Ҫ���������
MAINPARA MainPara;//����ɼ���������
void LoadSavePara(void)
{
  unsigned int PageAddress   =SAVEPAGEADDR;
  unsigned int PageAddressCoe=SAVEPAGEADDRCOE;
  unsigned char i=0;
  unsigned char j=0;
  unsigned char k=0;
  MenuCoefficient[0]=VcoeZ;
  MenuCoefficient[1]=VcoeZ;
  MenuCoefficient[2]=VcoeZ;
  MenuCoefficient[3]=IcoeZ;
  MenuCoefficient[4]=IcoeZ;
  MenuCoefficient[5]=IcoeZ;
  for(i=0;i<16;i++)//0~15��ϵ�����֧��16���Ӱ�
  {
    for(j=0;j<6;j++)
    {
      SaveCalaCoefficient[i][j]=(*(unsigned short *)(PageAddressCoe + k*2)); 
      
      if(j<3)//��ѹϵ��
      {
        if(SaveCalaCoefficient[i][j]>VcoeMax||SaveCalaCoefficient[i][j]<VcoeMin)
        {
          SaveCalaCoefficient[i][j]=VcoeZ;
        }
      }else//����ϵ��
      {
        if(SaveCalaCoefficient[i][j]>IcoeMax||SaveCalaCoefficient[i][j]<IcoeMax)
        {
          SaveCalaCoefficient[i][j]=IcoeZ;
        }
      }
      k++;
    }
  }
  SavePara.Awarm = (*(unsigned short *)(PageAddress + 0*2)); 
  SavePara.Bwarm = (*(unsigned short *)(PageAddress + 1*2)); 
  SavePara.Cwarm = (*(unsigned short *)(PageAddress + 2*2));
  SavePara.DcWarm= (*(unsigned short *)(PageAddress + 3*2));
  SavePara.DacOut =(*(unsigned short *)(PageAddress + 4*2));///////
  SavePara.DacVal= (*(unsigned short *)(PageAddress + 5*2)); 
  SavePara.DcHig = (*(unsigned short *)(PageAddress + 6*2)); 
  SavePara.DcLow = (*(unsigned short *)(PageAddress + 7*2)); 
  SavePara.DcRig = (*(unsigned short *)(PageAddress + 8*2));
  SavePara.AcHig = (*(unsigned short *)(PageAddress + 9*2)); 
  SavePara.AcLow = (*(unsigned short *)(PageAddress + 10*2));
  SavePara.Bdrate= (*(unsigned short *)(PageAddress + 11*2)); 
  SavePara.Addr  = (*(unsigned short *)(PageAddress + 12*2));
  if(SavePara.Awarm>WarnOnOffMax )SavePara.Awarm=WarnOnOffZ;
  if(SavePara.Bwarm>WarnOnOffMax )SavePara.Bwarm=WarnOnOffZ;
  if(SavePara.Cwarm>WarnOnOffMax )SavePara.Cwarm=WarnOnOffZ;
  if(SavePara.DcWarm>WarnOnOffMax)SavePara.DcWarm=WarnOnOffZ;
  if(SavePara.DacOut>WarnOnOffMax)SavePara.DacOut=WarnOnOffZ;
  if(SavePara.DacVal>DacValMax   )SavePara.DacVal=DacValZ;
  if(SavePara.DcHig>DcHigMax     ||SavePara.DcHig<DcHigMin)SavePara.DcHig=DcHigZ;
  if(SavePara.DcLow>DcLowMax     ||SavePara.DcLow<DcLowMin)SavePara.DcLow=DcLowZ;
  if(SavePara.DcRig>DcRigMax     ||SavePara.DcRig<DcRigMin)SavePara.DcRig=DcRigZ;
  if(SavePara.AcHig>AcHigMax     ||SavePara.AcHig<AcHigMin)SavePara.AcHig=AcHigZ;
  if(SavePara.AcLow>AcLowMax     ||SavePara.AcLow<AcLowMin)SavePara.AcLow=AcLowZ;
  if(SavePara.Bdrate>BdrateMax)SavePara.Bdrate=BdrateZ;
  if(SavePara.Addr>AddrMax    )SavePara.Addr=AddrZ;

    
}
extern void MX_USART1_UART_InitOwn(unsigned char wBrate);
unsigned char FlashUpdateBrate(void)
{
  FLASH_EraseInitTypeDef page;
  unsigned int page_error=0;
  page.NbPages=1;
  page.PageAddress=SAVEPAGEADDR;
  page.TypeErase=FLASH_TYPEERASE_PAGES;
  
  MX_USART1_UART_InitOwn(SavePara.Addr);

  HAL_FLASH_Unlock();//����flash���ƿ�
  
  HAL_FLASHEx_Erase(&page,&page_error);//��ҳɾ�� д��ǰ���ɾ�� ��ɾ�����ܵ�������д�벻��
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+0*2),SavePara.Awarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+1*2),SavePara.Bwarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+2*2),SavePara.Cwarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+3*2),SavePara.DcWarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+4*2),SavePara.DacOut);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+5*2),SavePara.DacVal);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+6*2),SavePara.DcHig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+7*2),SavePara.DcLow);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+8*2),SavePara.DcRig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+9*2),SavePara.AcHig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+10*2),SavePara.AcLow);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+11*2),SavePara.Bdrate);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+12*2),SavePara.Addr);

  HAL_FLASH_Lock();//��flash����
  return 1;
  
  
}
unsigned char FlashUpdate(void)
{
  FLASH_EraseInitTypeDef page;
  unsigned int page_error=0;
  page.NbPages=1;
  page.PageAddress=SAVEPAGEADDR;
  page.TypeErase=FLASH_TYPEERASE_PAGES;
  
  

  HAL_FLASH_Unlock();//����flash���ƿ�
  
  HAL_FLASHEx_Erase(&page,&page_error);//��ҳɾ�� д��ǰ���ɾ�� ��ɾ�����ܵ�������д�벻��
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+0*2),SavePara.Awarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+1*2),SavePara.Bwarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+2*2),SavePara.Cwarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+3*2),SavePara.DcWarm);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+4*2),SavePara.DacOut);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+5*2),SavePara.DacVal);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+6*2),SavePara.DcHig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+7*2),SavePara.DcLow);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+8*2),SavePara.DcRig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+9*2),SavePara.AcHig);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+10*2),SavePara.AcLow);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+11*2),SavePara.Bdrate);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+12*2),SavePara.Addr);

  HAL_FLASH_Lock();//��flash����
  return 1;
  
  
}

extern unsigned  char iMenuNum;//Ҫ��ʾ�ĵ�ַ

unsigned char FlashUpdateCoe(void)
{
  unsigned char i=0;
  unsigned char j=0;
  unsigned char k=0;
  FLASH_EraseInitTypeDef page;
  unsigned int page_error=0;
  page.NbPages=1;
  page.PageAddress=SAVEPAGEADDRCOE;
  page.TypeErase=FLASH_TYPEERASE_PAGES;
  
  SaveCalaCoefficient[iMenuNum][0]=MenuCoefficient[0];
  SaveCalaCoefficient[iMenuNum][1]=MenuCoefficient[1];
  SaveCalaCoefficient[iMenuNum][2]=MenuCoefficient[2];
  SaveCalaCoefficient[iMenuNum][3]=MenuCoefficient[3];
  SaveCalaCoefficient[iMenuNum][4]=MenuCoefficient[4];
  SaveCalaCoefficient[iMenuNum][5]=MenuCoefficient[5];

  HAL_FLASH_Unlock();//����flash���ƿ�
  
  HAL_FLASHEx_Erase(&page,&page_error);//��ҳɾ�� д��ǰ���ɾ�� ��ɾ�����ܵ�������д�벻��
  
  for(i=0;i<16;i++)//0~15��ϵ�����֧��16���Ӱ�
  {
    for(j=0;j<6;j++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(page.PageAddress+k*2),SaveCalaCoefficient[i][j]);
      k++;
    }
  }
  
  HAL_FLASH_Lock();//��flash����
  
  return 1;
  
  
}
