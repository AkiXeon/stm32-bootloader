#include "stm32f10x.h"
#include "delay.h"

void delay_init(void){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);		//72Mhz
}
void delay_us(uint32_t us){
	SysTick->LOAD = us*72;		//time = us * 72 / 72Mhz = us / 1 Mhz = us * 10^ -6 s
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= !SysTick_CTRL_ENABLE_Msk;
}
void delay_ms(uint16_t ms){
	while(ms--){
		delay_us(1000);
	}
}

