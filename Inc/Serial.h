#ifndef __SERIAL_H__
#define __SERIAL_H__

#define MAX_RX_LEN (256U) // һ���Կ��Խ��ܵ������ֽڳ��ȣ�������Լ����塣U��Unsigned����˼��
#define MAX_TX_LEN (512U) // һ���Կ��Է��͵������ֽڳ��ȣ�������Լ����塣
#include "stdio.h"
#include "string.h"
#include <stdarg.h>

extern uint8_t *RxData;
extern uint8_t *p_IsToReceive;
extern uint8_t WhichBufIsReady;
extern char RxDataStr[MAX_RX_LEN];


void USART1_Init(void);
void DMA_USART1_Tx_Data(uint8_t *buffer, uint16_t size);//���鷢�ʹ�������
void Myprintf(char *format, ...);//����printf���ʹ�������
void USART1_TX_Wait(void);//���͵ȴ�����
uint8_t GetRxFlag(void);
void StringTrans(void);
float GetRxValue(char *Str, uint8_t lenth);
#endif


