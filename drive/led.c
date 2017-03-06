#include "stm32f0xx.h"
#include "led.h"

void LED_Init(void)
{
	uint8_t IO;
	RCC->AHBENR |= 1<<17;	//ʹ��PORTAʱ��	
	//RCC->AHBENR |= 1<<18;	//ʹ��PORTBʱ��
	//RCC->AHBENR |= 1<<19;	//ʹ��PORTCʱ��
	//RCC->AHBENR |= 1<<20;	//ʹ��PORTDʱ��
	//RCC->AHBENR |= 1<<22;	//ʹ��PORTEʱ��
	//RCC->AHBENR |= 1<<0;	//ʹ��DMAʱ��
	IO=5;
	GPIOA->MODER   |= 1<<(IO*2);	//0:  ����ģʽ ( ��λ״̬ ) 1:  ͨ�����ģʽ 2:  ���ù���ģʽ 3:  ģ��ģʽ
	GPIOA->OTYPER  |= 0<<IO;	//0:  ������� ( ��λ״̬ ) 1:  ��©���
	GPIOA->OSPEEDR |= 2<<(IO*2);	//x0:  ���� 1:  ���� 2:  ����
	GPIOA->PUPDR   |= 1<<(IO*2);	//0:  ������������ 1:  ���� 2:  ���� 
	 
	//GPIOA->AFR[0]  |= 0<<(3*4);	//(0- 7��) 0: AF0 1: AF1 2: AF2 3: AF3 4: AF4 5: AF5 6: AF6 7: AF7
	//GPIOA->AFR[1]  |= 0<<(3*4);	//(8-15��) 0: AF0 1: AF1 2: AF2 3: AF3 4: AF4 5: AF5 6: AF6 7: AF7
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
