#include "DAC.h"

void DACModel_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);   
    TIM_TimeBaseStructure.TIM_Period = 140;                        //
    TIM_TimeBaseStructure.TIM_Prescaler = 8000;         
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //设为向下计数  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

    /*                      DAC初始化                            */
    DAC_InitTypeDef DAC_InitStrcuture;

    DAC_InitStrcuture.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStrcuture.DAC_OutputBuffer = DAC_OutputBuffer_Enable;              //使用缓冲区
    DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T2_TRGO;                       //定时器2触发
    DAC_InitStrcuture.DAC_WaveGeneration = DAC_WaveGeneration_None;
    
    DAC_Init(DAC_Channel_1,&DAC_InitStrcuture);
}

