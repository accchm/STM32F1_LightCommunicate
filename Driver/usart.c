#include "usart.h"		 

uint16_t USART_RX_DATA;
uint8_t USART_RX_Buf[200];   //接收缓冲区
uint8_t USART_RX_Flag;      //串口接收标志位
uint8_t USART_RX_Len;
char USART_RX_BufStr[100];

//typedef enum
//{
//	USART_RXStage_Start,
//	USART_RXStage_Get,
//	USART_RXStage_Stop
//}USART_RXSTage;

//USART_RXSTage USART_RXStage;
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void Serial_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		  //上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
//void USART1_IRQHandler(void)                	//串口1中断服务程序 接收单个字节
//{
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //接收中断
//	{
//		USART_RX_DATA =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
//	}
//}

//void USART1_IRQHandler()            //接收HEX数据包
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
//			if(USART_RX_DATA == 0xFE)          //如果接收到了数据尾
//			{
//				USART_RX_Flag = 1;
//			}
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
//	}
//}

//void USART1_IRQHandler()        //接受文本数据包
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
//			if(USART_RX_DATA == '\n')          //如果接收到了数据尾
//			{
//				USART_RX_BufStr[pRxPacket] = '\0';
//				USART_RX_Flag = 1;
//			}
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
//	}
//}
//发送单个字节
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);            //等待发送成功
}	

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)             //发送为文本形式的数字
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字   对0的ASCLL码偏移当前数字即为文本的数字
	}
}

//多个串口使用printf函数可使用此函数
/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}

void Serial_SendPacket(void)
{
	
}
