/*******************************************************************************
功能：IO驱动函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

#include "stm32f10x.h"

void  Io_Init(void)
{
		//初始化输入IO
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_10;         //GPIO_Pin_0=IN_0,GPIO_Pin_1=IN_2,GPIO_Pin_2=IN_1,GPIO_Pin_10=IN_3,
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	  //初始化输出IO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 				//GPIO_Pin_15=OUT_0,
		//GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |		GPIO_Pin_6=OUT_1,GPIO_Pin_5=OUT_2,GPIO_Pin_4=OUT_3,																																																																			//GPIO_Pin_15=Relay_0,GPIO_Pin_14=Relay_1,GPIO_Pin_13=Relay_2,GPIO_Pin_12=Relay_3,
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;     //GPIO_Pin_8=OUT_1,GPIO_Pin_11=OUT_2,GPIO_Pin_12=OUT_3,
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);
}

u8  Read_In_0(void)
{
	return  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
}

u8  Read_In_1(void)
{
	return  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
}

u8  Read_In_2(void)
{
	return  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
}

u8  Read_In_3(void)
{
	return  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
}

void  Write_Out_0(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, data);
}

void  Write_Out_1(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, data);
}

void  Write_Out_2(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, data);
}

void  Write_Out_3(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, data);
}

void  Write_Relay_0(BitAction data)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, data);
}

void  Write_Relay_1(BitAction data)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, data);
}

void  Write_Relay_2(BitAction data)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, data);
}

void  Write_Relay_3(BitAction data)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, data);
}

