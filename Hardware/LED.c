#include "LED.h"                  // Device header


/**
  * @brief  LED初始化
  * @param  none
  * @retval none
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStucture;
	GPIO_InitStucture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStucture.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	LED_All_Off();
}

/**
  * @brief 点亮或者熄灭指定LED灯（默认GPIOA）
  * @param 点亮灯的位置
  * @param   置1点亮 置0熄灭
  * @retval 
  */

void LED_Ctrl(uint8_t LED_num,uint8_t LEDFlag)
{
	switch(LED_num)
	{
		case 0:  GPIO_WriteBit(GPIOA,GPIO_Pin_0,LEDFlag ? Bit_RESET : Bit_SET);break;     
		case 1:  GPIO_WriteBit(GPIOA,GPIO_Pin_1,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 2:  GPIO_WriteBit(GPIOA,GPIO_Pin_2,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 3:  GPIO_WriteBit(GPIOA,GPIO_Pin_3,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 4:  GPIO_WriteBit(GPIOA,GPIO_Pin_4,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 5:  GPIO_WriteBit(GPIOA,GPIO_Pin_5,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 6:  GPIO_WriteBit(GPIOA,GPIO_Pin_6,LEDFlag ? Bit_RESET : Bit_SET);break;
		case 7:  GPIO_WriteBit(GPIOA,GPIO_Pin_7,LEDFlag ? Bit_RESET : Bit_SET);break;
//		case 8:  GPIO_WriteBit(GPIOA,GPIO_Pin_8,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 9:  GPIO_WriteBit(GPIOA,GPIO_Pin_9,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 10:  GPIO_WriteBit(GPIOA,GPIO_Pin_10,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 11:  GPIO_WriteBit(GPIOA,GPIO_Pin_11,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 12:  GPIO_WriteBit(GPIOA,GPIO_Pin_12,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 13:  GPIO_WriteBit(GPIOA,GPIO_Pin_13,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 14:  GPIO_WriteBit(GPIOA,GPIO_Pin_14,LEDFlag ? Bit_SET : Bit_RESET);break;
//		case 15:  GPIO_WriteBit(GPIOA,GPIO_Pin_15,LEDFlag ? Bit_SET : Bit_RESET);break;
	}
}

//打开全部灯
void LED_All_On(void)
{
	GPIO_Write(GPIOA,0);
}
//关闭全部灯
void LED_All_Off(void)
{
	GPIO_Write(GPIOA,0XFFFF);
}
