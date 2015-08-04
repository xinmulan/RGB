void GPIO_Configuration(void)  
{  
    
  GPIO_InitTypeDef GPIO_InitStructure;        //定义GPIO初始化结构体  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复合推挽输出     
  GPIO_Init(GPIOA, &GPIO_InitStructure);       //PA9串口输出  
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   //红外接收  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //配置成上拉输入;   
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
    
}



void NVIC_Configuration(void)    //中断分组和优先级配置    详见《STM32的函数说明(中文).pdf》P165  
{  
    NVIC_InitTypeDef NVIC_InitStructure;  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  //0组,先占优先级0位,从优先级4位  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //外部中断线 9_5 中断(5-9号引脚是统一使用中断线EXTI9_5_IRQn)  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //0组无抢占优先级,所以只配置从优先级,即响应式优先级  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能  
    NVIC_Init(& NVIC_InitStructure); //初始化配置  
}  
  
void EXTI_Configuration(void)  //中断配置  //详见《STM32的函数说明(中文).pdf》   P99  
{  
    EXTI_InitTypeDef EXTI_InitStructure;          
  
    EXTI_ClearITPendingBit(EXTI_Line8);//清除 EXTI 线路挂起位  
  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设置 EXTI 线路为中断请求  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设置输入线路下降沿为中断请求  
    EXTI_InitStructure.EXTI_Line = EXTI_Line8; //外部中断线 8  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能  
    EXTI_Init(& EXTI_InitStructure); //初始化配置  
      
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);    ///*将EXTI线8连接到PA8*/  
}  

u8 HW_ReceiveTime(void)  
{  
    u8 t=0;  
  
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1)//高电平  
    {  
        t++;  
        delay_us(20);  
  
        if(t>=250) return t;//超时溢出  
    }  
  
    return t;  
}  

/********************************************************* 
                中断服务程序 
********************************************************/  
void EXTI9_5_IRQHandler()       
{  
     u8 time=0,startSignal=0,Data=0,Count=0;  
     u32 HW_ReceivedData=0;  
     //printf("1\n");  
     if(EXTI_GetITStatus(EXTI_Line8) == SET)//检查指定的EXTI线路触发请求发生与否，返回一个EXTI_Line新状态  
      {  
            //printf("5\n");   
            //if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))  GPIO_SetBits(GPIOA,GPIO_Pin_0);//点亮LED  
            //else  GPIO_ResetBits(GPIOA,GPIO_Pin_0);//灭  
            while(1)  
            {  
                    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1)  
                    {  
                         time = HW_ReceiveTime();  
                            
                         if(time>=250)  
                         {  
                            //printf("VoverTime:%x\n",time);   
                            startSignal=0;  
                            HW_ReceivedData=0;  
                            Count=0;  
                            break;  
                         }   
                         else if(time>=200 && time<250)  
                         {  
                              startSignal=1;//收到起始信号  
                              HW_ReceivedData=0;  
                              Count=0;  
                              continue;  
                         }   
                         else if(time>=60 && time<90)  Data=1;//收到数据 1  
                         else if(time>=10 && time<50)  Data=0;//收到数据 0  
                         //printf("%x ",Data);  
                         if(startSignal==1)  
                          {  
                                HW_ReceivedData<<=1;  
                                HW_ReceivedData+=Data;  
                                Count++;  
                                if(Count>=32)  
                                {  
                                    //printf("received:%x\n",HW_ReceivedData);  
                                    if(HW_ReceivedData==0x1090000a)  printf("红外编码:0x%8X,指令:打开空调\n",HW_ReceivedData);  
                                    else if(HW_ReceivedData==0x90000a)  printf("红外编码:0x%08X,指令:关闭空调\n",HW_ReceivedData);  
                                    else   printf("红外编码:0x%08X,指令:未知\n",HW_ReceivedData);  
                                    HW_ReceivedData=0;  
                                    Count=0;  
                                    startSignal=0;  
                                    break;  
                                }   
                                  
                          }     
                    }  
            }  
              
            EXTI_ClearITPendingBit(EXTI_Line8);//清除线路挂起位  
            EXTI_ClearFlag(EXTI_Line8);//清除中断挂起位   
     }   
}

