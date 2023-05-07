#include "SoftWare_I2C.h"

void I2C_Delay(void)//�����ʱ2us
{
	uint32_t Delay = 1 * 168/4;
	do{__NOP();}
	while (Delay --);
}
void I2C_W_SCL(uint8_t x)
{//SCLд��
	if(x==0){HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);}
	else{HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);}
}
void I2C_W_SDA(uint8_t x)
{//SDAд��
	if(x==0){HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);}
	else{HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);}
}
uint8_t I2C_R_SCL(void)
{//��ȡSCL�ĵ�ƽ
	return HAL_GPIO_ReadPin(I2C_SCL_GPIO_Port, I2C_SCL_Pin);
}
uint8_t I2C_R_SDA(void)
{//��ȡSCL�ĵ�ƽ
	return HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
}
//�ڲ�ʹ�ã��û��������
void I2C_Start(void)
{
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_Delay();
	I2C_W_SDA(0);
	I2C_Delay();
	I2C_W_SCL(0);
	I2C_Delay();
}

//�ڲ�ʹ�ã��û��������
void I2C_Stop(void)
{
	I2C_W_SDA(0);
	I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SCL(1);
	I2C_Delay();
	I2C_W_SDA(1);
	I2C_Delay();
}

unsigned char I2C_Wait_Ack(void)
{
	unsigned char ucErrTime=0;

	I2C_W_SDA(1); //׼����������
	I2C_W_SCL(1);  

	while(I2C_R_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
//			printf("err");
			return 1;
		}
	}
	I2C_W_SCL(0);
	I2C_Delay();
	return 0;
}

void Write_I2C_Byte(unsigned char I2C_Byte)
{
	unsigned char i = 8;
	while(i--)		
	{
		if(I2C_Byte & 0x80){I2C_W_SDA(1);}
		else{I2C_W_SDA(0);}
		I2C_Byte <<= 1;		//������һλ
		I2C_Delay();		
		I2C_W_SCL(1); //��ʼ����
		I2C_Delay();
		I2C_W_SCL(0); //�������
	}
}

unsigned char I2C_Read_Byte(void)
{
	unsigned char receive=0;
	int i;
	
	I2C_W_SCL(0);
	for(i=0;i<8;i++)
	{
		I2C_W_SDA(1);
		I2C_Delay();
		I2C_W_SCL(1);//��ʼ��һλ��
		I2C_Delay();
		receive <<= 1;
		if(I2C_R_SDA())	receive++;  //DATAΪ�ߡ���λ��1 ������Ϊ0��������
		I2C_W_SCL(0);
		I2C_Delay();
   }					 
    return receive;
}

void I2C_SendACK(void)
{
	I2C_W_SDA(0);
	I2C_Delay();
    I2C_W_SCL(1);
	I2C_Delay();
    I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SDA(1);
	I2C_Delay();
}

void I2C_NoACK(void)
{
	I2C_W_SDA(1);
	I2C_Delay();
    I2C_W_SCL(1);
	I2C_Delay();
    I2C_W_SCL(0);
	I2C_Delay();
	I2C_W_SDA(0);
	I2C_Delay();
}



