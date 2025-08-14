#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "string.h"

void delay(){
	int i = 0;
	int j = 0;
	for(i = 0;i < 10000;i++)
	{
		for(j = 0;j < 1000;j++){
			
		}
	}
}

int fputc( int ch, FILE *f ){
                      USART_SendData(USART1,(u8) ch );
                      while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
                      return ch;
               }

int main(){
	GPIO_InitTypeDef GPIOS;
	USART_InitTypeDef USARTS;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	USARTS.USART_BaudRate = 115200;
	USARTS.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTS.USART_Mode = USART_Mode_Tx;
	USARTS.USART_Parity = USART_Parity_No;
	USARTS.USART_StopBits = USART_StopBits_1;
	USARTS.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USARTS);
	USART_Cmd(USART1,ENABLE);
	GPIOS.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOS.GPIO_Pin = GPIO_Pin_13;
	GPIOS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIOS);
	GPIOS.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOS.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIOS);
	
	while(1){
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay();
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay();
		printf("This is another program ...\r\n");
	}
}

