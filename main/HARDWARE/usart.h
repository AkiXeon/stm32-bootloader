#ifndef __01_PERIPH_UART1_H_
#define __01_PERIPH_UART1_H_


#include "stm32f10x.h"

#define		USART1_RXBufSize		2048			//宏定义串口缓冲区大小
#define		USART1_TXBufSize		2048			//宏定义串口缓冲区大小
#define		USART1_RXMax				256 			//宏定义串口单次接收数据量最大值
#define		USART1_RXBufSESize	20			  //宏定义记录串口接收数据起始位置及结束位置的指针对的数量


typedef struct{												//定义一个结构体类型，
	uint8_t * start_ptr;									//成员1指向本次传输数据的起始地址
	uint8_t * end_ptr;										//成员2指向本次传输数据的结束地址
}USART1_RXBufPtr;

typedef struct{																			//定义一个结构体类型，总的控制结构体类型
	uint16_t USART_RX_Counter;													//成员变量1记录传输的总数据量
	USART1_RXBufPtr USART1_RXBufSE[USART1_RXBufSESize];	//每次接收数据的起始位置及结束位置的数组
	USART1_RXBufPtr * USART1_RX_IN;											//成员变量3记录下一次接收数据存放的位置
	USART1_RXBufPtr * USART1_RX_OUT;										//成员变量4记录上一次处理完的数据的位置
	USART1_RXBufPtr * USART1_RX_END;										//成员变量5记录接收缓冲区数组的最后的位置
}USART1_RXCtrlBlock;

extern USART1_RXCtrlBlock USART1_RXCtrlBlockStru;
extern uint8_t USART1_RXBuf[USART1_RXBufSize];


void USART1_Init(uint32_t bandrate);
void USART1_RXCtrlBlockInit(void);
void USART_printf(char *format,...);




#endif /* __01_PERIPH_UART1_H_*/


/*********************************************END OF FILE**********************/
