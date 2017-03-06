#include "stm32f0xx.h"
#include "ucos_ii.h"
#include "led.h"


/* 私有类型定义---------------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define LED3            GPIO_Pin_9
#define LED4            GPIO_Pin_8

#define LED_PORT        GPIOC
#define LED_GPIO_CLK    RCC_AHBPeriph_GPIOC


/* 变量 ----------------------------------------------------------------------*/
static   OS_STK   App_Task_LED1_Stk[APP_TASK_LED1_STK_SIZE];
static   OS_STK   App_Task_LED2_Stk[APP_TASK_LED2_STK_SIZE];

/* 任务函数 ------------------------------------------------------------------*/
static  void  App_Task_LED1(void* p_arg);
static  void  App_Task_LED2(void* p_arg);
void Delay(__IO uint32_t nCount);

/***************************************************************************//**
  * @brief  主函数，硬件初始化，实现LED1-LED4闪烁
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
int main(void)
{
    INT8U os_err;
    LED_Init();
    OSInit();
    OS_CPU_SysTickInit();
    
    //创建LED1闪烁的任务
    os_err = OSTaskCreate( App_Task_LED1,
                          (void *) 0,
                          (OS_STK *) &App_Task_LED1_Stk[APP_TASK_LED1_STK_SIZE - 1],
                          (INT8U) APP_TASK_LED1_PRIO);

    //创建LED2闪烁的任务
    os_err = OSTaskCreate( App_Task_LED2,
                          (void*) 0,
                          (OS_STK*) &App_Task_LED2_Stk[APP_TASK_LED2_STK_SIZE - 1],
                          (INT8U ) APP_TASK_LED2_PRIO);
    
    os_err = os_err;//仅仅是清除这个变量未使用的编译警告
    
    //启动ucOS 操作系统
    OSStart ();
}



/*******************************************************************************
  * @函数名称	App_Task_LED1
  * @函数说明   LED任务1
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void App_Task_LED1(void* pdata)
{
    pdata = pdata;

    for (;;)
    {
         LED1_Open();;
        OSTimeDlyHMSM(0, 0, 0, 500);
         LED1_Close();
        OSTimeDlyHMSM(0, 0, 0, 500);
    }

}

/*******************************************************************************
  * @函数名称	App_Task_LED2
  * @函数说明   LED任务2
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void App_Task_LED2(void* pdata)
{
    pdata = pdata;

    for (;;)
    {
        LED2_Open();
        OSTimeDly(100);
        LED2_Close();
        OSTimeDly(100);
    }
}

/***************************************************************************//**
  * @brief  插入一段延时时间
  * @note   无
  * @param  nCount：指定延时的时间长度
  * @retval 无
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    int i,j;
    //利用循环来延时一定的时间
    for (i=0; i<nCount; i++)
        for (j=0; j<5000; j++)
            ;
}

#ifdef  USE_FULL_ASSERT
/***************************************************************************//**
  * @brief  报告在检查参数发生错误时的源文件名和错误行数
  * @param  file: 指向错误文件的源文件名
  * @param  line: 错误的源代码所在行数
  * @retval 无
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    /* 死循环 */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT wuguoyana ***************文件结束***********/
