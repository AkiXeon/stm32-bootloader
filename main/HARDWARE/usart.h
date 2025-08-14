#ifndef __01_PERIPH_UART1_H_
#define __01_PERIPH_UART1_H_


#include "stm32f10x.h"

#define		USART1_RXBufSize		2048			//�궨�崮�ڻ�������С
#define		USART1_TXBufSize		2048			//�궨�崮�ڻ�������С
#define		USART1_RXMax				256 			//�궨�崮�ڵ��ν������������ֵ
#define		USART1_RXBufSESize	20			  //�궨���¼���ڽ���������ʼλ�ü�����λ�õ�ָ��Ե�����


typedef struct{												//����һ���ṹ�����ͣ�
	uint8_t * start_ptr;									//��Ա1ָ�򱾴δ������ݵ���ʼ��ַ
	uint8_t * end_ptr;										//��Ա2ָ�򱾴δ������ݵĽ�����ַ
}USART1_RXBufPtr;

typedef struct{																			//����һ���ṹ�����ͣ��ܵĿ��ƽṹ������
	uint16_t USART_RX_Counter;													//��Ա����1��¼�������������
	USART1_RXBufPtr USART1_RXBufSE[USART1_RXBufSESize];	//ÿ�ν������ݵ���ʼλ�ü�����λ�õ�����
	USART1_RXBufPtr * USART1_RX_IN;											//��Ա����3��¼��һ�ν������ݴ�ŵ�λ��
	USART1_RXBufPtr * USART1_RX_OUT;										//��Ա����4��¼��һ�δ���������ݵ�λ��
	USART1_RXBufPtr * USART1_RX_END;										//��Ա����5��¼���ջ��������������λ��
}USART1_RXCtrlBlock;

extern USART1_RXCtrlBlock USART1_RXCtrlBlockStru;
extern uint8_t USART1_RXBuf[USART1_RXBufSize];


void USART1_Init(uint32_t bandrate);
void USART1_RXCtrlBlockInit(void);
void USART_printf(char *format,...);




#endif /* __01_PERIPH_UART1_H_*/


/*********************************************END OF FILE**********************/
