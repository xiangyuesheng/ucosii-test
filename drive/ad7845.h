#ifndef _AD7845_H_
#define _AD7845_H_

#include "stm32f0xx.h"
#include  "ili9328.h"

// volatile unsigned char touch_flag=0;

// ------------------------------------
// 硬件连接：PB10---T-INT
//           PA4----T-CS 
//           PA5----T-CLK
//           PA6----T-OUT
// 					 PA7----T-DIN
//          SPI1-- AF0
//
// -------------------------------------
#define TOUCH_INT_PIN        GPIO_Pin_10
#define TOUCH_INT_PORT       GPIOB
#define TOUCH_INT_PIN_SCK    RCC_AHBPeriph_GPIOB

#define TOUCH_CS_PIN        GPIO_Pin_4
#define TOUCH_CS_PORT       GPIOA
#define TOUCH_CS_PIN_SCK    RCC_AHBPeriph_GPIOA

#define TOUCH_SCK_PIN        GPIO_Pin_5
#define TOUCH_SCK_PORT       GPIOA
#define TOUCH_SCK_PIN_SCK    RCC_AHBPeriph_GPIOA
#define TOUCH_SCK_SOURCE     GPIO_PinSource5
#define TOUCH_SCK_AF         GPIO_AF_0

#define TOUCH_MISO_PIN       GPIO_Pin_6
#define TOUCH_MISO_PORT      GPIOA
#define TOUCH_MISO_PIN_SCK   RCC_AHBPeriph_GPIOA
#define TOUCH_MISO_SOURCE    GPIO_PinSource6
#define TOUCH_MISO_AF        GPIO_AF_0

#define TOUCH_MOSI_PIN       GPIO_Pin_7
#define TOUCH_MOSI_PORT      GPIOA
#define TOUCH_MOSI_PIN_SCK   RCC_AHBPeriph_GPIOA
#define TOUCH_MOSI_SOURCE    GPIO_PinSource7
#define TOUCH_MOSI_AF        GPIO_AF_0


#define TOUCH_SPI1               RCC_APB2Periph_SPI1
#define SPI_TOUCH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_TOUCH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SPI_TOUHC_INT          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)




/* AD通道选择命令字和工作寄存器 */
#define	CHX 	0x90 	/* 通道Y+的选择控制字 */	
#define	CHY 	0xd0	/* 通道X+的选择控制字 */
#define THRESHOLD 2  
/****************************************************/
/*                                                  */
/* Structures                                       */
/*                                                  */
/****************************************************/

typedef struct point
{ 
   uint16_t     x;
   uint16_t     y;
}Point;

typedef struct matrix
{
		//配置校验系数
   long double     An;     /* A = An/Divider */
   long double     Bn;     /* B = Bn/Divider */
   long double     Cn;     /* C = Cn/Divider */
   long double     Dn;     /* D = Dn/Divider */
   long double     En;     /* E = En/Divider */
   long double     Fn;     /* F = Fn/Divider */
   long double    Divider ;
}Matrix;

extern Matrix touch_para ;
extern Point  display ;

void DelayUS(uint32_t cnt);
void AD7845_Init(void);
void TOUCH_Int(void);
static void WR_Cmd(uint8_t cmd);
static int READ_Ad(void);
int Read_X(void);
int Read_Y(void);
void Touch_GetAdXY(int *x,int *y) ;
Point *Read_Point(void);
extern int setCalibrationMatrix( Point * display,Point * screen,Matrix * matrix) ;
extern int getDisplayPoint( Point * display, Point * screen,Matrix * matrix ) ;
int Touch_Calibrate(void);

#endif  /* _AD7845_H_ */
