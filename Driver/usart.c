#include "usart.h"		 

uint16_t USART_RX_DATA;
uint8_t USART_RX_Buf[200];   //���ջ�����
uint8_t USART_RX_Flag;      //���ڽ��ձ�־λ
uint8_t USART_RX_Len;
char USART_RX_BufStr[100];

//typedef enum
//{
//	USART_RXStage_Start,
//	USART_RXStage_Get,
//	USART_RXStage_Stop
//}USART_RXSTage;

//USART_RXSTage USART_RXStage;
int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void Serial_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

/*******************************************************************************
* �� �� ��         : USART1_IRQHandler
* ��������		   : USART1�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
//void USART1_IRQHandler(void)                	//����1�жϷ������ ���յ����ֽ�
//{
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //�����ж�
//	{
//		USART_RX_DATA =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//���USART1��RXNE��־λ
//	}
//}

//void USART1_IRQHandler()            //����HEX���ݰ�
//{
//	static uint16_t pRxPacket = 0;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		USART_RX_DATA = USART_ReceiveData(USART1);
//		if(USART_RXStage == USART_RXStage_Start && USART_RX_Flag == 0)
//		{
//			if(USART_RX_DATA == 0xFF)
//			{
//				USART_RXStage = USART_RXStage_Get;
//				pRxPacket = 0;
//			}
//		}
//		else if(USART_RXStage == USART_RXStage_Get)
//		{
//			USART_RX_Buf[pRxPacket] = USART_RX_DATA;
//			pRxPacket++;
//			if(pRxPacket >= USART_RX_Len)
//			{
//				USART_RXStage = USART_RXStage_Stop;
//			}
//		}
//		else if(USART_RXStage == USART_RXStage_Stop)
//		{
//			USART_RXStage = USART_RXStage_Start;
//			if(USART_RX_DATA == 0xFE)          //������յ�������β
//			{
//				USART_RX_Flag = 1;
//			}
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//���USART1��RXNE��־λ
//	}
//}

//void USART1_IRQHandler()        //�����ı����ݰ�
//{
//	static uint16_t pRxPacket = 0;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		USART_RX_DATA = USART_ReceiveData(USART1);
//		if(USART_RXStage == USART_RXStage_Start && USART_RX_Flag == 0)
//		{
//			if(USART_RX_DATA == '*')
//			{
//				USART_RXStage = USART_RXStage_Get;
//				pRxPacket = 0;
//			}
//		}
//		else if(USART_RXStage == USART_RXStage_Get)
//		{
//			if(USART_RX_DATA == '\r')
//			{
//				USART_RXStage = USART_RXStage_Stop;
//			}
//			else
//			{
//				USART_RX_BufStr[pRxPacket] = USART_RX_DATA;
//				pRxPacket++;
//			}
//		}
//		else if(USART_RXStage == USART_RXStage_Stop)
//		{
//			USART_RXStage = USART_RXStage_Start;
//			if(USART_RX_DATA == '\n')          //������յ�������β
//			{
//				USART_RX_BufStr[pRxPacket] = '\0';
//				USART_RX_Flag = 1;
//			}
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//���USART1��RXNE��־λ
//	}
//}
//���͵����ֽ�
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);            //�ȴ����ͳɹ�
}	

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial_SendByte(Array[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial_SendByte(String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)             //����Ϊ�ı���ʽ������
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����   ��0��ASCLL��ƫ�Ƶ�ǰ���ּ�Ϊ�ı�������
	}
}

//�������ʹ��printf������ʹ�ô˺���
/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	Serial_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

void Serial_SendPacket(void)
{
	
}
