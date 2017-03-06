#include "key.h"
#include "stm32f0xx.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_7| GPIO_Pin_6 );
	
	}
	
 void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 


uint8_t KEY_Down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
		  /*����Ƿ��а������� */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 ) 
	  {	   
	 	   /*��ʱ����*/
	  	 Delay(10000);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
					{	 
						/*�ȴ������ͷ� */
						while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}

