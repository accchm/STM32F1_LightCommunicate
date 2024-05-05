#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "TIM.h"
#include "oled.h"
#include "IC.h"
#include "usart.h"
#include "AD.h"
#include "ds18b20.h"
#include <math.h>
#include <stdlib.h>
#include "Delay.h"
#include "stm32_dsp.h"
#include "lcd.h"
#include "DAC.h"

void Send_Wendu(float wendu);

#define FRE_40k 28
#define FRE_80k 7

uint8_t SendFlag = 0;
float wendu = 25;
int main(void)
{
    OLED_Init();
    Timer3_Init();
	DAC1_Triangle_Wave(0,4095);
    DS18B20_Init();
    while(1)
    {
        wendu = DS18B20_GetTemperture();
        OLED_Show_Float(0,0,wendu,1,16);
        OLED_Refresh_Gram();
        if(SendFlag)
        {
            Send_Wendu(wendu);
            SendFlag = 0;
        }
    }
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		SendFlag = 1;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void Send_Wendu(float wendu)
{
    uint16_t temp = 0;
    uint8_t i = 0;

    TIM4->CNT = 0;
    TIM4->PSC = FRE_40k;
    Delay_ms(50);
    TIM4->CNT = 0;
    TIM4->PSC = FRE_80k;
    Delay_ms(50);
    TIM4->CNT = 0;
    TIM4->PSC = FRE_40k;
    Delay_ms(50);                    //起始信号
    temp = (uint16_t)(wendu);
    for(i = 0;i < 6;i++)
    {
        if(temp&(1<<i))
        {
            TIM4->CNT = 0;
            TIM4->PSC = FRE_80k;
        }
        else
        {
            TIM4->CNT = 0;
            TIM4->PSC = FRE_40k;
        }
        Delay_ms(50);
    }               //发送整数部分

    /*                   发送小数部分               */
    temp = (uint16_t)(wendu * 10) % 10;
    for(i = 0; i < 4; i++)
    {
        if(temp&(1<<i))
        {
            TIM4->CNT = 0;
            TIM4->PSC = FRE_80k;
        }
        else
        {
            TIM4->CNT = 0;
            TIM4->PSC = FRE_40k;
        }
        Delay_ms(50);
    }
    TIM4->PSC = FRE_80k;
}

