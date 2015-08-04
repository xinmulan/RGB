/*******************************************************************************
功能：IO驱动函数
作者：xinlan
时间: 2014-1-22
*******************************************************************************/

#define In_0	Read_In_0()  
#define In_1 	Read_In_1() 
#define In_2	Read_In_2() 
#define In_3	Read_In_3() 

#define Out_0(data)	Write_Out_0(BitAction data)                //Bit_RESET = 0, Bit_SET
#define Out_1(data)	Write_Out_1(BitAction data) 
#define Out_2(data)	Write_Out_2(BitAction data) 
#define Out_3(data)	Write_Out_3(BitAction data)
void  Io_Init(void);
u8  	Read_In_0(void);
u8  	Read_In_1(void);
u8  	Read_In_2(void);
u8  	Read_In_3(void);

void  Write_Out_0(BitAction data);
void  Write_Out_1(BitAction data);
void  Write_Out_2(BitAction data);
void  Write_Out_3(BitAction data);

void  Write_Relay_0(BitAction data);
void  Write_Relay_1(BitAction data);
void  Write_Relay_2(BitAction data);
void  Write_Relay_3(BitAction data);
