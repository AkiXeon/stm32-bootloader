#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "fmc.h"
#include "boot.h"
#include "main.h"
uint16_t i;
OTA_InfoCB OTA_info;
UPDATE_CB   UPDATE_A;
uint32_t BootStatFlag = 0;


// 不幸的是，文件冲突了
int main(void)
{	
    delay_init();
	USART1_Init(115200);
	USART_printf("Init complete\r\n");
    uint32_t total;
    OTA_Check();
    while(1)
    {
        delay_ms(10);
		if(USART1_RXCtrlBlockStru.USART1_RX_IN != USART1_RXCtrlBlockStru.USART1_RX_OUT)
		{
			total = (USART1_RXCtrlBlockStru.USART1_RX_OUT->end_ptr - USART1_RXCtrlBlockStru.USART1_RX_OUT->start_ptr) + 1;
			Commandline_excute(USART1_RXCtrlBlockStru.USART1_RX_OUT->start_ptr,total);
			USART1_RXCtrlBlockStru.USART1_RX_OUT ++;
			if(USART1_RXCtrlBlockStru.USART1_RX_OUT == USART1_RXCtrlBlockStru.USART1_RX_END)
			{
				USART1_RXCtrlBlockStru.USART1_RX_OUT = &(USART1_RXCtrlBlockStru.USART1_RXBufSE[0]);
			}
		}
		if(BootStatFlag & IAP_MODEM_FLAG)
		{
			if(UPDATE_A.Xmodemtimer >= 100)
			{
				USART_printf("C \n");
				UPDATE_A.Xmodemtimer = 0;
			}
			UPDATE_A.Xmodemtimer++;
		}
    }	
}

