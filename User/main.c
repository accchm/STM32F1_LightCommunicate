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


float wendu = 25;
int main(void)
{
    OLED_Init();
	DAC1_Triangle_Wave(0,4095);
    DS18B20_Init();
    while(1)
    {
        wendu = DS18B20_GetTemperture();
        OLED_ShowNum(0,0,wendu,2,16);
        OLED_Refresh_Gram();
    }
}

void Send_Wendu(float wendu)
{
    uint16_t temp = 0;
    uint8_t i = 0;
    temp = (uint16_t)(wendu);
    for(i = 0;i < 6;i++)
    {
        if(temp&(1<<i))
        {
            TIM6->PSC = 72;
        }
        else
        {
            TIM6->PSC = 36;
        }
        Delay_ms(50);
    }               //发送整数部分

    /*                   发送小数部分               */
    temp = (uint16_t)(wendu * 10) % 10;
    for(i = 0; i < 4; i++)
    {
        if(temp&(1<<i))
        {
            TIM6->PSC = 72;
        }
        else
        {
            TIM6->PSC = 36;
        }
        Delay_ms(50);
    }
}

