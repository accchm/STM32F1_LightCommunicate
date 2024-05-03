#include "stm32f10x.h"                  // Device header

#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1的地址
uint16_t AD_Data[4];
uint8_t adc_complete_flag;

//AD多通道定时采集
void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //开启时钟
	
	//DMA初始化
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;                             //初始化NVIC
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = 1024;                               //转运次数
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//存储器到存储器，选择失能，数据由ADC外设触发转运到存储器
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Data;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //存储器地址自增
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);                     //外设地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;                     //16位数据
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                               //外设地址不自增
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//模式，选择循环模式，与ADC的连续转换一致
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          //优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);                   //使能传输完成中断
	
	DMA_Cmd(DMA1_Channel1,ENABLE);              //DMA使能
	
	//ADC初始化
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//配置通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);          //将序列1的位置配置为通道0
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
			
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                  //不进行连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;         //外部触发，可用定时器定时触发，当前为定时器4CH4触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                       //ADC1独立使用
	ADC_InitStructure.ADC_NbrOfChannel = 1;                                  //通道数
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                            //多通道扫描
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//设置外部触发模式使能
	ADC_DMACmd(ADC1,ENABLE);                 //ADC——DMA使能
	
	ADC_ResetCalibration(ADC1);				//复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));				//等待校准寄存器复位完成
	ADC_StartCalibration(ADC1);				//ADC校准
	while(ADC_GetCalibrationStatus(ADC1));				//等待校准完成
	
	
	//TIM初始化
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           // TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //不初始化引脚对ADC采集没有影响
	
	TIM_InternalClockConfig(TIM4);          //使用内部时钟     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计时模式 向上计时
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;                                //自动重装值   每记一次数为10k分之一秒    ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      //分频7200，频率就为10k      PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //高级定时器专用
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);                                  //清除标志位
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);                              //初始化结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //模式为PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //电平不翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;            //使能
	TIM_OCInitStructure.TIM_Pulse = 4;                                      //CCR的值
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                    //电平翻转
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;              //两种设置都可以达到电平翻转的效果
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM4, ENABLE); 			              //使能TIMx
}
void AD_DeInit(void)
{
	TIM_Cmd(TIM4,DISABLE);
}

/************************************************
	频率 = 时钟频率 / （PSC+1）/（ARR+1）
	占空比 = CCR / （ARR + 1）
	分辨率 = 1/（ARR+1）
	************************************************/

//void ADC1_DMA1_IT_Hander(void)
//{	
//	if (DMA_GetFlagStatus(DMA1_FLAG_TC1))
//	{
//		DMA_ClearITPendingBit(DMA1_FLAG_TC1);
//	//	adc_complete_flag = 1;                     //DMA转换完成标志
//	}
//}
