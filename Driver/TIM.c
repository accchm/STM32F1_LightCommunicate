#include "stm32f10x.h"                  // Device header


extern uint16_t i;


void Timer4_Init(void)                 //��ʱ��
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           // TIM3ʹ��
	
	TIM_InternalClockConfig(TIM4);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ�� 100us
	TIM_TimeBaseInitStructure.TIM_Prescaler = 3600 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);                                  //�����־λ
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                             //�����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC��ʼ��
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE);                      //��ʱ��ʹ��
	
}

void Timer3_Init(void) 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_InternalClockConfig(TIM3);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ��
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //�����־λ
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                             //�����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC��ʼ��
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}

void Timer2_Init(void)                 //��ʱ��
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           // TIM3ʹ��
	
	TIM_InternalClockConfig(TIM2);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ�� 100us
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                                  //�����־λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                             //�����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC��ʼ��
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                      //��ʱ��ʹ��
	
}

void Timer1_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);                //����ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;                          //��ʼ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                           //PA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);  
	
	TIM_InternalClockConfig(TIM1);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ��
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;                 //�Ƚϻ����������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;            //�������ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;              //ʹ��
	TIM_OCInitStructure.TIM_Pulse = 30;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void Timer1_DeInit(void)
{
	TIM_Cmd(TIM1,DISABLE);
}

void Timer2_DeInit(void) 
{
	TIM_Cmd(TIM2,DISABLE);
}
       //��ʱ���жϺ���
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
//	{
//		i++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}

void Time_CountInit(void)             //������
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           // TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);          //GPIOAʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //����ģʽ ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                                //ARR = 10000�ж�
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                      //PSC��1000 ARR+1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x02);        //ʹ���ⲿʱ�� ������
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                                  //�����־λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                             //�����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //NVIC��ʼ��
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                      //��ʱ��ʹ��
	
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);	//���ض�ʱ��TIM2��CNT
}

//void TIM3_IRQHandler(void)                   //���TIM3 50ms�ж�һ�Σ��ۼ�20�μ���Ƶ��
//{
//	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
//	{ 
//		{
//			static u8 i;
//			static u32 cnt;
//			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);   //���ж�
//			if(i<19)
//			{
//				cnt += TIM_GetCounter(TIM2) + count * 0XFFFF;  //����ȡ��������ֵ���ۼӼ������
//				TIM_SetCounter(TIM2,0);    //����������
//				count = 0;
//				i++;
//			}
//			else
//			{
//				cnt += TIM_GetCounter(TIM2) + count * 0XFFFF;
//				TIM_SetCounter(TIM2,0);
////				cnt += cnt*0.000025;	//����ʵ������޸�ϵ�����Բ���
//				frequent = cnt;
//				count = 0;
//				i = 0;
//				cnt = 0;
//			}
//		}
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
//	{
//		count++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}
