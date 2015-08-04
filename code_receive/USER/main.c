#include "stm32f10x.h"

#include <stdio.h> 
#include "DRIVERS/bsp.h"
#include "DRIVERS\io.h"
#include "e13_tll.h"
#include "DRIVERS\led.h"
#include "DRIVERS\bsp.h"
#include "DRIVERS\flash.h"
#include "DRIVERS\uart.h"
#include "DRIVERS\pwm.h"

/*******************************************************************************
项目：电气车间2014年QC 混取料机无线通信模块设计
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

volatile unsigned char   E13_Read_IO	;              //E13读取到的数据，来自于串口中断函数中
unsigned char  E13_Init_Succes;						 //E13配置成功,来自于E13配置函数
unsigned char  Communication_Mark =13;			//E13通信标志
u16 data_error;													 //通信错误
u16 communication_over_time ;								 //通信超时
u16 communication_miss ;								 //通信超时
u16 power_up=0;													//上电次数
u8 flash_write_mark=1;									//写flash标志

int main(void)
{
  //SYS_INIT_OK=0;
	//SYSTICK_INIT();
	//SYS_INIT_OK=1;
	//uint32_t While1_Lasttime=0;//存储while循环的时间
	char clear_flash=0;

	u8 volatile Input_0=0;
	u8 volatile Input_1=0;
	u8 volatile Input_2=0;
	u8 volatile Input_3=0;
	
	u8 volatile Ouput_0=0;
	u8 volatile Ouput_1=0;
	u8 volatile Ouput_2=0;
	u8 volatile Ouput_3=0;
	
  Bsp_Init();                 //初始化所有IO接口和串口
	power_up=power_up+1;
	Write_Flash(0x801f000,Read_Flash(0x801F000)+1);         //0x801f000开始的的1k为启动次数
  while(1)
  {
		Input_0=In_0;
		Input_1=In_1;
		Input_2=In_2;
		Input_3=In_3;

		Delay_Ms(80);
		if (Communication_Mark==0)
			{
				Write_Out_0(Bit_SET);
				Write_Relay_3(Bit_RESET);     //断开继电器
				Communication_Mark=0;
				
				//收不到信号需要做跳频处理
						
					Write_Flash(0x801f800,Read_Flash(0x801F800)+1);         //0x801f800开始的的1k为通信超时存储区
					if(flash_write_mark==1)
						{
							communication_over_time =communication_over_time+1;
							flash_write_mark=0;
						}
			}
		
		if (Communication_Mark==11)
		{
					Write_Flash(0x801f400,Read_Flash(0x801F400)+1);         //0x801f400开始的的1k为通信丢失存储区
				communication_miss =communication_miss+1;
		}
		if(Communication_Mark!=0)
			Communication_Mark--;

		//Uart2_PutChar(Communication_Mark);
		//Uart2_PutChar(0X77);
		//Uart2_PutString("\n清除通信丢失--丢失一包数据",13);
		}
			//清除flash 
		switch(clear_flash)
		{
			case 1:
						Write_Flash(0x801f400,0);                          //清除通信丢失--丢失一包数据
						break;
			case 2:
						Write_Flash(0x801f800,0);                          //清除通信丢失-延时1040s
						break;
			case 3:
						Write_Flash(0x801fc00,0);                          //清除通信数据错误
						break;
			case 4:
						Write_Flash(0x801f000,0);                         //上电重启
						break;			
		}

	
}


