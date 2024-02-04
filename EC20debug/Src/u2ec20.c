#include"ec20.h"

//���ڳ�ʼ��EC20����uart2  PA2��PA3
void u2Conf(void)
{
  HAL_UART_Receive_DMA(&huart2, RxBufferDMA, DMARXBUFFERSIZE);  
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); 
}

//��������Ŵ��ڽ��տ����ж���
void u2rxitProcess(void)
{
  if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!=RESET))//�����ж�
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    HAL_UART_DMAStop(&huart2);
    DMARxLenU2=DMARXBUFFERSIZE-huart2.hdmarx->Instance->CNDTR;//NDTR;//���յ����ֽڳ���
    HAL_UART_DMAStop(&huart2);
    rsPackFlag=1;//���ڽ���
    rsRxTime=0;
    unsigned short i=rsRxIndexLen;//�����ռ�
    for(i=rsRxIndexLen;i<rsRxIndexLen+DMARxLenU2;i++)
    {
      rsRxBuf[i]=RxBufferDMA[i-rsRxIndexLen];
    }
    rsRxIndexLen=rsRxIndexLen+DMARxLenU2;//����
  }
  
}


//EC20�ַ�������
void SendTxBuf( unsigned char *p)
{
  unsigned short len=strlen((char const *)p);
  HAL_UART_Transmit(&huart2,p,len,100);
}