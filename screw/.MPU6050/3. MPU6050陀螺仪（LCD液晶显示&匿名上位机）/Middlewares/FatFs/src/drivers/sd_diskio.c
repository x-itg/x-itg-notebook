/**
  ******************************************************************************
  * �ļ�����: bsp_sdcard.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: SD����FatFS�ļ�ϵͳ�ŽӺ���ʵ��
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
#include <string.h>
#include "sdio/bsp_sdcard.h"
#include "ff_gen_drv.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define BLOCK_SIZE                512

/* ˽�б��� ------------------------------------------------------------------*/
static volatile DSTATUS Stat = STA_NOINIT;


/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);

#if _USE_WRITE == 1   // �������д����
  DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1   // ������������������
  DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

/* ����SD���ӿں��� */
const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,                   // SD����ʼ�� 
  SD_status,                       // SD��״̬��ȡ
  SD_read,                         // SD��������
#if  _USE_WRITE == 1
  SD_write,                        // SD��д����
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1
  SD_ioctl,                         // ��ȡSD����Ϣ
#endif /* _USE_IOCTL == 1 */
};

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: SD����ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  /* ��ʼ��SDIO���� */
  MX_SDIO_SD_Init();
  /* ��ȡSD��״̬ */
  if(HAL_SD_GetStatus(&hsdcard)==SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}

/**
  * ��������: SD��״̬��ȡ
  * �������: lun : ����
  * �� �� ֵ: DSTATUS��SD��״̬����ֵ
  * ˵    ��: ��
  */
DSTATUS SD_status(BYTE lun)
{
  Stat = STA_NOINIT;

  if(HAL_SD_GetStatus(&hsdcard) == SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}

/**
  * ��������: ��SD����ȡ���ݵ�������
  * �������: lun : ����
  *           buff����Ŷ�ȡ�����ݻ�����ָ��
  *           sector��������ַ(LBA)
  *           count��������Ŀ
  * �� �� ֵ: DSTATUS���������
  * ˵    ��: SD��������ʹ��DMA����
  */
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;
  if((DWORD)buff&3)
  {
    DWORD scratch[BLOCK_SIZE/4];
    while (count--) 
    {
      res = SD_read(lun,(void *)scratch, sector++, 1);
      if (res != RES_OK) 
      {
        break;
      }
      memcpy(buff, scratch, BLOCK_SIZE);
      buff += BLOCK_SIZE;
    }
    return res;
  }
  
  if(HAL_SD_ReadBlocks_DMA(&hsdcard,(uint32_t*)buff,(uint64_t)(sector * BLOCK_SIZE),BLOCK_SIZE,count) != SD_OK)
  {
    res = RES_ERROR;
  }
  if(res==RES_OK)
  {
    if(HAL_SD_CheckReadOperation(&hsdcard, 0xFFFFFFFF) != SD_OK)
    {
      res = RES_ERROR;
    }
  }
  return res;
}

/**
  * ��������: ������������д�뵽SD����
  * �������: lun : ����
  *           buff����Ŵ�д�����ݵĻ�����ָ��
  *           sector��������ַ(LBA)
  *           count��������Ŀ
  * �� �� ֵ: DSTATUS���������
  * ˵    ��: SD��д����û��ʹ��DMA����
  */
#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;
  if((DWORD)buff&3)
  {
    DRESULT res = RES_OK;
    DWORD scratch[BLOCK_SIZE / 4];

    while (count--) 
    {
      memcpy( scratch,buff,BLOCK_SIZE);
      res = SD_write(lun,(void *)scratch, sector++, 1);
      if (res != RES_OK) 
      {
        break;
      }					
      buff += BLOCK_SIZE;
    }
    return res;
  }	
      
  if(HAL_SD_WriteBlocks(&hsdcard,(uint32_t*)buff,(uint64_t)(sector * BLOCK_SIZE),BLOCK_SIZE, count) != SD_OK)
  {
    res = RES_ERROR;
  }  
  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * ��������: ����������Ʋ���(I/O control operation)
  * �������: lun : ����
  *           cmd����������
  *           buff����Ŵ�д����߶�ȡ���ݵĻ�����ָ��
  * �� �� ֵ: DSTATUS���������
  * ˵    ��: ��
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;
  
  /* ��ȡSD����������Ŀ(DWORD) */
  case GET_SECTOR_COUNT :
    HAL_SD_Get_CardInfo(&hsdcard,&SDCardInfo);
    *(DWORD*)buff = SDCardInfo.CardCapacity / BLOCK_SIZE;
    res = RES_OK;
    break;
  
  /* ��ȡ��д������С(WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = BLOCK_SIZE;
    res = RES_OK;
    break;
  
  /* ��ȡ�������С(DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = BLOCK_SIZE;
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */
  
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
