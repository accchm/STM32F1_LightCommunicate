#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"                  // Device header

void LED_Init(void);
void LED_Ctrl(uint8_t LED_num,uint8_t LEDFlag);
void LED_All_Off(void);
void LED_All_On(void);

#endif
