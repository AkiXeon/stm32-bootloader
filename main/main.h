#ifndef __MAIN_H_
#define __MAIN_H_
#include "stdint.h"
#include "stm32f10x.h"
#include "fmc.h"

#define UPDATA_A_FLAG 0x00000001
#define IAP_MODEM_FLAG 0x00000002
#define IAP_MODEMD_FLAG 0x00000004
/*  逻辑结构 B - > A - > C - > D*/
// 定义 STM32 闪存的起始地址，0x08000000 是 STM32 系列芯片闪存的通常起始地址。
#define STM32_SADDR 0x08005000
// 定义 STM32 闪存的总页数，这里表示整个闪存共有 64 页。
#define STM32_PAGE 64
// 定义每页闪存的大小，单位为字节，这里表示每页闪存大小为 1024 字节（即 1KB）。
#define PAGE_SIZE 1024
// 定义逻辑结构 B 所占用的页数，这里表示 B 部分占用 20 页。
#define STM32_B 20
// 定义逻辑结构 A 所占用的页数，这里表示 A 部分占用 28 页。
#define STM32_A 28
// 定义逻辑结构 C 所占用的页数，这里表示 C 部分占用 15 页。
#define STM32_C 15
// 定义逻辑结构 D 所占用的页数，这里表示 D 部分占用 1 页。
#define STM32_D 1
// 定义逻辑结构 A 的起始页号，其起始页号等于逻辑结构 B 所占用的页数，即 A 从 B 结束的下一页开始。
#define A_SPAGE STM32_B
// 定义逻辑结构 C 的起始页号，其起始页号等于 A 的起始页号加上 A 所占用的页数，即 C 从 A 结束的下一页开始。
#define C_SPAGE (A_SPAGE+STM32_A)
// 定义逻辑结构 D 的起始页号，其起始页号等于 C 的起始页号加上 C 所占用的页数，即 D 从 C 结束的下一页开始。
#define D_SPAGE (C_SPAGE + STM32_C)

#define OTA_SET_FLAG 0xABCDDCBA

typedef struct 
{
    uint32_t OTA_Flag;
    uint32_t firelen[20];
}OTA_InfoCB;


typedef struct{
    uint8_t updatebuffer[1024]; //对应1K的PAGE大小；
    uint8_t flashblock_num;
    uint32_t Xmodemtimer;
    uint32_t Xmodemnum;
    uint32_t Xmodemcrc;
}UPDATE_CB;

extern OTA_InfoCB OTA_info;
extern UPDATE_CB   UPDATE_A;
extern uint32_t BootStatFlag;

#endif
