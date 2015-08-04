#include "stm32f10x.h"




void Time3_PWM_Init(void)
{
	Time4_GPIO_Config();
	Time4_Mode_Config();
}

static void TIM4_Mode_Config(void)   
  {   
      TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
      TIM_OCInitTypeDef  TIM_OCInitStructure;   
    
      /* PWM 信号电平跳变值 */  
      u16 CCR1_Val = 500;           
      u16 CCR2_Val = 375;   
      u16 CCR3_Val = 250;   
      u16 CCR4_Val = 125;   
    
  /* -----------------------------------------------------------------------  
      TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles:  
      TIM4 CLK = 36 MHz, Prescaler = 0x0, TIM4 counter clock = 36 MHz  
      TIM4 ARR Register = 999 => TIM4 Frequency = TIM4 counter clock/(ARR + 1)  
      TIM4 Frequency = 36 KHz.  
      TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%  
      TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%  
      TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 25%  
      TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 12.5%  
    ----------------------------------------------------------------------- */  
    
    /* Time base configuration */
   //当定时器从0计数到999即为1000次，为一个定时周期          
    TIM_TimeBaseStructure.TIM_Period = 999;
          
    TIM_TimeBaseStructure.TIM_Prescaler = 0;              //不预分频，即为36MHz 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //设置时钟分频系数:为0   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上技术模式
    IM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);   
    
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //配置为PWM模式1   
		
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;      //设置跳变值，当计数器的值到到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //当计数器的值小于 CCR1_Val 为高电平   
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);   //使能1通道   
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);   
    
    /* PWM1 Mode configuration: Channel2 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;       
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);   
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);   
    
    /* PWM1 Mode configuration: Channel3 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;     
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);   
    
    /* PWM1 Mode configuration: Channel4 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;   
    TIM_OC4Init(TIM4, &TIM_OCInitStructure); 
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);   
    TIM_ARRPreloadConfig(TIM4, ENABLE);   
    
    /* TIM4 enable counter */  
    TIM_Cmd(TIM4, ENABLE);   //使能定时器4    
  }

Time3_GPIO_Config()
{
	void GPIO_Cfg(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;  //D1 D2
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure);       
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_13;      //D3, D4
GPIO_Init(GPIOD, &GPIO_InitStructure); 
}
}
