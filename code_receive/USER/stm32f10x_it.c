/**
  ******************************************************************************
  * @file    USART/Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "e13_tll.h"
#include "io.h"
#include "DRIVERS\flash.h"
#include "DRIVERS\uart.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Interrupt
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern  u8 recevie_Buffer[]; 																					// 发送缓冲区。
extern  u8 recevie_Buffer_BufferHead; 																// 接收缓冲区的头索引。指向下一个空闲单元。
extern  u8 recevie_Buffer_BufferTail; 																// 接收缓冲区的尾索引。指向最后一个有效单元。
extern  unsigned char  E13_Read_IO;																		//读取到的E13数据
extern  unsigned char  E13_Init_Succes;																//E13配置成功
extern  unsigned char  Communication_Mark;																//E13配置成功
extern	u16 data_error;													 //通信错误
extern	u16 communication_over_time ;								 //通信超时
extern	u16 communication_miss ;	
extern	u8 flash_write_mark;									//写flash标志
extern  u16 power_up ;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
static volatile u8 recevie_Buffer[SERIAL_RECEIVE_BUFFER_SIZE]; // 发送缓冲区。
static volatile u8 recevie_Buffer_BufferHead = 0; // 接收缓冲区的头索引。指向下一个空闲单元。
static volatile u8 recevie_Buffer_BufferTail = 0; // 接收缓冲区的尾索引。指向最后一个有效单元。
  */
void USART1_IRQHandler(void)
{
	u16 temp=0;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    recevie_Buffer[recevie_Buffer_BufferHead++] = USART_ReceiveData(USART1);
		if(E13_Init_Succes==0) 
			return;
		if((recevie_Buffer[0]==0x99) & (E13_Init_Succes==1))
				{
				if( (recevie_Buffer_BufferHead==4) & (E13_Init_Succes==1) )//接收到一包完整的数据
					{
							USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);	 //接收中断模式允许
							//调用E13读取函数
							E13_Read_IO =E13_Receive_Data( recevie_Buffer,recevie_Buffer_BufferHead);
							recevie_Buffer_BufferHead=0;
							recevie_Buffer[0] =0;
							recevie_Buffer[1] =0;
							recevie_Buffer[2] =0;
							recevie_Buffer[3] =0;
								
							switch  (E13_Read_IO)
							{
								case 0x55: 
									Write_Out_0(Bit_RESET);
									Write_Relay_3(Bit_RESET);
									Communication_Mark=13;							//13×80=1040ms，即延时1040ms
								  flash_write_mark=1;  								//通信恢复正常，发生错误的时候可以在此写flash
									break;
								case 0xaa:
									Write_Out_0(Bit_SET );
									Write_Relay_3(Bit_SET);
									Communication_Mark=13;	
								  flash_write_mark=1;  								//通信恢复正常，发生错误的时候可以在此写flash
									break;
								default:
									//接收到一包错误的数据，通信失败
									{
										//Communication_Mark =Communication_Mark-1; 
										temp=Read_Flash(0x801FC00);
										Write_Flash(0x801fc00,temp+1);         //0x801fc00开始的的1k为通信错误存储区	
										data_error =data_error+1;										
									}					
									
							}				
							USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	 //接收中断模式允许
					}
				//清除收到的数据
				//recevie_Buffer[0] =0;
				if(recevie_Buffer_BufferHead == 255)
					recevie_Buffer_BufferHead=0;
				}
			else  
			{
				recevie_Buffer_BufferHead=0;
			}
	}
}

/**
  * @brief  This function handles USARTz global interrupt request.
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)
{
		u8 temp=0;
	  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
			temp=USART_ReceiveData(USART2);
			switch(temp)
		{
		case 1: 
			Uart2_PutChar( communication_miss&0xff);
		  break;
		case 2:
			Uart2_PutChar((u8)communication_over_time&0xff);
		  break;
		case 3:
			Uart2_PutChar((u8)data_error&0xff);
		  break;
		case 4:
			Uart2_PutChar((u8)power_up&0xff);
		  break;
		case 5:
			Uart2_PutChar(Communication_Mark);
		  break;
		case 6:
			Uart2_PutChar(6);
		  break;
		case 0:
		  break;
		case 7:
			Write_Flash(0x801f400,0);                          //清除通信丢失--丢失一包数据
		  Uart2_PutChar(0X77);
		  communication_miss=0;
			break;
		case 8:
			Write_Flash(0x801f800,0);                          //清除通信丢失-延时1040s
			Uart2_PutChar(0X88);       
		communication_over_time=0;
			break;
		case 9:
			Write_Flash(0x801fc00,0);                          //清除通信数据错误
			Uart2_PutChar(0X99);
		  data_error=0;
			break;
		case 10:
			Write_Flash(0x801f000,0);                          //清除上电次数
			Uart2_PutChar(0XAA);
		  power_up=0;
			break;
		default:
			Uart2_PutString("unknow command",14);

		}
		
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
