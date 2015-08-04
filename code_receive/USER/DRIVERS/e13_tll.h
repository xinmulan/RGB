/*******************************************************************************
功能：E13-TLL无线串口驱动函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

void  E13_Io_Init(void);
u8 E13_Configuration(void);
u8  Read_Aux(void);
void  Write_Set(BitAction data);
int El3_Send_Char(char);
char crc16(uint8_t *msg_buf, int8_t nbytes);
char E13_Receive_Data(u8 data[],u8 recevie_Buffer_Piont);
extern  unsigned char  E13_Init_Succes;																//E13配置成功
