#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"                  // Device header

extern uint8_t adc_complete_flag;
extern uint16_t AD_Data[4];
void AD_Init(void);
void AD_DeInit(void);
#endif
