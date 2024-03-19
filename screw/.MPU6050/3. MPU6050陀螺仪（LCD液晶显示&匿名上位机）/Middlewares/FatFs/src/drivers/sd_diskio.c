/**
  ******************************************************************************
  * 文件名程: bsp_sdcard.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: SD卡与FatFS文件系统桥接函数实现
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
#include <string.h>
#include "sdio/bsp_sdcard.h"
#include "ff_gen_drv.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define BLOCK_SIZE                512

/* 私有变量 ------------------------------------------------------------------*/
static volatile DSTATUS Stat = STA_NOINIT;


/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);

#if _USE_WRITE == 1   // 如果允许写操作
  DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1   // 如果输入输出操作控制
  DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

/* 定义SD卡接口函数 */
const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,                   // SD卡初始化 
  SD_status,                       // SD卡状态获取
  SD_read,                         // SD卡读数据
#if  _USE_WRITE == 1
  SD_write,                        // SD卡写数据
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1
  SD_ioctl,                         // 获取SD卡信息
#endif /* _USE_IOCTL == 1 */
};

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: SD卡初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  /* 初始化SDIO外设 */
  MX_SDIO_SD_Init();
  /* 获取SD卡状态 */
  if(HAL_SD_GetStatus(&hsdcard)==SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}

/**
  * 函数功能: SD卡状态获取
  * 输入参数: lun : 不用
  * 返 回 值: DSTATUS：SD卡状态返回值
  * 说    明: 无
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
  * 函数功能: 从SD卡读取数据到缓冲区
  * 输入参数: lun : 不用
  *           buff：存放读取到数据缓冲区指针
  *           sector：扇区地址(LBA)
  *           count：扇区数目
  * 返 回 值: DSTATUS：操作结果
  * 说    明: SD卡读操作使用DMA传输
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
  * 函数功能: 将缓冲区数据写入到SD卡内
  * 输入参数: lun : 不用
  *           buff：存放待写入数据的缓冲区指针
  *           sector：扇区地址(LBA)
  *           count：扇区数目
  * 返 回 值: DSTATUS：操作结果
  * 说    明: SD卡写操作没有使用DMA传输
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
  * 函数功能: 输入输出控制操作(I/O control operation)
  * 输入参数: lun : 不用
  *           cmd：控制命令
  *           buff：存放待写入或者读取数据的缓冲区指针
  * 返 回 值: DSTATUS：操作结果
  * 说    明: 无
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
  
  /* 获取SD卡总扇区数目(DWORD) */
  case GET_SECTOR_COUNT :
    HAL_SD_Get_CardInfo(&hsdcard,&SDCardInfo);
    *(DWORD*)buff = SDCardInfo.CardCapacity / BLOCK_SIZE;
    res = RES_OK;
    break;
  
  /* 获取读写扇区大小(WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = BLOCK_SIZE;
    res = RES_OK;
    break;
  
  /* 获取擦除块大小(DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = BLOCK_SIZE;
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */
  
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
