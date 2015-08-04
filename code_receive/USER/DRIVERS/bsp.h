/*******************************************************************************
功能：BSP初始化函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/
#ifndef __FILE_DRIVERS_BSP_H__
#define __FILE_DRIVERS_BSP_H__

void Bsp_Init(void);
void Delay_Ms(uint32_t ms);
void Delay_Us(uint32_t usec);
void SYSTICK_INIT(void);

#endif // __FILE_BSP_BSP_H__
