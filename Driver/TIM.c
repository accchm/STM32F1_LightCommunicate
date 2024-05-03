#include "stm32f10x.h"                  // Device header


extern uint16_t i;


void Timer4_Init(void)                 //计时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           // TIM3使能
	
	TIM_InternalClockConfig(TIM4);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                                //自动重装值   每记一次数为10k分之一秒 100us
	TIM_TimeBaseInitStructure.TIM_Prescaler = 3600 - 1;                      //分频7200，频率就为10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);                                  //清除标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                             //允许中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC初始化
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE);                      //定时器使能
	
}

void Timer3_Init(void) 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_InternalClockConfig(TIM3);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                                //自动重装值   每记一次数为10k分之一秒
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;                      //分频7200，频率就为10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //清除标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                             //允许中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC初始化
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}

void Timer2_Init(void)                 //计时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           // TIM3使能
	
	TIM_InternalClockConfig(TIM2);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //自动重装值   每记一次数为10k分之一秒 100us
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;                      //分频7200，频率就为10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                                  //清除标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                             //允许中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC初始化
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                      //定时器使能
	
}

void Timer1_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);                //引脚时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;                          //初始化引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                           //PA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);  
	
	TIM_InternalClockConfig(TIM1);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //自动重装值   每记一次数为10k分之一秒
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                      //分频7200，频率就为10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;                 //比较互补输出极性
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;            //互补输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;              //使能
	TIM_OCInitStructure.TIM_Pulse = 30;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void Timer1_DeInit(void)
{
	TIM_Cmd(TIM1,DISABLE);
}

void Timer2_DeInit(void) 
{
	TIM_Cmd(TIM2,DISABLE);
}
       //定时器中断函数
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
//	{
//		i++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}

void Time_CountInit(void)             //计数器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           // TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);          //GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //浮空模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计数模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                                //ARR = 10000中断
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                      //PSC计1000 ARR+1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x02);        //使用外部时钟 上升沿
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                                  //清除标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                             //允许中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC初始化
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                      //定时器使能
	
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);	//返回定时器TIM2的CNT
}

//void TIM3_IRQHandler(void)                   //配合TIM3 50ms中断一次，累计20次计算频率
//{
//	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
//	{ 
//		{
//			static u8 i;
//			static u32 cnt;
//			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);   //清中断
//			if(i<19)
//			{
//				cnt += TIM_GetCounter(TIM2) + count * 0XFFFF;  //，获取计数器的值，累加减少误差
//				TIM_SetCounter(TIM2,0);    //计数器清零
//				count = 0;
//				i++;
//			}
//			else
//			{
//				cnt += TIM_GetCounter(TIM2) + count * 0XFFFF;
//				TIM_SetCounter(TIM2,0);
////				cnt += cnt*0.000025;	//根据实际情况修改系数线性补偿
//				frequent = cnt;
//				count = 0;
//				i = 0;
//				cnt = 0;
//			}
//		}
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
//	{
//		count++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}
