#include "DAC.h"


uint16_t g_Wave1[128];
void DACModel_Init(uint32_t Buf_Addr ,uint16_t Buf_Count)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    //GPIOA
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
    }
    //TIM2
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);   
        TIM_TimeBaseStructure.TIM_Period = 2 - 1;                        //
        TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;         
        TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //设为向上计数  
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
        TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    }
    //DAC
    {
        DAC_InitTypeDef DAC_InitStrcuture;
        DAC_InitStrcuture.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
        DAC_InitStrcuture.DAC_OutputBuffer = DAC_OutputBuffer_Disable;              //不使用缓冲区
        DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T2_TRGO;                       //定时器2触发
        DAC_InitStrcuture.DAC_WaveGeneration = DAC_WaveGeneration_None;
        DAC_Init(DAC_Channel_1,&DAC_InitStrcuture);
    }
    //DMA
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.DMA_BufferSize = Buf_Count;                               //转运次数
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;            //存储器到外设
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//存储器到存储器，选择失能，数据由存储器转运到DAC
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_Wave1;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //存储器地址自增
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12R1);                     //外设地址
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;                     //16位数据
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                               //外设地址不自增
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//模式，选择循环模式，与ADC的连续转换一致
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          //优先级
        DMA_Init(DMA1_Channel1,&DMA_InitStructure);
    }
    DAC_DMACmd(DAC_Channel_1, ENABLE);
    DMA_Cmd(DMA1_Channel1,ENABLE);              //DMA使能
    DAC_Cmd(DAC_Channel_1, ENABLE);
//    DAC_SetDualChannelData(DAC_Align_12b_R, 0, 0);
    TIM_Cmd(TIM2,ENABLE);
}

void DAC1_Triangle_Wave(uint16_t low, uint16_t high)
{
    uint16_t m = 64, dac;
    uint8_t i = 0;
    TIM_Cmd(TIM2,DISABLE);
	
	if (m == 0)
	{
		m = 1;
	}
	
	if (m > 127)
	{
		m = 127;
	}
	for (i = 0; i < m; i++)
	{
		dac = low + ((high - low) * i) / m;
		g_Wave1[i] = dac;
	}
	for (; i < 128; i++)
	{
		dac = high - ((high - low) * (i - m)) / (128 - m);
		g_Wave1[i] = dac;
	}
	
	DACModel_Init((uint32_t)&g_Wave1, 128);
}
