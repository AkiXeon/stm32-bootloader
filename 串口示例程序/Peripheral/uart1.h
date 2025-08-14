#ifndef __UART1_H__
#define __UART1_H__

#include "stdint.h"
#include "stm32f10x.h"

#define RXBUFFERSIZE  2048
#define RXSIZEMAX 256
#define RXBUFFERPTRNUM 10




typedef struct{
    uint8_t *start;
    uint8_t *end;
}Uart_Rxptr;

typedef struct
{
    uint16_t Rx_Counter;
    Uart_Rxptr Uart_Rxbuffptr[RXBUFFERPTRNUM];
    Uart_Rxptr *Uart_Rxptrin;
    Uart_Rxptr *Uart_Rxptrout;
    Uart_Rxptr *Uart_Rxptrend;
}Uart_ConBlock;




extern Uart_ConBlock Uart_CB1;
extern uint8_t Uart_RxBuffer[RXBUFFERSIZE];
void uart1_init(uint32_t baudrate);

#endif


