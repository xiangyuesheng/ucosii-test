/******************************************************************/
/* 名称：TFT驱动                                                   */
/* 硬件连接：PB0-PB7---DB0-DB7   低8位                             */
/*           PC0-PC7---DB8-DB7   高8位                             */
/*           PD2---WR  PC12---RD PC11---CS  PC10---RS  PB11---CM     */            
/* 作者：青风                                                     */
/*                  （店址：	qfv5.taobao.com）                   */
/* 联系方式：QQ:157736309                                         */
/******************************************************************/

#include"stm32f0xx.h"
#include"ili9328.h"
#include "w25x16.h"
#include "ascii.h"

uint16_t  POINT_COLOR=BLACK;
uint16_t  BACK_COLOR=WHITE;
/**************************************************/
/* 函数功能；短暂延时，为LCD初始化时序服务        */
/**************************************************/	

static void delay (int cnt) {

  cnt <<= DELAY_2N;
  while (cnt--);
}
/***************************************************/
/* 函数功能；给ILI9325的寄存器写数据               */
/* 入口参数；val：16位数据                         */
/* 说明：    写16位数据，学习板上的LCD采用8位连接，*/
/*           所以，16位数据分两次写进寄存器，先写高*/
/*           位，再写低位。                        */
/* 注意：使用时需要先使用LCD_WR_REG(reg)选择要把数 */
/*       据写到哪个寄存器里面。                    */
/***************************************************/
void LCD_WR_DATA(uint16_t val)
{  
	
 	Set_Rs;
	Clr_Cs;
// 	Set_nRd;	
  GPIOB->ODR &= 0xff00;
  GPIOC->ODR &= 0xff00;		
	GPIOB->ODR|=((val)&(0x00ff));
	GPIOC->ODR|=(val>>8);
	Clr_nWr;
	Set_nWr;
  Set_Cs;	
}

/****************************************************/
/* 函数功能：给ILI9238的写命令操作对应寄存器         */
/* 入口参数；写命令                                 */
/* 说明：    寄存器号是16位数，8位连接方式需要写两次*/
/****************************************************/
void LCD_WR_REG(uint16_t cmd)
{
	Clr_Rs;
	Clr_Cs;
// 	Set_nRd;
	 GPIOB->ODR &= 0xff00;
   GPIOC->ODR &= 0xff00;	
	GPIOB->ODR |=((cmd)&(0x00ff));
	GPIOC->ODR|=(cmd>>8);
	Clr_nWr;
	Set_nWr;
	 Set_Cs;	
	}
/******************************************************/
/* 函数功能向寄存器里写数据                        */
/* 入口参数；reg：寄存器号  data：数据值             */
/* 说明：    该函数是前两个函数的合成，也就是上面两个 */
/*           函数一般的用法就是这样组合。             */
/******************************************************/

void LCD_WR_REG_DATA(uint16_t reg, uint16_t data)
{ 
	 
   	LCD_WR_REG(reg);
   	LCD_WR_DATA(data);
 	
	}	
	
	
/****************************************************/
/* 函数功能：从ILI9238中读数据                      */
/* 入口参数；数据                               */
/* 说明：    寄存器号是16位数，8位连接方式需要写两次*/
/****************************************************/
uint16_t LCD_RD_DATA(void)
{ 
	unsigned short val,val1,val2;
	Set_Rs;
	Set_nWr;
	Clr_nRd;
	GPIOB->MODER&=0xffff0000;		   
  GPIOC->MODER&=0xffff0000;
	val1 = GPIOB->IDR;
	val2 = GPIOC->IDR;
	val=((val1)&(0x00ff))+((val2<<8)&(0xff00));
	               //输入完了后从新设置为输出
		GPIOB->MODER|=0x00005555; 
		GPIOC->MODER|=0x00005555; 
	Set_nRd;
	return val;
}	

/******************************************************/
/* 函数功能读寄存器里的数据                        */
/* 入口参数；reg：寄存器号                           */
/* 说明：    该函数是前两个函数的合成，也就是上面两个 */
/*           函数一般的用法就是这样组合。             */
/******************************************************/
uint16_t  LCD_RD_REG_DATA (uint16_t  reg) 
{
  uint16_t value;
 	Clr_Cs;
	LCD_WR_REG(reg);    
	value = LCD_RD_DATA();      	
	Set_Cs;
	return value;
}

