#ifndef __DAC_H_
#define __DAC_H_
#include "stm32f10x.h"                  // Device header

extern uint16_t g_Wave1[];
void DACModel_Init(uint32_t Buf_Addr ,uint16_t Buf_Count);
void DAC1_Triangle_Wave(uint16_t low, uint16_t high);

#endif 
