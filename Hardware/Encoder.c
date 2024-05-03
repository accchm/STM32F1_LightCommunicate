#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM3使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;                  //编码器占用定时器的通道1和通道2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                                //自动重装值   每记一次数为10k分之一秒 ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                      //分频7200，频率就为10k         PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //清除标志位
	
	TIM_ICInitTypeDef TIM_ICStructure;                      //捕获模式初始化
	TIM_ICStructInit(&TIM_ICStructure);							//结构体初始化，若结构体没有完整赋值
																                 	//则最好执行此函数，给结构体所有成员都赋一个默认值
														                 			//避免结构体初值不确定的问题
	TIM_ICStructure.TIM_Channel = TIM_Channel_1;            //通道1
	TIM_ICStructure.TIM_ICFilter =0XF;                      //滤波
	TIM_ICInit(TIM3,&TIM_ICStructure);                   //将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	TIM_ICStructure.TIM_Channel = TIM_Channel_2;            //通道2
	TIM_ICStructure.TIM_ICFilter =0XF;                      //滤波
	TIM_ICInit(TIM3,&TIM_ICStructure);                   //将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//配置编码器模式以及两个输入通道是否反相
																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	TIM_Cmd(TIM3,ENABLE);                      //定时器使能
}
int16_t Encoder_Get(void)
{
	/*使用Temp变量作为中继，目的是返回CNT后将其清零*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}
