#include "stm32f10x.h"                  // Device header

#define TIM TIM3

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);                //����ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;                          //��ʼ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                  //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                           //PA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ��    ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k      PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM,TIM_FLAG_Update);                                  //�����־λ
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);                              //��ʼ���ṹ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //ģʽΪPWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //��ƽ����ת
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;            //ʹ��
	TIM_OCInitStructure.TIM_Pulse = 50;                                      //CCR��ֵ
	TIM_OC1Init(TIM,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM,ENABLE);                      //��ʱ��ʹ��
	
	/************************************************
	Ƶ�� = ʱ��Ƶ�� / ��PSC+1��/��ARR+1��
	ռ�ձ� = CCR / ��ARR + 1��
	�ֱ��� = 1/��ARR+1��
	************************************************/
}

void PWM_SetCompare1(uint16_t Compare)         
{
	TIM_SetCompare1(TIM2, Compare);		//����CCR1��ֵ������ռ�ձ�
}

void PWM_SetPrescaler(uint16_t prescaler)          //����PSC��ֵ������Ƶ��
{
	TIM_PrescalerConfig(TIM2,prescaler,TIM_PSCReloadMode_Update);
}
