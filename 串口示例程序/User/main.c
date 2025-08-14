#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "string.h"
#include "uart1.h"
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
	uart1_init(115200);
	printf("hello world!\r\n");
	uint16_t count = 0;
	while(1){
		if(Uart_CB1.Uart_Rxptrout != Uart_CB1.Uart_Rxptrin){
			printf("you have received %d bytes data\r\n",(Uart_CB1.Uart_Rxptrout->end - Uart_CB1.Uart_Rxptrout->start + 1));
			for(count = 0;count < (Uart_CB1.Uart_Rxptrout->end - Uart_CB1.Uart_Rxptrout->start + 1);count++){
				printf("%c",Uart_CB1.Uart_Rxptrout->start[count]);
			}
			printf("\r\n");
			Uart_CB1.Uart_Rxptrout++;
		}
		if(Uart_CB1.Uart_Rxptrout == Uart_CB1.Uart_Rxptrend){
			Uart_CB1.Uart_Rxptrout = Uart_CB1.Uart_Rxbuffptr;
		}
	}
}





