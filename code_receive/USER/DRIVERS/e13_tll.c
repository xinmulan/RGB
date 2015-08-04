/*******************************************************************************
���ܣ�E13-TLL���ߴ�����������
���ߣ�xinlan
ʱ��: 2014-1-22
*******************************************************************************/

#include "stm32f10x.h"
#include "DRIVERS/e13_tll.h"
#include "DRIVERS/bsp.h"
#include "DRIVERS\uart.h"
#include "DRIVERS\led.h"

extern u8 recevie_Buffer[];
extern u8 recevie_Buffer_BufferHead;
extern u8 recevie_Buffer_BufferTail;

u8 E13_Config_Data[] ={0xc0,0x0,0x1,0x1C,0xFA,0x7f};   //ģ�鹤����450MHZ,��ַΪ1�������ٶ�9600BPS,���߿�������8KBSP,���书��20dBm
//u8 E13_Config_Data[] ={0xc0,0x0,0x1,0x1B,0xFA,0x7f};   //ģ�鹤����450MHZ,��ַΪ1�������ٶ�9600BPS,���߿�������6KBSP,���书��20dBm

void  E13_Io_Init(void)
{
		//��ʼ������AUX
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;         //GPIO_Pin_13=AUX, PWR.7=0:�������ָʾ�����ڻ��ѺͿ����ⲿ�豸
																											 // PWR.7=0:���߷�æָʾ����֪ͨʱ��������ݵķ���
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	  //��ʼ�����SET
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ; 				//GPIO_Pin_12=SET  SET=0Ϊ����ģ�飬SET=1Ϊ͸��ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

u8 E13_Configuration(void)
{
	int i=0;
	for(i=0;i<255;i++)																 //��ջ�����
		{
			recevie_Buffer[i] =0;
		}
	Write_Set( Bit_RESET);                             //��SET�������ͣ��������ģʽ
	Delay_Ms(100);																		 //��ʱ100ms
	if(!(recevie_Buffer[0]=='S'& recevie_Buffer[1]=='T'& recevie_Buffer[2]=='A'& recevie_Buffer[3]=='R' & recevie_Buffer[4]=='T'))
	{
		return 1;                                       //û�н��յ�START�ַ����Ƴ�����
	}
	i=5;
	Uart1_PutString(E13_Config_Data,6);               //�������ò��� 
	Delay_Ms(500);
	if((recevie_Buffer[i]=='O'& recevie_Buffer[i+1]=='K'))
	{
		recevie_Buffer_BufferHead =0;
		recevie_Buffer_BufferTail =0;	
			for(i=0;i<255;i++)																 //��ջ�����
			{
				recevie_Buffer[i] =0;
			}
		Write_Set( Bit_SET);                             //��SET�������ߣ�����͸��ģʽ	
		E13_Init_Succes =1;	
		return 0;                                       //�������óɹ�
		
	}
	if((recevie_Buffer[i]=='E'& recevie_Buffer[i+1]=='R'& recevie_Buffer[i+2]=='R'& recevie_Buffer[i+2]=='O' &recevie_Buffer[3]=='R'))
	{
		return 1;                                       //�������ô���
	}	

	return 1; 
}

u8  Read_Aux(void)
{
	return  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}

void  Write_Set(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, data);
}

/*******************************************************************************
���ܣC����ģ�鷢�ͺ���
���ߣ�xinlan
ʱ��: 2014-2-17
*******************************************************************************/
int El3_Send_Char(char data)
{
	unsigned char Send_Buf[4];
	unsigned char  *p =&Send_Buf[0];
	Send_Buf[0] =0x99;
	Send_Buf[1] =0x77;
	Send_Buf[2] =data;
	Send_Buf[3]=crc16(p ,3);
	serial_transmit(p,4);
	return 0;
}

/*******************************************************************************
���ܣC��E13����һ������
���ߣ�xinlan
ʱ��: 2014-3-3
˵�����ݸ�ʽ 0x99,0x77,data ,crc ;
*******************************************************************************/
char E13_Receive_Data(u8 data[],u8 recevie_Buffer_Piont)
{
	unsigned char Temp_Data[4]={0,0,0,0};
	Temp_Data[0]=0x99;
	Temp_Data[1]=0x77;
	Temp_Data[2]=data[recevie_Buffer_Piont-2];
	if(data[recevie_Buffer_Piont-4]==0x99 & data[recevie_Buffer_Piont-3]==0x77 )
		Temp_Data[3]=crc16(Temp_Data,3);
		if(Temp_Data[3] ==data[recevie_Buffer_Piont-1] )
			{
			led_reverse_r();
			led_reverse_t();
			return Temp_Data[2];
			}
	else
		return 0;
}

/*******************************************************************************
���ܣCCRC16У�麯��
���ߣ�xinlan
ʱ��: 2014-2-17
*******************************************************************************/
char  crc16(uint8_t *msg_buf, int8_t nbytes)
{
	uint8_t crc_reg=0xff, poly,byte_count,bit_count;
	uint8_t *byte_point;
	uint8_t bit_point;

	for (byte_count=0, byte_point=msg_buf; byte_count<nbytes; ++byte_count, ++byte_point)
	{
		for (bit_count=0, bit_point=0x80 ; bit_count<8; ++bit_count, bit_point>>=1)
		{
			if (bit_point & *byte_point)							// case for new bit = 1
			{
				if (crc_reg & 0x80)
					poly=1;											// define the polynomial
				else
					poly=0x1c;
				crc_reg= ( (crc_reg << 1) | 1) ^ poly;
			}
			else										 
			// case for new bit = 0
			{
				poly=0;
				if (crc_reg & 0x80)
					poly=0x1d;
				crc_reg= (crc_reg << 1) ^ poly;
			}
		}
	}
	return ~crc_reg;											   // Return CRC
} 




