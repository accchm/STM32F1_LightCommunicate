#include "DAC.h"


uint16_t g_Wave1[16];
const u16 Sine12bit[32] = { //�������
2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};


void DACModel_Init(uint32_t Buf_Addr ,uint16_t Buf_Count)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
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
    {    /*                                  ���ǲ�            */
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);   
        TIM_TimeBaseStructure.TIM_Period = 2 - 1;                        //
        TIM_TimeBaseStructure.TIM_Prescaler =45 - 1;         //15K
        TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //��Ϊ���ϼ���  
        TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); 
        TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
        /*                                    ���Ҳ�                */
        TIM_TimeBaseStructure.TIM_Period = 2 - 1;                        //
        TIM_TimeBaseStructure.TIM_Prescaler =28 - 1;         //80K-7   40k-28
        TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;      
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //��Ϊ���ϼ���  
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
        TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
    }
    //DAC
    {
        DAC_InitTypeDef DAC_InitStrcuture;
        DAC_InitStrcuture.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
        DAC_InitStrcuture.DAC_OutputBuffer = DAC_OutputBuffer_Disable;              //��ʹ�û�����
        DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T6_TRGO;                       //��ʱ��2����
        DAC_InitStrcuture.DAC_WaveGeneration = DAC_WaveGeneration_None;
        DAC_Init(DAC_Channel_1,&DAC_InitStrcuture);
        DAC_InitStrcuture.DAC_Trigger = DAC_Trigger_T4_TRGO;
        DAC_Init(DAC_Channel_2,&DAC_InitStrcuture);
    }
    //DMA
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.DMA_BufferSize = Buf_Count;                               //ת�˴���
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;            //�洢��������
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//�洢�����洢����ѡ��ʧ�ܣ������ɴ洢��ת�˵�DAC
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_Wave1;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //�洢����ַ����
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12R1);                     //�����ַ
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;                     //16λ����
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                               //�����ַ������
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ģʽ��ѡ��ѭ��ģʽ����ADC������ת��һ��
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          //���ȼ�
        DMA_Init(DMA2_Channel3,&DMA_InitStructure);
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;                     //16λ����
        DMA_InitStructure.DMA_BufferSize = 32;                               //ת�˴���
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12R2);                     //�����ַ
        DMA_Init(DMA2_Channel4,&DMA_InitStructure);
    }
    DAC_DMACmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_2, ENABLE);
    DMA_Cmd(DMA2_Channel3,ENABLE);              //DMAʹ��
    DMA_Cmd(DMA2_Channel4,ENABLE);              //DMAʹ��
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
//    DAC_SetDualChannelData(DAC_Align_12b_R, 0, 0);
    TIM_Cmd(TIM4,ENABLE);
    TIM_Cmd(TIM6,ENABLE);
}

void DAC1_Triangle_Wave(uint16_t low, uint16_t high)
{
    uint16_t m = 2, dac;
    uint8_t i = 0;
    TIM_Cmd(TIM6,DISABLE);
	
	if (m == 0)
	{
		m = 1;
	}
	
	if (m > 2 * m - 1)
	{
		m = 2 * m - 1;
	}
	for (i = 0; i < m; i++)
	{
		dac = low + ((high - low) * i) / m;
		g_Wave1[i] = dac;
	}
	for (; i < 2 * m; i++)
	{
		dac = high - ((high - low) * (i - m)) / (2 * m - m);
		g_Wave1[i] = dac;
	}
	/* g_Wave1[0] = 0;
    g_Wave1[1] = 4096; */
	DACModel_Init((uint32_t)&g_Wave1, m);
}
