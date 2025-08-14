#include "fmc.h"
#include "stm32f10x.h"

void erase_flash(uint16_t startpage,uint16_t number){
    uint16_t i;
    FLASH_Unlock();
    for(i=0;i<number;i++){
        FLASH_ErasePage(0x08000000 + startpage*1024 + i*1024);
    }
    FLASH_Lock();
}
void write_flash(uint32_t addr,uint32_t *data,uint32_t number){
    FLASH_Unlock();
    while(number){
        FLASH_ProgramWord(addr,*data);
        addr+=4;
        number-=4;
        data++;
        
    }
    FLASH_Lock();
}