/*******************************************************/
/* 函数功能：初始化LCD                                 */
/*******************************************************/
void LCD_init (void) 
{ uint16_t i;
	//////////////////////////////////////////////////////////////////////管脚初始化
	 RCC->AHBENR&=0xffe3ffff;
	 RCC->AHBENR|=0x001c0000;
	
	GPIOB->MODER&=0xffff0000;		   // 设置PB口低8位引脚为输出，用作LCD16位并行数据和控制引脚
	GPIOB->MODER|=0x00005555;  
// 	GPIOB->BSRR=0x00ff;
  GPIOB->ODR &= 0xff00;	
// 	GPIOB->ODR |= 0x00ff;	   // PB口低8位引脚置高
	
	GPIOC->MODER&=0xffff0000;		   // 设置PC口低8位引脚为输出，用作LCD16位并行数据和控制引脚
	GPIOC->MODER|=0x00005555; 
// 	GPIOC->BSRR=0x00ff;
	GPIOC->ODR &= 0xff00;	
// 	GPIOC->ODR |= 0x00ff;	   // PC口低8位引脚置高
	
	GPIOB->MODER&=0xff3fffff;
	GPIOB->MODER|=0x00400000;  //PB11为输出模式
	GPIOB->BRR =0x00000800;    //输出为低电平
	
	 GPIOC->MODER&= 0xfc0fffff;                                   //PD2---WR  PC12---RD PC11---CS  PC10---RS  
   GPIOC->MODER|=0x01500000;
	 
	 GPIOC->BSRR = 0x00001c00;
	 GPIOD->MODER&= 0xffffffcf;
   GPIOD->MODER|=0x00000010;
	 
	 GPIOD->BSRR = 0x00000004;
//////////////////////////////////////////////////////////////////	
	
	delay(5);                             /* Delay 50 ms                        */

	LCD_WR_REG_DATA(0x00e7,0x0010);      
	LCD_WR_REG_DATA(0x0000,0x0001);  			//start internal osc
	LCD_WR_REG_DATA(0x0001,0x0100);     
	LCD_WR_REG_DATA(0x0002,0x0700); 				//power on sequence                     
	LCD_WR_REG_DATA(0x0003,(1<<12)|(1<<5)|(1<<4) ); 	//65K 
	LCD_WR_REG_DATA(0x0004,0x0000);                                   
	LCD_WR_REG_DATA(0x0008,0x0207);	           
	LCD_WR_REG_DATA(0x0009,0x0000);         
	LCD_WR_REG_DATA(0x000a,0x0000); 				//display setting         
	LCD_WR_REG_DATA(0x000c,0x0001);				//display setting          
	LCD_WR_REG_DATA(0x000d,0x0000); 				//0f3c          
	LCD_WR_REG_DATA(0x000f,0x0000);
	//Power On sequence //
	LCD_WR_REG_DATA(0x0010,0x0000);   
	LCD_WR_REG_DATA(0x0011,0x0007);
	LCD_WR_REG_DATA(0x0012,0x0000);                                                                 
	LCD_WR_REG_DATA(0x0013,0x0000);                 
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
  LCD_WR_REG_DATA(0x0010,0x1590);   
	LCD_WR_REG_DATA(0x0011,0x0227);
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WR_REG_DATA(0x0012,0x009c);                  
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WR_REG_DATA(0x0013,0x1900);   
	LCD_WR_REG_DATA(0x0029,0x0023);
	LCD_WR_REG_DATA(0x002b,0x000e);
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WR_REG_DATA(0x0020,0x0000);                                                            
	LCD_WR_REG_DATA(0x0021,0x0000);           
	///////////////////////////////////////////////////////      
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WR_REG_DATA(0x0030,0x0007); 
	LCD_WR_REG_DATA(0x0031,0x0707);   
	LCD_WR_REG_DATA(0x0032,0x0006);
	LCD_WR_REG_DATA(0x0035,0x0704);
	LCD_WR_REG_DATA(0x0036,0x1f04); 
	LCD_WR_REG_DATA(0x0037,0x0004);
	LCD_WR_REG_DATA(0x0038,0x0000);        
	LCD_WR_REG_DATA(0x0039,0x0706);     
	LCD_WR_REG_DATA(0x003c,0x0701);
	LCD_WR_REG_DATA(0x003d,0x000f);
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WR_REG_DATA(0x0050,0x0000);        
	LCD_WR_REG_DATA(0x0051,0x00ef);   
	LCD_WR_REG_DATA(0x0052,0x0000);     
	LCD_WR_REG_DATA(0x0053,0x013f);
	LCD_WR_REG_DATA(0x0060,0xa700);        
	LCD_WR_REG_DATA(0x0061,0x0001); 
	LCD_WR_REG_DATA(0x006a,0x0000);
	LCD_WR_REG_DATA(0x0080,0x0000);
	LCD_WR_REG_DATA(0x0081,0x0000);
	LCD_WR_REG_DATA(0x0082,0x0000);
	LCD_WR_REG_DATA(0x0083,0x0000);
	LCD_WR_REG_DATA(0x0084,0x0000);
	LCD_WR_REG_DATA(0x0085,0x0000);
	
	LCD_WR_REG_DATA(0x0090,0x0010);     
	LCD_WR_REG_DATA(0x0092,0x0000);  
	LCD_WR_REG_DATA(0x0093,0x0003);
	LCD_WR_REG_DATA(0x0095,0x0110);
	LCD_WR_REG_DATA(0x0097,0x0000);        
	LCD_WR_REG_DATA(0x0098,0x0000);  
	 //display on sequence     
	LCD_WR_REG_DATA(0x0007,0x0133);
	
	LCD_WR_REG_DATA(0x0020,0x0000);                                                            
	LCD_WR_REG_DATA(0x0021,0x0000);
 
}

