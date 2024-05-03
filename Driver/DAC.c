#include "DAC.h"


uint16_t DAC_Data[2] = {0,4095};
void DACModel_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
    //TIM2
    {
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);   
        TIM_TimeBaseStructure.TIM_Period = 140;                        //
        TIM_TimeBaseStructure.TIM_Prescaler = 7200;         
        TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //��Ϊ���¼���  
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
        TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    }
    //DAC
    {
        DAC_InitTypeDef DAC_InitStrcuture;
        DAC_InitStrcuture.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
        DAC_InitStrcuture.DAC_OutputBuffer = DAC_OutputBuffer_Disable;              //��ʹ�û�����
        DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T2_TRGO;                       //��ʱ��2����
        DAC_InitStrcuture.DAC_WaveGeneration = DAC_WaveGeneration_None;
        DAC_Init(DAC_Channel_1,&DAC_InitStrcuture);
    }
    //DMA
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
	
        DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.DMA_BufferSize = 2;                               //ת�˴���
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;            //�洢��������
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//�洢�����洢����ѡ��ʧ�ܣ������ɴ洢��ת�˵�DAC
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DAC_Data;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //�洢����ַ����
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12R1);                     //�����ַ
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;                     //16λ����
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                               //�����ַ������
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ģʽ��ѡ��ѭ��ģʽ����ADC������ת��һ��
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          //���ȼ�
        DMA_Init(DMA1_Channel1,&DMA_InitStructure);
    }
    DMA_Cmd(DMA1_Channel1,ENABLE);              //DMAʹ��
    DAC_Cmd(DAC_Channel_1, ENABLE);
    TIM_Cmd(TIM2,ENABLE);
}

