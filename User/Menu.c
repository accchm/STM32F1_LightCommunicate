#include "Menu.h"

float Fun_9_Freq = 0;
uint8_t Delayus_100_Flag = 0, AMP_Sin = 1, FRE_Sin = 1;

void Fun_1(void)
{
	OLED_ShowChar(0,0,'>',12,1);
	OLED_ShowString(24,0,"ADC_Capture",12);
	OLED_ShowString(24,12,"PWM_OUT",12);
	OLED_ShowString(24,24,"GET_Freq",12);
	OLED_ShowString(24,36,"SIN_OUT",12);
	OLED_ShowString(24,48,"USART_TEST",12);
	OLED_Refresh_Gram();
}
void Fun_2(void)
{
	OLED_ShowChar(0,12,'>',12,1);
	OLED_ShowString(24,0,"ADC_Capture",12);
	OLED_ShowString(24,12,"PWM_OUT",12);
	OLED_ShowString(24,24,"GET_Freq",12);
	OLED_ShowString(24,36,"SIN_OUT",12);
	OLED_ShowString(24,48,"USART_TEST",12);
	OLED_Refresh_Gram();
}
void Fun_3(void)
{
	OLED_ShowChar(0,24,'>',12,1);
	OLED_ShowString(24,0,"ADC_Capture",12);
	OLED_ShowString(24,12,"PWM_OUT",12);
	OLED_ShowString(24,24,"GET_Freq",12);
	OLED_ShowString(24,36,"SIN_OUT",12);
	OLED_ShowString(24,48,"USART_TEST",12);
	OLED_Refresh_Gram();
}
void Fun_4(void)
{
	OLED_ShowChar(0,36,'>',12,1);
	OLED_ShowString(24,0,"ADC_Capture",12);
	OLED_ShowString(24,12,"PWM_OUT",12);
	OLED_ShowString(24,24,"GET_Freq",12);
	OLED_ShowString(24,36,"SIN_OUT",12);
	OLED_ShowString(24,48,"USART_TEST",12);
	OLED_Refresh_Gram();
}
void Fun_5(void)
{
	OLED_ShowChar(0,48,'>',12,1);
	OLED_ShowString(24,0,"ADC_Capture",12);
	OLED_ShowString(24,12,"PWM_OUT",12);
	OLED_ShowString(24,24,"GET_Freq",12);
	OLED_ShowString(24,36,"SIN_OUT",12);
	OLED_ShowString(24,48,"USART_TEST",12);
	OLED_Refresh_Gram();
}

void Fun_6(void)
{
	OLED_ShowString(0,0,"ADC1:",12);
	OLED_ShowString(0,12,"ADC2:",12);
	OLED_ShowString(0,24,"ADC3:",12);
	OLED_ShowString(0,36,"ADC4:",12);
	
	if(adc_complete_flag)
	{
		OLED_ShowNum(32,0,AD_Data[0],4,12);
		OLED_ShowNum(32,12,AD_Data[1],4,12);
		OLED_ShowNum(32,24,AD_Data[2],4,12);
		OLED_ShowNum(32,36,AD_Data[3],4,12);
		adc_complete_flag = 0;
	}
	OLED_Refresh_Gram();
}

void Fun_7(void)
{
	OLED_ShowString(0,0,"PWM1Duty:",12);
	OLED_ShowString(0,12,"PWM2Duty:",12);
	OLED_ShowNum(64,0,TIM_GetCapture1(TIM1),3,12);
	OLED_ShowNum(64,12,(100 - TIM_GetCapture1(TIM1)),3,12);
	OLED_Refresh_Gram();
}
void Fun_8(void)
{
	OLED_ShowString(0,0,"Freq:",12);
	OLED_ShowNum(36,0,IC_GetFreq() * 1.016,3,12);
	OLED_ShowString(0,12,"Duty:",12);
	OLED_ShowNum(36,12,IC_GetDuty(),2,12);
	OLED_Refresh_Gram();
}
void Fun_9(void)
{
	OLED_ShowString(0,0,"Freq:KEY1",12);
	OLED_ShowString(0,12,"AMP:KEY2",12);
	if(Delayus_100_Flag)
	{
		Fun_9_Freq += 1;
		if(Fun_9_Freq >= 100)
		{
			Fun_9_Freq = 0;
		}
		printf("%f\r\n", AMP_Sin * sin( FRE_Sin * (float)Fun_9_Freq / 6.28));
		Delayus_100_Flag = 0;
	}
	OLED_Refresh_Gram();
}

void Fun_10(void)
{
	OLED_ShowString(0,0,"USARTTesting¡¤¡¤¡¤",12);
	OLED_Refresh_Gram();
}