/****************************************************************************
* 名    称：void GLCD_Test(void)
* 功    能：测试液晶屏
* 入口参数：无
* 出口参数：无
* 说    明：显示彩条，测试液晶屏是否正常工作
* 调用方法：GLCD_Test();
****************************************************************************/
void GLCD_Test(void)
{
  uint16_t i,j;
  LCD_WR_REG_DATA(0x20, 0);
  LCD_WR_REG_DATA(0x21, 0);

  Clr_Cs; 
  LCD_WR_REG(0x22);
  
  for(i=0;i<320;i++)
    for(j=0;j<240;j++)
    {
      if(i>279)LCD_WR_DATA(0x0000);
      else if(i>239)LCD_WR_DATA(0x001f);
      else if(i>199)LCD_WR_DATA(0x07e0);
      else if(i>159)LCD_WR_DATA(0x07ff);
      else if(i>119)LCD_WR_DATA(0xf800);
      else if(i>79)LCD_WR_DATA(0xf81f);
      else if(i>39)LCD_WR_DATA(0xffe0);
      else LCD_WR_DATA(0xffff);
    }
  Set_Cs;
}

/**********************************************/
/* 函数功能；开启显示                         */
/* 说    明：一般用于关闭显示后的开启         */
/**********************************************/
void LCD_DisplayOn(void)
{
 	LCD_WR_REG_DATA(0x0007, 0x0133); // 开启显示
}
/**********************************************/
/* 函数功能；关闭显示                         */
/* 说    明：并不是使TFT黑屏，而是使得TFT上的 */
/*           内容保持不变，一般用于TFT的数据线*/
/*           和其它模块连接，操作其它模块的时 */
/*           候不希望影响TFT的显示。          */
/**********************************************/
void LCD_DisplayOff(void)
{
	LCD_WR_REG_DATA(0x0007, 0x0); // 关闭显示
}

/***************************************************/
/* 函数功能；设置显存区域                          */
/* 说    明：设置将要显示的显存XY起始和结束坐标    */
/* 注    意：用完以后要恢复起点(0,0)和终点(239,319)*/
/***************************************************/
void LCD_XYRAM(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend)
{
	LCD_WR_REG_DATA(0x0050, xstart); // 设置横坐标GRAM起始地址
	LCD_WR_REG_DATA(0x0051, xend); // 设置横坐标GRAM结束地址
	LCD_WR_REG_DATA(0x0052, ystart); // 设置纵坐标GRAM起始地址
	LCD_WR_REG_DATA(0x0053, yend); // 设置纵坐标GRAM结束地址
}

