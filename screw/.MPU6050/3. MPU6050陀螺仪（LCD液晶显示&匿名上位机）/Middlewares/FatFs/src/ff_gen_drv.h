#ifndef __FF_GEN_DRV_H__
#define __FF_GEN_DRV_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "diskio.h"
#include "ff.h"

/* ���Ͷ��� ------------------------------------------------------------------*/

/* �洢�豸������������������Ͷ��� */
typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                           /* ��ʼ���洢�豸����ָ��         */
  DSTATUS (*disk_status)     (BYTE);                           /* ��ȡ�洢�豸״̬����ָ��       */
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);       /* �Ӵ洢�豸��ȡ�������ݺ���ָ�� */
#if _USE_WRITE == 1                                            /* ��_USE_WRITE=1ʱ����Ч         */
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT); /* д���ݵ��洢�豸�����ں���ָ�� */
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1                                            /* ��_USE_IOCTL=1ʱ����Ч         */
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);              /* ����������Ʋ�������ָ��       */
#endif /* _USE_IOCTL == 1 */
}Diskio_drvTypeDef;

/* �洢�豸���Ͷ��� */
typedef struct
{ 
  uint8_t                 is_initialized[_VOLUMES];            /* �洢�豸��ʼ��״̬       */ 
  Diskio_drvTypeDef       *drv[_VOLUMES];                      /* �洢�豸��������������� */
  uint8_t                 lun[_VOLUMES];                       /* USB�豸�߼�������� */
  __IO uint8_t            nbr;                                 /* �洢�豸��FatFS���豸��� */
}Disk_drvTypeDef;

/* �궨�� --------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
uint8_t FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, uint8_t lun);
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
uint8_t FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, BYTE lun);
uint8_t FATFS_UnLinkDriverEx(char *path, BYTE lun);
uint8_t FATFS_GetAttachedDriversNbr(void);

#endif  /* __FF_GEN_DRV_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
