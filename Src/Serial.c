/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                              
  *                     By(JCML)
    *
  *    
  * FileName   : Serial.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-01-17         
  * Description: ����Hal��Ĵ����շ����庯��  
					PS��ʹ�øú���ǰ�뵽stm32f1xx_it.h�н�
									void USART1_IRQHandler(void)
									void DMA1_Channel5_IRQHandler(void)
							��������ע�͵�
��"usart.h"�����
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

  * May Be Used Function List:  
1. Head file:
#include ".h" 

2. Function File:


******************************************************************************
 */
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "Serial.h"
#include "stm32f1xx_it.h"


uint8_t USART1_TX_BUF[MAX_TX_LEN];   // my_printf�ķ��ͻ��壬�������������á�
volatile uint8_t USART1_TX_FLAG = 0; // USART���ͱ�־����������ʱ��1����volatile���������Ż�
uint8_t u1rxbuf[MAX_RX_LEN];         // ���ݽ��ջ���1
uint8_t u2rxbuf[MAX_RX_LEN];         // ���ݽ��ջ���2
uint8_t WhichBufIsReady = 0;         // ˫����ָʾ����
// 0:u1rxbuf ��DMAռ�ý���,  u2rxbuf ���Զ�ȡ.
// 0:u2rxbuf ��DMAռ�ý���,  u1rxbuf ���Զ�ȡ.
uint8_t *RxData = u2rxbuf;        // ָ�롪��ָ����Զ�ȡ���Ǹ�����
uint8_t *p_IsToReceive = u1rxbuf; // ָ�롪��ָ��ռ�õ��Ǹ�����
char RxDataStr[MAX_RX_LEN];
//ע�ⶨ���ʱ��Ҫ����������ָ�밴��WhichBufIsReady�ĳ�ʼ״̬�ȳ�ʼ��һ�¡�
uint8_t Rxflag;
uint32_t data_length;


/***********************************************************
*@fuction	:USART1_Init
*@brief		:���ڳ�ʼ��
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void USART1_Init(void)
{
  // ��������1�����ж�
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

  // ����DMA����ͨ���ķ�������жϣ�����ʵ�ַ�װ���ͺ�������ĵȴ�����
  __HAL_DMA_ENABLE_IT(&hdma_usart1_tx, DMA_IT_TC);

  // ������б�־λ����ֹ�ж�����
  __HAL_UART_CLEAR_IDLEFLAG(&huart1);

  // ������Ҫ��DMA����
  // ��ȻDMAû����ǰ׼������һ�ν��յ������Ƕ�ȡ��������
  HAL_UART_Receive_DMA(&huart1, p_IsToReceive, MAX_RX_LEN);
}

/***********************************************************
*@fuction	:StringTrans
*@brief		:���ַ�����ת��Ϊ�ַ���Ҳ���ڸú��������if���������ض���ͨ�ŷ�ʽ
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void StringTrans(void)
{
	uint32_t i=0;
	while (i < data_length)
	{
		if (RxData[i] == '#' ||RxData[i] == 0)
		{
			break;
		}
		else {RxDataStr[i]=RxData[i];}
		i++;
	}
	//������������·�ʽ
//	for (i=0; i < data_length; i++)//
//	{
//		if (u1rxbuf[i] == '\r' ||u1rxbuf[i] == '\n')
//		RxDataStr[i]=u1rxbuf[i];
//	}
	RxDataStr[data_length]='\0';
}



/***********************************************************
*@���� 	:GetRxValue
*@����	:ת����ȡ�Ӵ����н������� ���Ϊfloat��
*@����	:Strָ��Ŀ���ַ�����ָ�룬 lenth�ַ����ĳ��ȣ������ַ������ȼ���
*@����ֵ	:��ȡ����ֵ
*@����	:JCML
*@����	:2023-04-20
***********************************************************/

float GetRxValue(char *Str, uint8_t lenth)
{
	uint8_t i, j;
	for (i = 0; i<lenth; i++)
	{//�ж������ָ��Ż�������ʱ��RxDataStr�е�ֵǰ��
		if ((Str[i]=='-'&&Str[i+1]<='9'&&Str[i+1]>='0')||
															(Str[i]<='9'&&Str[i]>='0'))
		{
			for(j=0;Str[j]!='\0';j++)
			{
				Str[j] = Str[j+i];
			}
			Str[j]='\0';
			break;
		}
	}
	return atof(Str);//
}


