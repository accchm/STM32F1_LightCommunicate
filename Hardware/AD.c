#include "stm32f10x.h"                  // Device header

#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1�ĵ�ַ
uint16_t AD_Data[4];
uint8_t adc_complete_flag;

//AD��ͨ����ʱ�ɼ�
void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //����ʱ��
	
	//DMA��ʼ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;                             //��ʼ��NVIC
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = 1024;                               //ת�˴���
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //���赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//�洢�����洢����ѡ��ʧ�ܣ�������ADC���败��ת�˵��洢��
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Data;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       //�洢����ַ����
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);                     //�����ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;                     //16λ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                               //�����ַ������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ģʽ��ѡ��ѭ��ģʽ����ADC������ת��һ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          //���ȼ�
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);                   //ʹ�ܴ�������ж�
	
	DMA_Cmd(DMA1_Channel1,ENABLE);              //DMAʹ��
	
	//ADC��ʼ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//����ͨ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);          //������1��λ������Ϊͨ��0
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
			
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                  //����������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;         //�ⲿ���������ö�ʱ����ʱ��������ǰΪ��ʱ��4CH4����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                       //ADC1����ʹ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;                                  //ͨ����
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                            //��ͨ��ɨ��
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//�����ⲿ����ģʽʹ��
	ADC_DMACmd(ADC1,ENABLE);                 //ADC����DMAʹ��
	
	ADC_ResetCalibration(ADC1);				//��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));				//�ȴ�У׼�Ĵ�����λ���
	ADC_StartCalibration(ADC1);				//ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�У׼���
	
	
	//TIM��ʼ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           // TIM4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //����ʼ�����Ŷ�ADC�ɼ�û��Ӱ��
	
	TIM_InternalClockConfig(TIM4);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ��    ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k      PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);                                  //�����־λ
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);                              //��ʼ���ṹ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //ģʽΪPWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //��ƽ����ת
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;            //ʹ��
	TIM_OCInitStructure.TIM_Pulse = 4;                                      //CCR��ֵ
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                    //��ƽ��ת
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;              //�������ö����Դﵽ��ƽ��ת��Ч��
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM4, ENABLE); 			              //ʹ��TIMx
}
void AD_DeInit(void)
{
	TIM_Cmd(TIM4,DISABLE);
}

/************************************************
	Ƶ�� = ʱ��Ƶ�� / ��PSC+1��/��ARR+1��
	ռ�ձ� = CCR / ��ARR + 1��
	�ֱ��� = 1/��ARR+1��
	************************************************/

//void ADC1_DMA1_IT_Hander(void)
//{	
//	if (DMA_GetFlagStatus(DMA1_FLAG_TC1))
//	{
//		DMA_ClearITPendingBit(DMA1_FLAG_TC1);
//	//	adc_complete_flag = 1;                     //DMAת����ɱ�־
//	}
//}
