#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 
#include <stdarg.h>

extern uint16_t USART_RX_DATA;
extern uint8_t USART_RX_Buf[200];   //接收缓冲区
extern uint8_t USART_RX_Flag;      //串口接收标志位
extern uint8_t USART_RX_Len;
extern char USART_RX_BufStr[100];

void Serial_Init(u32 bound);
void Serial_SendByte(uint8_t Byte);


#endif


