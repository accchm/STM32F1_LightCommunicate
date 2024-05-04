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

int main(void)
{
	DAC1_Triangle_Wave(0,4095);
    while(1)
    {

    }
}
