#include "stm32f10x.h"                  // Device header



//����ģʽ��ʼ��
void PWM_ICInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM3ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ�� ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k         PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //�����־λ
	
	TIM_ICInitTypeDef TIM_ICStructure;                      //����ģʽ��ʼ��
	TIM_ICStructure.TIM_Channel = TIM_Channel_1;            //ͨ��1
	TIM_ICStructure.TIM_ICFilter =0XF;                      //�˲�
	TIM_ICStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //�����ش�������
	TIM_ICStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //��Ƶ
	TIM_ICStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;       //�����źŽ��棬ѡ��ֱͨ��������
	TIM_PWMIConfig(TIM3,&TIM_ICStructure);                   //���ṹ���������TIM_PWMIConfig������TIM3�����벶��ͨ��
	                                                          ////�˺���ͬʱ�����һ��ͨ������Ϊ�෴�����ã�ʵ��PWMIģʽ
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);          //����Դѡ��TI1FP1
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);					//��ģʽѡ��λ
	
	
	TIM_Cmd(TIM3,ENABLE);                      //��ʱ��ʹ��
}
void PWM_DeICInit(void)
{
	TIM_Cmd(TIM3,DISABLE);   
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / TIM_GetCapture1(TIM3);          //������ֵת��ΪƵ��
}
/************************************************
	Ƶ�� = ʱ��Ƶ�� / ��PSC+1��/��ARR+1��
	ռ�ձ� = CCR / ��ARR + 1��
	�ֱ��� = 1/��ARR+1��
	************************************************/
uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);                 //����ռ�ձ�
}


//void IC_ShowFreq(void)
//{
//	OLED_ShowNum(0,0,IC_GetFreq()/100000,1,12);
//	OLED_ShowNum(8,0,IC_GetFreq()/10000%10,1,12);
//	OLED_ShowNum(16,0,IC_GetFreq()/1000%10,1,12);
//	OLED_ShowNum(24,0,IC_GetFreq()/100%10,1,12);
//	OLED_ShowNum(32,0,IC_GetFreq()/10%10,1,12);
//	OLED_ShowNum(40,0,IC_GetFreq()%10,1,12);
//	OLED_ShowNum(0,30,IC_GetDuty()%10,2,12);
//}
