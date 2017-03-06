#ifndef __RTC_H
#define	__RTC_H

#include "stm32f0xx.h"
#include <stdio.h>

void RTC_Exit(void);
void RTC_Config(void);
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_AlarmShow(void);
uint8_t USART_Scanf(uint32_t value);


#endif /* __RTC_H */
