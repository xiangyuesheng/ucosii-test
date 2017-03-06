// ------------------------------------
// Ӳ�����ӣ�PB10---T-INT
//           PA4----T-CS 
//           PA5----T-CLK
//           PA6----T-OUT
// 					 PA7----T-DIN
//           SPI1-- AF0
//
// -------------------------------------
#include "ad7845.h"
#include "ili9328.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_exti.h"

extern volatile unsigned char touch_flag;

Matrix touch_para ;
Point  display ;
long double linear=0 ;

 long double aa1=0,bb1=0,cc1=0,aa2=0,bb2=0,cc2=0;
/* DisplaySample LCD�����϶�Ӧ��ads7843����ADֵ �磺LCD ����45,45 Ӧ�õ�X Y����ADC�ֱ�Ϊ3388,920 */	
Point ScreenSample[4];
/* ��4��У��� */
Point DisplaySample[4] =   
{
    { 45, 45 },
    { 45, 270},
    { 190,190},
    {210,280}
} ;


	/******************************************************
* ��������AD7845_Init();
* ����  ����ʼ��ADC784��spi�ӿ�
* ����  : ��������ʱ�ӣ�Ȼ������GPIO���������ADC�˿ڸ���
* ���  ����
* ע��  ��
*********************************************************/ 
void AD7845_Init(void)
{ 
	//������Ҫ���õĲ���
	GPIO_InitTypeDef  GPIO_InitStruct;
  SPI_InitTypeDef   SPI_InitStruct;
	//����ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	 RCC_AHBPeriphClockCmd(TOUCH_CS_PIN_SCK|TOUCH_SCK_PIN_SCK|TOUCH_MISO_PIN_SCK | TOUCH_MOSI_PIN_SCK|TOUCH_INT_PIN_SCK , ENABLE);
	 RCC_APB2PeriphClockCmd(TOUCH_SPI1, ENABLE); 
	//����spiʹ�õ���GPIO�˿�
	
// 	/< Configure TOUCH_SPI pins: SCK */
  GPIO_InitStruct.GPIO_Pin = TOUCH_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(TOUCH_SCK_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_SPI pins: MISO */
  GPIO_InitStruct.GPIO_Pin = TOUCH_MISO_PIN;
  GPIO_Init(TOUCH_MISO_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_SPI pins: MOSI */
  GPIO_InitStruct.GPIO_Pin =TOUCH_MOSI_PIN;
  GPIO_Init(TOUCH_MOSI_PORT, &GPIO_InitStruct);

  /*!< Configure TOUCH_CS_PIN pin */
  GPIO_InitStruct.GPIO_Pin =TOUCH_CS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);
	
   /*!< Configure TOUCH_TIN_PIN pin */
	GPIO_InitStruct.GPIO_Pin =TOUCH_INT_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TOUCH_INT_PORT, &GPIO_InitStruct);
	//��������
  GPIO_PinAFConfig(TOUCH_SCK_PORT, TOUCH_SCK_SOURCE, TOUCH_SCK_AF);
  GPIO_PinAFConfig(TOUCH_MISO_PORT, TOUCH_MISO_SOURCE, TOUCH_MISO_AF); 
  GPIO_PinAFConfig(TOUCH_MOSI_PORT, TOUCH_MOSI_SOURCE, TOUCH_MOSI_AF);
	
	//����spi
	SPI_TOUCH_CS_HIGH();
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStruct);
  
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  
  SPI_Cmd(SPI1, ENABLE); /*!< TOUCH_SPI enable */
	
	}
	/******************************************************
* ��������TOUCH_INT()��
* ����  ��AD7845�ж�Ƕ��
* ����  : 
* ���  ��
* ע��  ��
*********************************************************/    	
void TOUCH_Int(void)
	{ 
	
		NVIC_InitTypeDef NVIC_InitStruct;
		EXTI_InitTypeDef EXTI_InitStruct;
		
		NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		
		
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
		EXTI_InitStruct.EXTI_Line = EXTI_Line10;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct); 
		
		
		}
		
	/******************************************************
* ��������DelayUS
* ����  ��������ʱcnt ΢��
* ����  : cnt
* ���  ����
* ע��  ���ú���������Ŀ���Ǿֲ�ʹ��,�Ӷ���������ȥʹ�ö�ʱ������ʱ
*********************************************************/  
void DelayUS(uint32_t cnt)
{
    uint16_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* ����ֵΪ12����Լ��1΢�� */    
        while (us--)     /* ��1΢��	*/
        {
            ;   
        }
    }
}	

	/******************************************************
* ��������WR_Cmd();
* ����  ��ad7845 spiд����
* ����  : cmd
* ���  ����
* ע��  ��
*********************************************************/  

static void WR_Cmd(uint8_t cmd)
{   /* Wait for SPI1 Tx buffer empty */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
    /* Send SPI1 data */ 
    SPI_SendData8(SPI1,cmd); 
    /* Wait for SPI1 data reception */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
    /* Read SPI1 received data */ 
    SPI_ReceiveData8(SPI1); 
	
	}

	/******************************************************
* ��������READ_Ad();
* ����  ����AD7845��adֵ
* ����  : ��
* ���  ��temp2
* ע��  ��
*********************************************************/  	
	
