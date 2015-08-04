#include "stdio.h"
#include <stm32f10x.h>


void Write_Flash(u32 address,u16 data)
{
		
		FLASH_Unlock();

    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR); 

    FLASH_ErasePage(address);
    FLASH_ProgramHalfWord(address,data);  
		FLASH_Lock();

}
u16  Read_Flash(u32 point)
{	
 return *(uint16_t*)point;
}
