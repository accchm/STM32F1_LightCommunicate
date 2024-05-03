#include "stm32f10x.h"                  // Device header

void EXTI_Init_model(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);       //打开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);        //打开引脚时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //配置引脚
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);     //配置PB14引脚AFIO
	
	EXTI_InitTypeDef EXTI_InitStructure;             //配置外部中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;                 //配置14引脚
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  //开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;            //中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;         //上升沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //配置中断分组
	NVIC_InitTypeDef NVIC_InitStructure;                 //NVIC初始化
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                //选择通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                //配置响应优先级
	NVIC_Init(&NVIC_InitStructure);
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		
		
		EXTI_ClearITPendingBit(EXTI_Line14);     //清除标志
	}
}