static int READ_Ad(void)
	{
		unsigned short temp1,temp2; 
    /*  �ȴ����ݼĴ����� */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* ͨ��SPI1�������� */  
    SPI_SendData8(SPI1,0x0000); 

    /* �ȴ����յ�һ���ֽڵ����� */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 

    /* ��ȡSPI1���յ����� */		 
    temp1=SPI_ReceiveData8(SPI1); 

    temp2=temp1<<8; 
    DelayUS(1);

		/*  �ȴ����ݼĴ����� */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* ͨ��SPI1�������� */ 
    SPI_SendData8(SPI1,0x0000); 

    /* �ȴ����յ�һ���ֽڵ����� */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
 
    /* ��ȡSPI1���յ����� */ 
    temp1=SPI_ReceiveData8(SPI1); 
 
    temp2|= temp1; 
    temp2>>=3; 
    temp2&=0xfff; 
    return temp2; 
		}

	/******************************************************
* ��������READ_X();
* ����  ��д�����ȡAD7845ת����X���ֵ
* ����  : ��
* ���  ��i
* ע��  ��
*********************************************************/ 
		
	int Read_X(void)  
{  
    int i; 
    SPI_TOUCH_CS_LOW() ; 
    DelayUS(1); 
    WR_Cmd(CHX); 
    DelayUS(1); 
    i=READ_Ad(); 
    SPI_TOUCH_CS_HIGH(); 
    return i;    
} 

	/******************************************************
* ��������READ_Y();
* ����  ��д�����ȡAD7845ת����Y���ֵ
* ����  : ��
* ���  ��i
* ע��  ��
*********************************************************/ 
		
int Read_Y(void)  
{  
    int i; 
    SPI_TOUCH_CS_LOW() ; 
    DelayUS(1); 
    WR_Cmd(CHY); 
    DelayUS(1); 
    i=READ_Ad(); 
    SPI_TOUCH_CS_HIGH(); 
    return i;     
} 
	/******************************************************
* ��������Touch_GetAdXY(int *x,int *y) ;
* ����  : ��ȡX,Y���ADֵ
* ����  : ��
* ���  ��int *x,int *y
* ע��  ��
*********************************************************/ 
void Touch_GetAdXY(int *x,int *y)  
{ 
    int ADx,ADy; 
    ADx=Read_X(); 
    DelayUS(1); 
    ADy=Read_Y(); 
    *x=ADx; 
    *y=ADy; 
} 
	/******************************************************
* ��������Point *Read_Point(void) ;
* ����  : �������������Ѷ�ȡ��ADֵת��ΪTFT��Ļ���λ��
* ����  : ��
* ���  ��
* ע��  ��
*********************************************************/ 

