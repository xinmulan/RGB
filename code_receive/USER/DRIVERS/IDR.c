void GPIO_Configuration(void)  
{  
    
  GPIO_InitTypeDef GPIO_InitStructure;        //����GPIO��ʼ���ṹ��  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������     
  GPIO_Init(GPIOA, &GPIO_InitStructure);       //PA9�������  
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   //�������  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //���ó���������;   
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
    
}



void NVIC_Configuration(void)    //�жϷ�������ȼ�����    �����STM32�ĺ���˵��(����).pdf��P165  
{  
    NVIC_InitTypeDef NVIC_InitStructure;  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  //0��,��ռ���ȼ�0λ,�����ȼ�4λ  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //�ⲿ�ж��� 9_5 �ж�(5-9��������ͳһʹ���ж���EXTI9_5_IRQn)  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //0������ռ���ȼ�,����ֻ���ô����ȼ�,����Ӧʽ���ȼ�  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��  
    NVIC_Init(& NVIC_InitStructure); //��ʼ������  
}  
  
void EXTI_Configuration(void)  //�ж�����  //�����STM32�ĺ���˵��(����).pdf��   P99  
{  
    EXTI_InitTypeDef EXTI_InitStructure;          
  
    EXTI_ClearITPendingBit(EXTI_Line8);//��� EXTI ��·����λ  
  
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //���� EXTI ��·Ϊ�ж�����  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //����������·�½���Ϊ�ж�����  
    EXTI_InitStructure.EXTI_Line = EXTI_Line8; //�ⲿ�ж��� 8  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��  
    EXTI_Init(& EXTI_InitStructure); //��ʼ������  
      
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);    ///*��EXTI��8���ӵ�PA8*/  
}  

u8 HW_ReceiveTime(void)  
{  
    u8 t=0;  
  
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1)//�ߵ�ƽ  
    {  
        t++;  
        delay_us(20);  
  
        if(t>=250) return t;//��ʱ���  
    }  
  
    return t;  
}  

/********************************************************* 
                �жϷ������ 
********************************************************/  
void EXTI9_5_IRQHandler()       
{  
     u8 time=0,startSignal=0,Data=0,Count=0;  
     u32 HW_ReceivedData=0;  
     //printf("1\n");  
     if(EXTI_GetITStatus(EXTI_Line8) == SET)//���ָ����EXTI��·������������񣬷���һ��EXTI_Line��״̬  
      {  
            //printf("5\n");   
            //if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))  GPIO_SetBits(GPIOA,GPIO_Pin_0);//����LED  
            //else  GPIO_ResetBits(GPIOA,GPIO_Pin_0);//��  
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
                              startSignal=1;//�յ���ʼ�ź�  
                              HW_ReceivedData=0;  
                              Count=0;  
                              continue;  
                         }   
                         else if(time>=60 && time<90)  Data=1;//�յ����� 1  
                         else if(time>=10 && time<50)  Data=0;//�յ����� 0  
                         //printf("%x ",Data);  
                         if(startSignal==1)  
                          {  
                                HW_ReceivedData<<=1;  
                                HW_ReceivedData+=Data;  
                                Count++;  
                                if(Count>=32)  
                                {  
                                    //printf("received:%x\n",HW_ReceivedData);  
                                    if(HW_ReceivedData==0x1090000a)  printf("�������:0x%8X,ָ��:�򿪿յ�\n",HW_ReceivedData);  
                                    else if(HW_ReceivedData==0x90000a)  printf("�������:0x%08X,ָ��:�رտյ�\n",HW_ReceivedData);  
                                    else   printf("�������:0x%08X,ָ��:δ֪\n",HW_ReceivedData);  
                                    HW_ReceivedData=0;  
                                    Count=0;  
                                    startSignal=0;  
                                    break;  
                                }   
                                  
                          }     
                    }  
            }  
              
            EXTI_ClearITPendingBit(EXTI_Line8);//�����·����λ  
            EXTI_ClearFlag(EXTI_Line8);//����жϹ���λ   
     }   
}

