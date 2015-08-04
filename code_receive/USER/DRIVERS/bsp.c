/*******************************************************************************
功能：BSP初始化函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

#include "stm32f10x.h"
#include "DRIVERS/led.h"
#include "DRIVERS/uart.h"
#include "DRIVERS/io.h"
#include "DRIVERS/bsp.h"
#include "DRIVERS/e13_tll.h"
#include "DRIVERS/flash.h"

extern  u16 data_error;																		//读取到的E13数据
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
    //初始化LED灯
		 led_init_com();
		 led_init_io();
	
    //初始化串口
	   UART1_NVIC_Configuration();
     UART1_Configuration();
     UART1_GPIO_Configuration();
	
		 UART2_NVIC_Configuration();
     UART2_Configuration();
     UART2_GPIO_Configuration();
		 //清除标志位，否则第一位会丢失
		 USART_ClearFlag(USART1,USART_FLAG_TC);
		 USART_ClearFlag(USART2,USART_FLAG_TC);
	
		 //初始化输入输出接口
	   Io_Init();
		 
		 //初始化E13_TLL模块
		 /***
		 E13_Io_Init();

		 
		 //配置E13_TLL模块
		 if (E13_Configuration()==1)            //E13模块配置失效，闪烁发送和接受LED灯，进入死循环 
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
		 led_on_in_3();                  //点亮IN3作为电源指示灯
		 
		 //初始化flash
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

/**************************实现函数********************************************
*函数原型:		
*功　　能:		
*******************************************************************************/
void SYSTICK_INIT(void)
{
	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL = 0x0;
	SysTick->CTRL = 0x01;//启用systick,8分频(9M),关中断,本寄存器第四位为重载标志,=1说明重载了,读取本寄存器第四位自动清零
	
}
uint32_t GET_NOWTIME(uint32_t * lasttime)//返回当前systick计数器值,32位
{
	uint32_t temp,temp1,temp2;
	
	temp1 = SysTick->VAL;
	temp = SysTick->CTRL;
	if(temp&(1<<16)) 
		temp2 = *lasttime + 0xffffff - temp1;//发生重载
	else
		temp2 = *lasttime - temp1;
	*lasttime = temp1;
	if(temp2>100000)	return 0;
	return temp2;
}

void RCC_Configuration(void)
{
     /* 定义枚举型变量HSEStartUpStatus */
    ErrorStatus HSEStartUpStatus;
    /* 复位系统时钟设置*/
    RCC_DeInit();
    /* 开启HSE*/
    RCC_HSEConfig(RCC_HSE_ON);
    /* 等待HSE启振并稳定*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    /*判断HSE是否起振成功, */
    if(HSEStartUpStatus == SUCCESS)
    {
        /* 选择HCLK(AHB)时钟源为 SYSCLK 1分频 */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        /* 选择PCLK2时钟源为HCLK(AHB) 1分频*/
        RCC_PCLK2Config(RCC_HCLK_Div1);
        /* 选择PCLK1时钟源为HCLK(AHB) 2分频*/
        RCC_PCLK1Config(RCC_HCLK_Div2);
        /* 设置FLASH延时周期数为2 */
        FLASH_SetLatency(FLASH_Latency_2);
        /* 使能FLASH预缓存 */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* 选择锁相环(PLL)时钟源为HSE 1分频,倍频数为9,则PLL输出频率 8MHz * 9 = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        /* 使能PLL */
        RCC_PLLCmd(ENABLE);
        /* 等待PLL输出稳定 */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        /* 选择SYSCLK时钟源为PLL */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        /* 等待PLL成为SYSCLK时钟源*/
        while(RCC_GetSYSCLKSource() != 0x08);
    }

    /* 打开APB2总线上的GPIOA时钟*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|
													 RCC_APB2Periph_AFIO  |RCC_APB2Periph_USART1 ,ENABLE);       
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_TIM4,ENABLE);
		

}

