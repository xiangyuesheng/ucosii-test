#ifndef __UART_H
#define	__UART_H

#include "stm32f0xx.h"
#include <stdio.h>

void USART_Configuration(void);
int fputc(int ch, FILE *f);


#endif /* __UART_H */
