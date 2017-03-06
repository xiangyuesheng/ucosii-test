#include "stm32f0xx.h"
#include "led.h"

void LED_Init(void)
{
	uint8_t IO;
	RCC->AHBENR |= 1<<17;	//使能PORTA时钟	
	//RCC->AHBENR |= 1<<18;	//使能PORTB时钟
	//RCC->AHBENR |= 1<<19;	//使能PORTC时钟
	//RCC->AHBENR |= 1<<20;	//使能PORTD时钟
	//RCC->AHBENR |= 1<<22;	//使能PORTE时钟
	//RCC->AHBENR |= 1<<0;	//使能DMA时钟
	IO=5;
	GPIOA->MODER   |= 1<<(IO*2);	//0:  输入模式 ( 复位状态 ) 1:  通用输出模式 2:  复用功能模式 3:  模拟模式
	GPIOA->OTYPER  |= 0<<IO;	//0:  推挽输出 ( 复位状态 ) 1:  开漏输出
	GPIOA->OSPEEDR |= 2<<(IO*2);	//x0:  低速 1:  中速 2:  高速
	GPIOA->PUPDR   |= 1<<(IO*2);	//0:  无上拉和下拉 1:  上拉 2:  下拉 
	 
	//GPIOA->AFR[0]  |= 0<<(3*4);	//(0- 7脚) 0: AF0 1: AF1 2: AF2 3: AF3 4: AF4 5: AF5 6: AF6 7: AF7
	//GPIOA->AFR[1]  |= 0<<(3*4);	//(8-15脚) 0: AF0 1: AF1 2: AF2 3: AF3 4: AF4 5: AF5 6: AF6 7: AF7
}

void LED1_Open(void)
{
GPIOA->BRR = GPIO_ODR_5;
}
void LED1_Close(void)
{
GPIOA->BSRR = GPIO_ODR_5;
}


void LED2_Open(void)
{

}
	
void LED2_Close(void)
{

}
