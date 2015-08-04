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
��Ŀ����������2014��QC ��ȡ�ϻ�����ͨ��ģ�����
���ߣ�xinlan
ʱ��: 2014-1-22
*******************************************************************************/

volatile unsigned char   E13_Read_IO	;              //E13��ȡ�������ݣ������ڴ����жϺ�����
unsigned char  E13_Init_Succes;						 //E13���óɹ�,������E13���ú���
unsigned char  Communication_Mark =13;			//E13ͨ�ű�־
u16 data_error;													 //ͨ�Ŵ���
u16 communication_over_time ;								 //ͨ�ų�ʱ
u16 communication_miss ;								 //ͨ�ų�ʱ
u16 power_up=0;													//�ϵ����
u8 flash_write_mark=1;									//дflash��־

int main(void)
{
  //SYS_INIT_OK=0;
	//SYSTICK_INIT();
	//SYS_INIT_OK=1;
	//uint32_t While1_Lasttime=0;//�洢whileѭ����ʱ��
	char clear_flash=0;

	u8 volatile Input_0=0;
	u8 volatile Input_1=0;
	u8 volatile Input_2=0;
	u8 volatile Input_3=0;
	
	u8 volatile Ouput_0=0;
	u8 volatile Ouput_1=0;
	u8 volatile Ouput_2=0;
	u8 volatile Ouput_3=0;
	
  Bsp_Init();                 //��ʼ������IO�ӿںʹ���
	power_up=power_up+1;
	Write_Flash(0x801f000,Read_Flash(0x801F000)+1);         //0x801f000��ʼ�ĵ�1kΪ��������
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
				Write_Relay_3(Bit_RESET);     //�Ͽ��̵���
				Communication_Mark=0;
				
				//�ղ����ź���Ҫ����Ƶ����
						
					Write_Flash(0x801f800,Read_Flash(0x801F800)+1);         //0x801f800��ʼ�ĵ�1kΪͨ�ų�ʱ�洢��
					if(flash_write_mark==1)
						{
							communication_over_time =communication_over_time+1;
							flash_write_mark=0;
						}
			}
		
		if (Communication_Mark==11)
		{
					Write_Flash(0x801f400,Read_Flash(0x801F400)+1);         //0x801f400��ʼ�ĵ�1kΪͨ�Ŷ�ʧ�洢��
				communication_miss =communication_miss+1;
		}
		if(Communication_Mark!=0)
			Communication_Mark--;

		//Uart2_PutChar(Communication_Mark);
		//Uart2_PutChar(0X77);
		//Uart2_PutString("\n���ͨ�Ŷ�ʧ--��ʧһ������",13);
		}
			//���flash 
		switch(clear_flash)
		{
			case 1:
						Write_Flash(0x801f400,0);                          //���ͨ�Ŷ�ʧ--��ʧһ������
						break;
			case 2:
						Write_Flash(0x801f800,0);                          //���ͨ�Ŷ�ʧ-��ʱ1040s
						break;
			case 3:
						Write_Flash(0x801fc00,0);                          //���ͨ�����ݴ���
						break;
			case 4:
						Write_Flash(0x801f000,0);                         //�ϵ�����
						break;			
		}

	
}


