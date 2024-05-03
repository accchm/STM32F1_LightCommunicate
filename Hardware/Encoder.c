#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           // TIM3ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;                  //������ռ�ö�ʱ����ͨ��1��ͨ��2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);          //ʹ���ڲ�ʱ��     
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //��ʱģʽ ���ϼ�ʱ
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                                //�Զ���װֵ   ÿ��һ����Ϊ10k��֮һ�� ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                      //��Ƶ7200��Ƶ�ʾ�Ϊ10k         PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                //�߼���ʱ��ר��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);                                  //�����־λ
	
	TIM_ICInitTypeDef TIM_ICStructure;                      //����ģʽ��ʼ��
	TIM_ICStructInit(&TIM_ICStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																                 	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
														                 			//����ṹ���ֵ��ȷ��������
	TIM_ICStructure.TIM_Channel = TIM_Channel_1;            //ͨ��1
	TIM_ICStructure.TIM_ICFilter =0XF;                      //�˲�
	TIM_ICInit(TIM3,&TIM_ICStructure);                   //���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	TIM_ICStructure.TIM_Channel = TIM_Channel_2;            //ͨ��2
	TIM_ICStructure.TIM_ICFilter =0XF;                      //�˲�
	TIM_ICInit(TIM3,&TIM_ICStructure);                   //���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//���ñ�����ģʽ�Լ���������ͨ���Ƿ���
																	//ע���ʱ������Rising��Falling�Ѿ������������غ��½����ˣ����Ǵ����Ƿ���
																	//�˺������������벶���ʼ��֮����У��������벶������ûḲ�Ǵ˺����Ĳ�������
	TIM_Cmd(TIM3,ENABLE);                      //��ʱ��ʹ��
}
int16_t Encoder_Get(void)
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���CNT��������*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}
