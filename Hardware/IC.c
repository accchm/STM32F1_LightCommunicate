#include "stm32f10x.h"                  // Device header



//捕获模式初始化
void PWM_ICInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM3使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                                //自动重装值   每记一次数为10k分之一秒 ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      //分频7200，频率就为10k         PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //清除标志位
	
	TIM_ICInitTypeDef TIM_ICStructure;                      //捕获模式初始化
	TIM_ICStructure.TIM_Channel = TIM_Channel_1;            //通道1
	TIM_ICStructure.TIM_ICFilter =0XF;                      //滤波
	TIM_ICStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿触发捕获
	TIM_ICStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //分频
	TIM_ICStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;       //输入信号交叉，选择直通，不交叉
	TIM_PWMIConfig(TIM3,&TIM_ICStructure);                   //将结构体变量交给TIM_PWMIConfig，配置TIM3的输入捕获通道
	                                                          ////此函数同时会把另一个通道配置为相反的配置，实现PWMI模式
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);          //触发源选择TI1FP1
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);					//从模式选择复位
	
	
	TIM_Cmd(TIM3,ENABLE);                      //定时器使能
}
void PWM_DeICInit(void)
{
	TIM_Cmd(TIM3,DISABLE);   
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / TIM_GetCapture1(TIM3);          //将捕获值转换为频率
}
/************************************************
	频率 = 时钟频率 / （PSC+1）/（ARR+1）
	占空比 = CCR / （ARR + 1）
	分辨率 = 1/（ARR+1）
	************************************************/
uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);                 //返回占空比
}


//void IC_ShowFreq(void)
//{
//	OLED_ShowNum(0,0,IC_GetFreq()/100000,1,12);
//	OLED_ShowNum(8,0,IC_GetFreq()/10000%10,1,12);
//	OLED_ShowNum(16,0,IC_GetFreq()/1000%10,1,12);
//	OLED_ShowNum(24,0,IC_GetFreq()/100%10,1,12);
//	OLED_ShowNum(32,0,IC_GetFreq()/10%10,1,12);
//	OLED_ShowNum(40,0,IC_GetFreq()%10,1,12);
//	OLED_ShowNum(0,30,IC_GetDuty()%10,2,12);
//}
