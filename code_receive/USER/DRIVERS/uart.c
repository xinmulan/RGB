/*******************************************************************************
���ܣ�BSP��ʼ������
���ߣ�xinlan
ʱ��: 2014-1-22
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "DRIVERS\uart.h"
#include "stm32f10x.h"

#define SERIAL_TRANSMIT_BUFFER_SIZE 255
#define SERIAL_RECEIVE_BUFFER_SIZE 255
volatile u8 transmit_Buffer[SERIAL_TRANSMIT_BUFFER_SIZE]; // ���ͻ�������
volatile u8 transmit_BufferHead = 0; // ���ͻ�������ͷ������ָ����һ�����е�Ԫ��
volatile u8 transmit_BufferTail = 0; // ���ͻ�������β������ָ�����һ����Ч��Ԫ��
volatile u8 recevie_Buffer[SERIAL_RECEIVE_BUFFER_SIZE]; // ���ͻ�������
volatile u8 recevie_Buffer_BufferHead = 0; // ���ջ�������ͷ������ָ����һ�����е�Ԫ��
volatile u8 recevie_Buffer_BufferTail = 0; // ���ջ�������β������ָ�����һ����Ч��Ԫ��

enum
{
    UART_SUCCEED = 0,
    UART_FAILED  = 1
};


/*******************************************************************************
* Function Name  : UART1_Configuration
* Description    : Configures the uart1 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure the USART1*/ 
  USART_Init(USART1, &USART_InitStructure);

     /* Enable USART1 Receive and Transmit interrupts */
  USART_ClearFlag(USART1,USART_FLAG_TC);
  //USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	 //�����ж�ģʽ����


  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);  
}


/*******************************************************************************
* Function Name  : UART1_GPIO_Configuration
* Description    : Configures the uart1 GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Configure USART1_Tx as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART1_Rx as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}


/*******************************************************************************
* Function Name  : Uart1_PutChar
* Description    : printf a char to the uart.
* Input          : ch ��Ҫ���͵��ַ� 
* Output         : None
* Return         : None
*******************************************************************************/
u8 Uart1_PutChar(u8 ch)//����һ���ַ�
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}

/*******************************************************************************
* Function Name  : Uart1_PutString
* Description    : print a string to the uart1
* Input          : bufΪ�������ݵĵ�ַ , lenΪ�����ַ��ĸ���
* Output         : None
* Return         : None
*******************************************************************************/
void Uart1_PutString(u8* buf , u8 len)//����һ���ַ���
{
	u8 i;
    for(i=0;i<len;i++)
    {
        Uart1_PutChar(*buf++);
    }
}
//����1�ж�
void UART1_NVIC_Configuration(void)
{
	/* Enable the USART1 Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                               //

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


// ��ȡ���ͻ��������пռ��С��
uint8_t serial_transmitBufferIdle(void)
{

	if(transmit_BufferTail <= transmit_BufferHead)
		return SERIAL_TRANSMIT_BUFFER_SIZE - transmit_BufferHead + transmit_BufferTail;
	return transmit_BufferTail - transmit_BufferHead - 1; // -1:������������
}

// ��ȡ���ͻ������е����ݳ��ȡ�
uint32_t uart_transmitBufferSizeOccupy(void)
{
	if(transmit_BufferTail <= transmit_BufferHead)
		return transmit_BufferHead - transmit_BufferTail;
	return transmit_BufferHead  + (SERIAL_TRANSMIT_BUFFER_SIZE - transmit_BufferTail);
}

// �ӷ��ͻ�����ȡ��һ���ֽڣ����ͳ�ȥ��
// ����黺�����Ƿ�գ�����鷢��״̬��
void uart_transmitAByte(void)
{
    USART_SendData(USART1,transmit_Buffer[transmit_BufferTail]);
	transmit_BufferTail ++;
	if(transmit_BufferTail >= SERIAL_TRANSMIT_BUFFER_SIZE)
		transmit_BufferTail = 0;
}


// �������ݡ�
// ����0��ʾ�Ѿ����뻺����������1��ʾʧ�ܡ�
uint8_t serial_transmit(unsigned char * data,uint8_t len)
{
	uint32_t i;

	if(serial_transmitBufferIdle() >= len)
	{
		for(i=0;i<len;i++)
		{
			transmit_Buffer[transmit_BufferHead] = ((uint8_t *)data)[i];
			transmit_BufferHead ++;
			if(transmit_BufferHead >= SERIAL_TRANSMIT_BUFFER_SIZE)
			transmit_BufferHead = 0;
		}	
	}

		__disable_irq();

		if(uart_transmitBufferSizeOccupy() > 0)
        if(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == SET) 
					uart_transmitAByte();

   	__enable_irq();

	return 0;
}

/*******************************************************************************
* Function Name  : UART1_Configuration
* Description    : Configures the uart1 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure the USART1*/ 
  USART_Init(USART2, &USART_InitStructure);

     /* Enable USART1 Receive and Transmit interrupts */
  USART_ClearFlag(USART2,USART_FLAG_TC);
  //USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	 //�����ж�ģʽ����


  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);  
}


/*******************************************************************************
* Function Name  : UART1_GPIO_Configuration
* Description    : Configures the uart1 GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Configure USART2_Tx as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART2_Rx as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}


/*******************************************************************************
* Function Name  : Uart2_PutChar
* Description    : printf a char to the uart.
* Input          : ch ��Ҫ���͵��ַ� 
* Output         : None
* Return         : None
*******************************************************************************/
u8 Uart2_PutChar(u8 ch)//����һ���ַ�
{
  /* Write a character to the USART */
  USART_SendData(USART2, (u8) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {
  }
return ch;
}

/*******************************************************************************
* Function Name  : Uart1_PutString
* Description    : print a string to the uart1
* Input          : bufΪ�������ݵĵ�ַ , lenΪ�����ַ��ĸ���
* Output         : None
* Return         : None
*******************************************************************************/
void Uart2_PutString(u8* buf , u8 len)//����һ���ַ���
{
	u8 i;
    for(i=0;i<len;i++)
    {
        Uart2_PutChar(*buf++);
    }
}

//����2�ж�
void UART2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                               //

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