Point *Read_Point(void)
{
    static Point  screen;
    int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
    uint8_t count=0;
		
		/* ����X��Y����9�β���*/
    int buffer[2][9]={{0},{0}};  	
    do
    {		   
        Touch_GetAdXY(TP_X,TP_Y);  
        buffer[0][count]=TP_X[0];  
        buffer[1][count]=TP_Y[0];
        count++; 
				 
    }	/*�û����������ʱ��TP_INT_IN�ź�Ϊ�� ���� count<9*/
    while(!SPI_TOUHC_INT&& count<9);  
    
		
		/*������ʵ���*/
		if(SPI_TOUHC_INT ==1)
    {
        /*�жϱ�־��λ*/
				touch_flag =0;						
    }

		/* ����ɹ�����9��,�����˲� */ 
    if(count==9)   								
    {  
        /* Ϊ����������,�ֱ��3��ȡƽ��ֵ */
        temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
        temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
        temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
        
        /* ����3�����ݵĲ�ֵ */
        m0=temp[0]-temp[1];
        m1=temp[1]-temp[2];
        m2=temp[2]-temp[0];
        
        /* ��������ֵȡ����ֵ */
        m0=m0>0?m0:(-m0);
        m1=m1>0?m1:(-m1);
        m2=m2>0?m2:(-m2);
        
        /* �жϾ��Բ�ֵ�Ƿ񶼳�����ֵ���ޣ������3�����Բ�ֵ����������ֵ�����ж���β�����ΪҰ��,���������㣬��ֵ����ȡΪ2 */
        if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) 
            return 0;
        
        /* �������ǵ�ƽ��ֵ��ͬʱ��ֵ��screen */ 
        if(m0<m1)
        {
            if(m2<m0) 
                screen.x=(temp[0]+temp[2])/2;
            else 
                screen.x=(temp[0]+temp[1])/2;	
        }
        else if(m2<m1) 
            screen.x=(temp[0]+temp[2])/2;
        else 
            screen.x=(temp[1]+temp[2])/2;
        
        /* ͬ�� ����Y��ƽ��ֵ */
        temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
        temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
        temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
        m0=temp[0]-temp[1];
        m1=temp[1]-temp[2];
        m2=temp[2]-temp[0];
        m0=m0>0?m0:(-m0);
        m1=m1>0?m1:(-m1);
        m2=m2>0?m2:(-m2);
        if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) 
            return 0;
        
        if(m0<m1)
        {
            if(m2<m0) 
                screen.y=(temp[0]+temp[2])/2;
            else 
                screen.y=(temp[0]+temp[1])/2;	
        }
        else if(m2<m1) 
            screen.y=(temp[0]+temp[2])/2;
        else
            screen.y=(temp[1]+temp[2])/2;
        
        return &screen;
    }
    
    else if(count>1)
    {
        screen.x=buffer[0][0];
        screen.y=buffer[1][0];
        return &screen;
    }  
    return 0; 
}

	/******************************************************
* ��������setCalibrationMatrix 
* ����  : �������ת������
* ����  : ��
* ���  ��
* ע��  ��
*********************************************************/ 

 extern int setCalibrationMatrix( Point * displayPtr,Point * screenPtr,Matrix * matrixPtr)
{

    int  retValue = 1 ;
    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

    if ( matrixPtr->Divider == 0 )
    {
        retValue = 0 ;
    }
    else
    {
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;

        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
    }

    return( retValue ) ;

} /* end of setCalibrationMatrix() */

	/******************************************************
* ��������setCalibrationMatrix 
* ����  : ����ʵ�ʵ�TFT�����
* ����  : ��
* ���  ��
* ע��  ��
*********************************************************/ 
  extern int getDisplayPoint( Point * displayPtr,Point * screenPtr,Matrix * matrixPtr )
{
    int  retValue = 1 ;
    if ( matrixPtr->Divider != 0 )
    {

        /* Operation order is important since we are doing integer */
        /*  math. Make sure you add all terms together before      */
        /*  dividing, so that the remainder is not rounded off     */
        /*  prematurely.                                           */

        displayPtr->x = ( (aa1 * screenPtr->x) +
                          (bb1 * screenPtr->y) +
                          cc1
                        ) ;

        displayPtr->y = ( (aa2 * screenPtr->x) +
                          (bb2 * screenPtr->y) +
                          cc2
                        )  ;
    }
    else
    {
        retValue = 0 ;
    }

    return( retValue ) ;

} 

/******************************************************
* ��������Touch_Calibrate
* ����  ��������У������
* ����  : ��
* ���  ��0	---	У���ɹ�
					1	---	У��ʧ��
* ����  ����
* ע��  ����
*********************************************************/    
int Touch_Calibrate(void)
{
    uint8_t i;
    uint16_t test_x=0, test_y=0;
    uint16_t gap_x=0, gap_y=0;
    Point * Ptr;
    for(i=0;i<4;i++)
    {   
			  LCD_Clear(0XFFFF);
        LCD_ShowString(10, 10,"Touch Calibrate");			 
        LCD_ShowNum(10, 25, i+1,4 );
        
        DelayUS(50);
        DrawCross(DisplaySample[i].x,DisplaySample[i].y);
        do
        {
            Ptr=Read_Point();
        }
        while( Ptr == (void*)0 );
        ScreenSample[i].x= Ptr->x; 
        ScreenSample[i].y= Ptr->y;

    }

		/* ����ֵ�õ����� */
    setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&touch_para ) ;  	   
    
		/*����Xֵ*/
    test_x = ( (touch_para.An * ScreenSample[3].x) + 
               (touch_para.Bn * ScreenSample[3].y) + 
               touch_para.Cn 
               ) / touch_para.Divider ;			 
    
		/*����Yֵ*/
    test_y = ( (touch_para.Dn * ScreenSample[3].x) + 
               (touch_para.En * ScreenSample[3].y) + 
               touch_para.Fn 
               ) / touch_para.Divider ;			 
    
    gap_x = (test_x > DisplaySample[3].x)?(test_x - DisplaySample[3].x):(DisplaySample[3].x - test_x);
    gap_x = (test_y > DisplaySample[3].y)?(test_y - DisplaySample[3].y):(DisplaySample[3].y - test_y);
    
    
//     LCD_Clear(0XFFFF);
    
    if((gap_x>8)||(gap_y>8))
    {
        LCD_ShowString(100, 80, "Calibrate fail");
        LCD_ShowString(100, 80, "  try again   ");
         DelayUS(2000000);
        return 1;
    }
    
    
    aa1 = (touch_para.An*1.0)/touch_para.Divider;
    bb1 = (touch_para.Bn*1.0)/touch_para.Divider;
    cc1 = (touch_para.Cn*1.0)/touch_para.Divider;
    
    aa2 = (touch_para.Dn*1.0)/touch_para.Divider;
    bb2 = (touch_para.En*1.0)/touch_para.Divider;
    cc2 = (touch_para.Fn*1.0)/touch_para.Divider;
    
    LCD_ShowString(100, 80,"Calibrate Success");
    DelayUS(1000000);
    
    return 0;    
}
