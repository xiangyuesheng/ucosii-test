#include "stm32f0xx.h"
#include "ucos_ii.h"
#include "led.h"


/* ˽�����Ͷ���---------------------------------------------------------------*/
/* ˽�ж��� ------------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define LED3            GPIO_Pin_9
#define LED4            GPIO_Pin_8

#define LED_PORT        GPIOC
#define LED_GPIO_CLK    RCC_AHBPeriph_GPIOC


/* ���� ----------------------------------------------------------------------*/
static   OS_STK   App_Task_LED1_Stk[APP_TASK_LED1_STK_SIZE];
static   OS_STK   App_Task_LED2_Stk[APP_TASK_LED2_STK_SIZE];

/* ������ ------------------------------------------------------------------*/
static  void  App_Task_LED1(void* p_arg);
static  void  App_Task_LED2(void* p_arg);
void Delay(__IO uint32_t nCount);

/***************************************************************************//**
  * @brief  ��������Ӳ����ʼ����ʵ��LED1-LED4��˸
  * @note   ��
  * @param  ��
  * @retval ��
*******************************************************************************/
int main(void)
{
    INT8U os_err;
    LED_Init();
    OSInit();
    OS_CPU_SysTickInit();
    
    //����LED1��˸������
    os_err = OSTaskCreate( App_Task_LED1,
                          (void *) 0,
                          (OS_STK *) &App_Task_LED1_Stk[APP_TASK_LED1_STK_SIZE - 1],
                          (INT8U) APP_TASK_LED1_PRIO);

    //����LED2��˸������
    os_err = OSTaskCreate( App_Task_LED2,
                          (void*) 0,
                          (OS_STK*) &App_Task_LED2_Stk[APP_TASK_LED2_STK_SIZE - 1],
                          (INT8U ) APP_TASK_LED2_PRIO);
    
    os_err = os_err;//����������������δʹ�õı��뾯��
    
    //����ucOS ����ϵͳ
    OSStart ();
}



/*******************************************************************************
  * @��������	App_Task_LED1
  * @����˵��   LED����1
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
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
  * @��������	App_Task_LED2
  * @����˵��   LED����2
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
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
  * @brief  ����һ����ʱʱ��
  * @note   ��
  * @param  nCount��ָ����ʱ��ʱ�䳤��
  * @retval ��
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    int i,j;
    //����ѭ������ʱһ����ʱ��
    for (i=0; i<nCount; i++)
        for (j=0; j<5000; j++)
            ;
}

#ifdef  USE_FULL_ASSERT
/***************************************************************************//**
  * @brief  �����ڼ�������������ʱ��Դ�ļ����ʹ�������
  * @param  file: ָ������ļ���Դ�ļ���
  * @param  line: �����Դ������������
  * @retval ��
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* �û����������Լ��Ĵ������ڱ��������ļ�������������,
       ���磺printf("�������ֵ: �ļ��� %s �� %d��\r\n", file, line) */

    /* ��ѭ�� */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT wuguoyana ***************�ļ�����***********/
