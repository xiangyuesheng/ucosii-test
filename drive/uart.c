#include "stm32f0xx.h"
#include "uart.h"


void USART_Configuration(void)
  {     
      GPIO_InitTypeDef GPIO_InitStruct;
    	USART_InitTypeDef USART_InitStruct;	
     	//串口时钟配置	
	  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
		 //gpio配置
		 /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
		/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
		
		//串口模式的配置
			 USART_InitStruct.USART_BaudRate = 115200;
			 USART_InitStruct.USART_WordLength = USART_WordLength_8b;
			 USART_InitStruct.USART_StopBits = USART_StopBits_1;
			 USART_InitStruct.USART_Parity = USART_Parity_No ;
			 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			 USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			 USART_Init(USART1, &USART_InitStruct); 
			 USART_Cmd(USART1, ENABLE);
		}			
		                                 

	int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}
