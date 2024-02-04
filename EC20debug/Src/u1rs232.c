#include "ec20.h"
// ����RS232���� PA9 PA10
unsigned char u1buf[100];
void u1Conf(void)
{
  HAL_UART_Receive_DMA(&huart1, u1buf, 100);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}
void u1rxitProcess(void)
{
  if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)) // �����ж�
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_UART_DMAStop(&huart1);
    HAL_UART_Receive_DMA(&huart1, u1buf, 100); // �ٴο���DMA����
  }
}

// ���͵�����Ϣ������
void SendTxDebug(unsigned char *p)
{
  unsigned short len = strlen((char const *)p);
  if (len < 100)
  {
    HAL_UART_Transmit(&huart1, p, len, 100);
  }
}