/*******************************************************/
/* 函数功能：设置TFT屏起始坐标                         */
/*******************************************************/
void LCD_SetC(uint16_t x, uint16_t y)
{
	LCD_WR_REG_DATA(0x0020,x);  //设置X坐标位置
	LCD_WR_REG_DATA(0x0021,y);  //设置Y坐标位置
}
/**********************************************/
/* 函数功能；清屏TFT                          */
/* 入口参数：color 颜色值                     */
/**********************************************/
void LCD_Clear(uint16_t color)
{
    uint32_t temp;
    LCD_WR_REG_DATA(0x0020,0);//设置X坐标位置
    LCD_WR_REG_DATA(0x0021,0);//设置Y坐标位置
    LCD_WR_REG(0x0022);				//指向RAM寄存器，准备写数据到RAM
	for(temp=0;temp<76800;temp++)
	{
		LCD_WR_DATA(color);    
	}
}
/******************************************/
/* 函数功能：画一个像素的点               */
/* 入口参数：x,y   点的坐标               */
/******************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_WR_REG_DATA(0x0020,x);//设置X坐标位置
    LCD_WR_REG_DATA(0x0021,y);//设置Y坐标位置
	LCD_WR_REG(0x0022);       //开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}
/******************************************/
/* 函数功能：画一个小十字标               */
/* 入口参数：x,y   点的坐标               */
/******************************************/
void DrawCross(uint16_t x,uint16_t y)
{
    uint8_t b;    
    for(b=0; b<10; b++)
    {
        LCD_DrawPoint(240-(x),y-b);
        LCD_DrawPoint(240-(x),y+b);
        LCD_DrawPoint(240-(x+b),y);
        LCD_DrawPoint(240-(x-b),y);
    }
       
}	

/******************************************/
/* 函数功能：读TFT某一点的颜色            */
/* 出口参数：color  点颜色值              */
/******************************************/
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
	uint16_t  color;

	LCD_WR_REG_DATA(0x0020,x);//设置X坐标位置
  LCD_WR_REG_DATA(0x0021,y);//设置Y坐标位置
	LCD_WR_REG(0x0022);       //开始写入GRAM
	
	GPIOB->MODER&=0xffff0000;		   
  GPIOC->MODER&=0xffff0000;        //把TFT数据引脚设置为输入
	
	color = LCD_RD_DATA();	  //读出GRAM值（注意：GRAM值必须读取两次）
	color = LCD_RD_DATA();	  //读出GRAM值（详见ILI932x数据手册）
	
	GPIOB->MODER|=0x00005555; 
	GPIOC->MODER|=0x00005555; 		  //恢复数据引脚为输出

	return color;
} 

/**********************************************/
/* 函数功能；画直线                           */
/**********************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/**********************************************/
/* 函数功能；画矩形                           */
/**********************************************/
void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(xstart, ystart, xend, ystart);
	LCD_DrawLine(xstart, yend, xend, yend);
	LCD_DrawLine(xstart, ystart, xstart, yend);
	LCD_DrawLine(xend, ystart, xend, yend);
}
/**********************************************/
/* 函数功能；矩形填充                         */
/**********************************************/
void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color)
{                    
    uint32_t max;
	LCD_XYRAM(xstart ,ystart ,xend ,yend); // 设置GRAM坐标
	LCD_WR_REG_DATA(0x0020,xstart);//设置X坐标位置
    LCD_WR_REG_DATA(0x0021,ystart);//设置Y坐标位置
    LCD_WR_REG(0x0022);				//指向RAM寄存器，准备写数据到RAM
	max=(uint32_t)((xend-xstart+1)*(yend-ystart+1));    
	while(max--)
	{
		LCD_WR_DATA(color);
	}
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // 恢复GRAM整屏显示
}
/*******************************************/
/* 函数功能：画圆                          */
/* 入口参数：x0,y0  圆心坐标               */
/*           r      半径(单位：像素)       */
/*******************************************/
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
/**********************************************/
/* 函数功能；求m的n次方                       */
/**********************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/**********************************************/
/* 函数功能：显示数字                         */
/* 入口参数：x,y :起点坐标	 	              */
/*           len :数字的位数				  */
/*           num:数值(0~4294967295);	   	  */
/**********************************************/
void LCD_ShowNum(uint8_t x,uint16_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;		 // 此变量用来去掉最高位的0	
				   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+'0'); 
	}
} 
/**********************************************/
/* 函数功能：显示8*16点阵英文字符             */
/* 入口参数：x,y :起点坐标	 	              */
/*           num:字母或符号         	   	  */
/* 注    意：x,y的取值要在240到320范围内      */
/**********************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t num)
{
    uint8_t temp;
    uint8_t pos,t;
	      
   	LCD_WR_REG_DATA(0x0020,x);//设置X坐标位置
    LCD_WR_REG_DATA(0x0021,y);//设置Y坐标位置
 	/*开辟显存区域*/
	LCD_XYRAM(x,y,x+7,y+15); // 设置GRAM坐标
    LCD_WR_REG(0x0022);	  	 //指向RAM寄存器，准备写数据到RAM
 	
	num=num-' ';//得到偏移后的值
	for(pos=0;pos<16;pos++)
	{
	    temp=ascii_16[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80)LCD_WR_DATA(POINT_COLOR);
	        else LCD_WR_DATA(BACK_COLOR);    
	        temp<<=1; 
	    }
	}
	/* 恢复显存显示区域240*320 */
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // 恢复GRAM整屏显示
	return;
}

