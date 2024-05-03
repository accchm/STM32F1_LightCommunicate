#include "stm32f10x.h"                  // Device header

#define TIM TIM3

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //引脚时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;                          //初始化引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                           //PA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //自动重装值   每记一次数为10k分之一秒    ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                      //分频7200，频率就为10k      PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM,TIM_FLAG_Update);                                  //清除标志位
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);                              //初始化结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //模式为PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //电平不翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;            //使能
	TIM_OCInitStructure.TIM_Pulse = 50;                                      //CCR的值
	TIM_OC1Init(TIM,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM,ENABLE);                      //定时器使能
	
	/************************************************
	频率 = 时钟频率 / （PSC+1）/（ARR+1）
	占空比 = CCR / （ARR + 1）
	分辨率 = 1/（ARR+1）
	************************************************/
}

void PWM_SetCompare1(uint16_t Compare)         
{
	TIM_SetCompare1(TIM2, Compare);		//设置CCR1的值，设置占空比
}

void PWM_SetPrescaler(uint16_t prescaler)          //设置PSC的值，设置频率
{
	TIM_PrescalerConfig(TIM2,prescaler,TIM_PSCReloadMode_Update);
}
