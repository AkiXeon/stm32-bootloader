#ifndef __FMC_H_
#define __FMC_H_
#include "stm32f10x.h"
#include "stdint.h"
void erase_flash(uint16_t startpage,uint16_t number);
void write_flash(uint32_t addr,uint32_t *data,uint32_t number);
#endif



