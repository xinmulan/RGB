/*******************************************************************************
功能：LED驱动函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

#include "stm32f10x.h"

void led_init_com(void);
void led_init_io(void);
void led_on_com_r(void);
void led_on_com_t(void);
void led_off_r(void);
void led_off_t(void);
void led_reverse_r(void);
void led_reverse_t(void);
void led_on_in_0(void);
void led_on_in_1(void);
void led_on_in_2(void);
void led_on_in_3(void);
void led_off_in_0(void);
void led_off_in_1(void);
void led_off_in_2(void);
void led_off_in_3(void);
void led_on_out_0(void);
void led_on_out_1(void);
void led_on_out_2(void);
void led_on_out_3(void);
void led_off_out_0(void);
void led_off_out_1(void);
void led_off_out_2(void);
void led_off_out_3(void);

void led_init_io(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;         //GPIO_Pin_4=IN_0,GPIO_Pin_5=IN_1,GPIO_Pin_6=IN_2,GPIO_Pin_7=IN_3,
    GPIO_Init(GPIOA,&GPIO_InitStructure);     //初始化输入指示LED灯												
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;					//GPIO_Pin_4=OUT_0,GPIO_Pin_5=OUT_1,GPIO_Pin_6=OUT_2,GPIO_Pin_7=OUT_3,
	  GPIO_Init(GPIOB,&GPIO_InitStructure);     //初始化输出指示LED灯
    GPIO_SetBits(GPIOA,GPIO_InitStructure.GPIO_Pin);							//点亮所有的LED输入指示灯
		GPIO_SetBits(GPIOB,GPIO_InitStructure.GPIO_Pin);							//点亮所有的LED输出指示灯
}

void led_init_com(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;    //com_t
    GPIO_Init(GPIOA,&GPIO_InitStructure);     //初始化通讯指示LED灯
    GPIO_SetBits(GPIOA,GPIO_InitStructure.GPIO_Pin);							//点亮所有的LED通迅指示灯
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    //com_r
    GPIO_Init(GPIOB,&GPIO_InitStructure);     //初始化通讯指示LED灯
    GPIO_SetBits(GPIOB,GPIO_InitStructure.GPIO_Pin);							//点亮所有的LED通迅指示灯
}

void led_on_com_r(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void led_on_com_t(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

void led_off_r(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
}

void led_off_t(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void led_reverse_r(void)
{
    GPIOB->ODR ^= GPIO_Pin_3;
}

void led_reverse_t(void)
{
    GPIOA->ODR ^= GPIO_Pin_1;
}

void led_on_in_0(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void led_on_in_1(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

void led_on_in_2(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

void led_on_in_3(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

void led_off_in_0(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void led_off_in_1(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void led_off_in_2(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

void led_off_in_3(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

void led_on_out_0(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
}

void led_on_out_1(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
}

void led_on_out_2(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

void led_on_out_3(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}

void led_off_out_0(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_4);
}

void led_off_out_1(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_5);
}

void led_off_out_2(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

void led_off_out_3(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
