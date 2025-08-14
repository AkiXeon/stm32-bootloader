#include "uart1.h"

uint8_t Uart_RxBuffer[RXBUFFERSIZE];
uint8_t Uart_TxBuffer[RXBUFFERSIZE];
Uart_ConBlock Uart_CB1;

void ptrinit(void){
    Uart_CB1.Uart_Rxptrin = &Uart_CB1.Uart_Rxbuffptr[0];
    Uart_CB1.Uart_Rxptrout = &Uart_CB1.Uart_Rxbuffptr[0];
    Uart_CB1.Uart_Rxptrend = &Uart_CB1.Uart_Rxbuffptr[RXBUFFERPTRNUM-1];
    Uart_CB1.Rx_Counter = 0;
    Uart_CB1.Uart_Rxptrin->start = Uart_RxBuffer;  
}
void uart1_init(uint32_t baudrate){
    GPIO_InitTypeDef GPIOS;
    USART_InitTypeDef USARTS;
	NVIC_InitTypeDef NVICS;
	DMA_InitTypeDef DMA_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIOS.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOS.GPIO_Pin = GPIO_Pin_9;
    GPIOS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOS);
    GPIOS.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOS.GPIO_Pin = GPIO_Pin_10;
    GPIOS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOS);

    USARTS.USART_BaudRate = baudrate;
    USARTS.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USARTS.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USARTS.USART_Parity = USART_Parity_No;
    USARTS.USART_StopBits = USART_StopBits_1;
    USART_Init(USART1, &USARTS);
    

    
    NVICS.NVIC_IRQChannel = USART1_IRQn;
    NVICS.NVIC_IRQChannelPreemptionPriority = 0;
    NVICS.NVIC_IRQChannelSubPriority = 0;
    NVICS.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICS);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart_RxBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RXSIZEMAX + 1;   //避免进入完毕状态
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    ptrinit();
}