/***********************************************************
*@fuction	:DMA_USART1_Tx_Data
*@brief		:
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void DMA_USART1_Tx_Data(uint8_t *buffer, uint16_t size)
{
  USART1_TX_Wait();                             // �ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�
  USART1_TX_FLAG = 1;                           // USART1���ͱ�־���������ͣ�
  HAL_UART_Transmit_DMA(&huart1, buffer, size); // ����ָ�����ȵ�����
}
/***********************************************************
*@fuction	:Myprintf
*@brief		:printf�ض���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void Myprintf(char *format, ...)
{
  //VA_LIST ����C�����н����������һ��꣬
  //����ͷ�ļ���#include <stdarg.h>�����ڻ�ȡ��ȷ�������Ĳ�����
  va_list arg_ptr;//ʵ�����ɱ䳤�����б�

  USART1_TX_Wait(); //�ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�

  va_start(arg_ptr, format);//��ʼ���ɱ�����б�����formatΪ�ɱ䳤�б����ʼ�㣨��һ��Ԫ�أ�

  // MAX_TX_LEN+1�ɽ��ܵ�����ַ���(���ֽ�����UNICODEһ���ַ������ֽ�), ��ֹ��������Խ��
  vsnprintf((char *)USART1_TX_BUF, MAX_TX_LEN + 1, format, arg_ptr);
  //��USART1_TX_BUF���׵�ַ��ʼƴ�ϣ�ƴ��format���ݣ�MAX_TX_LEN+1���Ƴ��ȣ���ֹ��������Խ��

  va_end(arg_ptr); //ע�����ر�

  DMA_USART1_Tx_Data(USART1_TX_BUF, strlen((const char *)USART1_TX_BUF)); 
  // �ǵð�buf����Ķ�����HAL����ȥ 
}

void USART1_TX_Wait(void)
{
  uint16_t delay = 20000;
  while (USART1_TX_FLAG)
  {
    delay--;
    if (delay == 0)
      return;
  }
}

/***********************************************************
*@fuction	:GetRxFlag
*@brief		:���ؽ������ݵı�־ͬʱ����ת��Ϊ�ַ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

uint8_t GetRxFlag(void)
{
	if (Rxflag==1){StringTrans();Rxflag=0;return 1;}
	else {return 0;}
}


/***********************************************************
*@fuction	:DMA1_Channel5_IRQHandler
*@brief		:DMA�жϴ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */
  if (__HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4) != RESET) //���ݷ�������ж�
  {
    // __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4);
    // ��һ������ʵ�� HAL_DMA_IRQHandler(&hdma_usart1_tx) Ҳ����ˡ�

    __HAL_UART_CLEAR_IDLEFLAG(&huart1); //������ڿ����жϱ�־λ�����������ô����Ҳ�ǿ���̬Ŷ~

    USART1_TX_FLAG = 0; // ���÷��ͱ�־λ

    huart1.gState = HAL_UART_STATE_READY;
    hdma_usart1_tx.State = HAL_DMA_STATE_READY;
    __HAL_UNLOCK(&hdma_usart1_tx);
    // ����������HAL�⺯����bug��������Բο��Ҹ�������
    // huart1,hdma_usart1_tx ��״̬Ҫ�ֶ���λ��READY״̬
    // ��Ȼ���ͺ�����һֱ��Ϊͨ��æ���Ͳ��ٷ��������ˣ�
  }
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}


/***********************************************************
*@fuction	:USART1_IRQHandler
*@brief		:���ڽ��������жϴ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
  { 
	  Rxflag = 1;	
    // __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    // ��һ������ʵ�� HAL_UART_IRQHandler(&huart1) Ҳ����ˡ�

    HAL_UART_DMAStop(&huart1); // ��DMA����ͣ������ֹ�ٶȹ��쵼���ж����룬���ݱ���д��
	
    data_length = MAX_RX_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
    // �����ܳ���=���޽��ճ���-DMAʣ��Ľ��ճ���
    if (WhichBufIsReady)	//WhichBufIsReady=1
    {
      RxData = u2rxbuf;        // u2rxbuf ���Զ�ȡ������ָ��ָ������
      p_IsToReceive = u1rxbuf; // u1rxbuf ��Ϊ��һ��DMA�洢�Ļ��壬ռ��ָ��ָ������
      WhichBufIsReady = 0;		//�л�һ��ָʾ��״̬
    }
    else				//WhichBufIsReady=0
    {
      RxData = u1rxbuf;        // u1rxbuf ���Զ�ȡ������ָ��ָ������
      p_IsToReceive = u2rxbuf; // u2rxbuf ��Ϊ��һ��DMA�洢�Ļ��壬ռ��ָ��ָ������
      WhichBufIsReady = 1;		//�л�һ��ָʾ��״̬
    }
    //�����濪ʼ���Դ�������յ������������ٸ����ӣ������յ�������ԭԭ�����Ļ���ȥ
      
    //DMA_USART1_Tx_Data(RxData,data_length);//���ݴ��ȥ�����Ⱦ������ݳ���
      
    memset((uint8_t *)p_IsToReceive, 0, MAX_RX_LEN);	// �ѽ������ݵ�ָ��ָ��Ļ��������
  }
  HAL_UART_IRQHandler(&huart1);
  HAL_UART_Receive_DMA(&huart1, p_IsToReceive, MAX_RX_LEN); //���ݴ�����ϣ�������������
}





