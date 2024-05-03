#include "Key.h"
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);                  //重映射，加了之后没法用ST—LINK下载
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //上拉模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t Keynum_Get(void)
{
	uint8_t keynum = 0;
	static uint8_t key_flag = 0;
	if((!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)) || (key_flag == 1))
	{
		if(key_flag != 1)
		{
			Delay_ms(10);
			key_flag = 1;
			Delay_ms(10);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) && (key_flag == 1))
		{
		  keynum = 1;
			key_flag = 0;
		}
	}
	else if((!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14)) || key_flag == 2)
	{
		if(key_flag != 2)
		{
			Delay_ms(10);
			key_flag = 2;
			Delay_ms(10);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14) && key_flag == 2)
		{
		  keynum = 2;
			key_flag = 0;
		}
	}
	else if((!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)) || key_flag == 3)
	{
		if(key_flag != 3)
		{
			Delay_ms(10);
			key_flag = 3;
			Delay_ms(10);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13) && key_flag == 3)
		{
		  keynum = 3;
			key_flag = 0;
		}
	}
	else if((!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)) || key_flag == 4)
	{
		if(key_flag != 4)
		{
			Delay_ms(10);
			key_flag = 4;
			Delay_ms(10);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) && key_flag == 4)
		{
		  keynum = 4;
			key_flag = 0;
		}
	}
	return keynum;
}
