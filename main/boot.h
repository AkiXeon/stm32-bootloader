#ifndef __BOOT_H_
#define __BOOT_H_
#include "stdint.h"

typedef void (*pc)(void);
void OTA_Check(void);
void readflag(void);
void OTA_JUMP(uint32_t addr);
void Commandline_excute(uint8_t *data,uint16_t datalen);
uint16_t bootloader_enter(uint16_t timeout);
uint16_t xmodem_crc16(uint8_t *data, uint16_t len);
#endif



