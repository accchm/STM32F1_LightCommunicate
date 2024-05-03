#include "DAC.h"

void DACModel_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);   
    TIM_TimeBaseStructure.TIM_Period = 140;                        //
    TIM_TimeBaseStructure.TIM_Prescaler = 8000;         
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //��Ϊ���¼���  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

    /*                      DAC��ʼ��                            */
    DAC_InitTypeDef DAC_InitStrcuture;

    DAC_InitStrcuture.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStrcuture.DAC_OutputBuffer = DAC_OutputBuffer_Enable;              //ʹ�û�����
    DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T2_TRGO;                       //��ʱ��2����
    DAC_InitStrcuture.DAC_WaveGeneration = DAC_WaveGeneration_None;
    
    DAC_Init(DAC_Channel_1,&DAC_InitStrcuture);
}

