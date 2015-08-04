/*******************************************************************************
���ܣ�BSP��ʼ������
���ߣ�xinlan
ʱ��: 2014-1-22
*******************************************************************************/

#include "stm32f10x.h"
#include "DRIVERS/led.h"
#include "DRIVERS/uart.h"
#include "DRIVERS/io.h"
#include "DRIVERS/bsp.h"
#include "DRIVERS/e13_tll.h"
#include "DRIVERS/flash.h"

extern  u16 data_error;																		//��ȡ����E13����
extern  u16 communication_over_time ;	
extern  u16 communication_miss ;
extern  u16 power_up ;
void RCC_Configuration(void);

void Bsp_Init(void)
{
   RCC_Configuration();
		/*********************************************************************************************/
    //
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    //��ʼ��LED��
		 led_init_com();
		 led_init_io();
	
    //��ʼ������
	   UART1_NVIC_Configuration();
     UART1_Configuration();
     UART1_GPIO_Configuration();
	
		 UART2_NVIC_Configuration();
     UART2_Configuration();
     UART2_GPIO_Configuration();
		 //�����־λ�������һλ�ᶪʧ
		 USART_ClearFlag(USART1,USART_FLAG_TC);
		 USART_ClearFlag(USART2,USART_FLAG_TC);
	
		 //��ʼ����������ӿ�
	   Io_Init();
		 
		 //��ʼ��E13_TLLģ��
		 /***
		 E13_Io_Init();

		 
		 //����E13_TLLģ��
		 if (E13_Configuration()==1)            //E13ģ������ʧЧ����˸���ͺͽ���LED�ƣ�������ѭ�� 
		 {
			 while(1)
			 {
			 Delay_Ms(25);
			 led_on_com_r();
				 led_on_com_t();
			 Delay_Ms(25);
				 led_off_r();
				 led_off_t();

			 }
		 }
		 *///////////////
		 led_on_in_3();                  //����IN3��Ϊ��Դָʾ��
		 
		 //��ʼ��flash
		 data_error =Read_Flash(0x0801fc00);
		 communication_over_time =Read_Flash(0x0801f800);
		 communication_miss =Read_Flash(0x0801f400);
		 power_up=Read_Flash(0x0801f000);
		 //Write_Flash(0x0801fc00,0);
		 //Write_Flash(0x0801f800,0);
}

static __IO uint32_t delay_ms_count = 0;

void Delay_Ms(uint32_t ms)
{
    delay_ms_count = ms*500;
    while(delay_ms_count)
		{
		delay_ms_count--;
		}
}

void Delay_Us(uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 105);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		
*��������:		
*******************************************************************************/
void SYSTICK_INIT(void)
{
	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL = 0x0;
	SysTick->CTRL = 0x01;//����systick,8��Ƶ(9M),���ж�,���Ĵ�������λΪ���ر�־,=1˵��������,��ȡ���Ĵ�������λ�Զ�����
	
}
uint32_t GET_NOWTIME(uint32_t * lasttime)//���ص�ǰsystick������ֵ,32λ
{
	uint32_t temp,temp1,temp2;
	
	temp1 = SysTick->VAL;
	temp = SysTick->CTRL;
	if(temp&(1<<16)) 
		temp2 = *lasttime + 0xffffff - temp1;//��������
	else
		temp2 = *lasttime - temp1;
	*lasttime = temp1;
	if(temp2>100000)	return 0;
	return temp2;
}

void RCC_Configuration(void)
{
     /* ����ö���ͱ���HSEStartUpStatus */
    ErrorStatus HSEStartUpStatus;
    /* ��λϵͳʱ������*/
    RCC_DeInit();
    /* ����HSE*/
    RCC_HSEConfig(RCC_HSE_ON);
    /* �ȴ�HSE�����ȶ�*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    /*�ж�HSE�Ƿ�����ɹ�, */
    if(HSEStartUpStatus == SUCCESS)
    {
        /* ѡ��HCLK(AHB)ʱ��ԴΪ SYSCLK 1��Ƶ */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        /* ѡ��PCLK2ʱ��ԴΪHCLK(AHB) 1��Ƶ*/
        RCC_PCLK2Config(RCC_HCLK_Div1);
        /* ѡ��PCLK1ʱ��ԴΪHCLK(AHB) 2��Ƶ*/
        RCC_PCLK1Config(RCC_HCLK_Div2);
        /* ����FLASH��ʱ������Ϊ2 */
        FLASH_SetLatency(FLASH_Latency_2);
        /* ʹ��FLASHԤ���� */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* ѡ�����໷(PLL)ʱ��ԴΪHSE 1��Ƶ,��Ƶ��Ϊ9,��PLL���Ƶ�� 8MHz * 9 = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        /* ʹ��PLL */
        RCC_PLLCmd(ENABLE);
        /* �ȴ�PLL����ȶ� */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        /* ѡ��SYSCLKʱ��ԴΪPLL */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        /* �ȴ�PLL��ΪSYSCLKʱ��Դ*/
        while(RCC_GetSYSCLKSource() != 0x08);
    }

    /* ��APB2�����ϵ�GPIOAʱ��*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|
													 RCC_APB2Periph_AFIO  |RCC_APB2Periph_USART1 ,ENABLE);       
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_TIM4,ENABLE);
		

}