uint8_t buf[32]; //用于存放16*16点阵汉字数据

/**********************************************/
/* 函数功能；从W25X16中提取点阵码             */
/* 入口参数：code:GBK码第一个字节             */
/*           dz_data:存放点阵码的数组         */
/**********************************************/
void Get_GBK_DZK(uint8_t *code, uint8_t *dz_data)
{
	uint8_t GBKH,GBKL;   // GBK码高位与低位					  
	uint32_t offset; 	   // 点阵偏移量

	GBKH=*code;
	GBKL=*(code+1);	   // GBKL=*(code+1);
	if(GBKH>0XFE||GBKH<0X81)return;
	GBKH-=0x81;
	GBKL-=0x40;
	offset=((uint32_t)192*GBKH+GBKL)*32;//得到字库中的字节偏移量 
	SPI_FLASH_BufferRead(dz_data,offset+0x100,32);
// 	W25X16_Read(dz_data,offset+0x100,32); 		  
	return;
}
/**********************************************/
/* 函数功能：显示16*16点阵中文                */
/* 入口参数：x,y :起点坐标	 	              */
/*           *hz：汉字               	   	  */
/**********************************************/
void LCD_Show_hz(uint16_t x,uint16_t y,uint8_t *hz)
{
	uint8_t i,j,temp;
	uint8_t dz_data[32];

	Get_GBK_DZK(hz, dz_data);

	LCD_WR_REG_DATA(0x0020,x);//设置X坐标位置
    LCD_WR_REG_DATA(0x0021,y);//设置Y坐标位置
 	/*开辟显存区域*/
	LCD_XYRAM(x,y,x+15,y+15); // 设置GRAM坐标
    LCD_WR_REG(0x0022);	  	 //指向RAM寄存器，准备写数据到RAM

 	for(i=0;i<32;i++)
	{
		temp=dz_data[i];
	 	for(j=0;j<8;j++)
		{
		 	if(temp&0x80)LCD_WR_DATA(POINT_COLOR);
	        else LCD_WR_DATA(BACK_COLOR);    
	        temp<<=1; 
		}
	}

	/* 恢复显存显示区域240*320 */
	LCD_XYRAM(0x0000 ,0x0000 ,0x00EF ,0X013F); // 恢复GRAM整屏显示
	
	return;
}

/*********************************************/
/* 函数功能：显示字符串（中文和英文）        */
/* 入口参数：x,y: 坐标                       */
/*           *p:字符串                       */
/*********************************************/
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t *p)
{         
    while(*p!='\0')	// 如果没有结束
    {       
        if(*p>0x80)//如果是中文
		{
			if((*p=='\n')||(x>224))  // 换段和换行
			{
				y=y+19;   //字体高16 行间距3
				x=2;      //2是边距
			}
		LCD_Show_hz(x, y, p);
			x+=16;
			p+=2;
		}
		else	   //如果是英文
		{
			if((*p=='\n')||(x>224))   // 换段和换行
			{
				y=y+19;   //字体高16 行间距3
				x=2;      //2是边距
			}
			LCD_ShowChar(x,y,*p);
			x+=8;
			p++;
		}
    }  
}

  


