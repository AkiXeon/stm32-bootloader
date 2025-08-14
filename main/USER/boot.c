#include "boot.h"
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
uint8_t test[5] = {1,2,3,4,5};
void readflag(void){
    OTA_info.OTA_Flag = *(uint32_t *)(D_SPAGE*PAGE_SIZE+STM32_SADDR);
}
void Commandline_info(void){
    USART_printf("\r\n");
    USART_printf("[1]  : erase the A sector\r\n");
    USART_printf("[2]  : download the program by IAP\r\n");
    USART_printf("[3]  : set OTA version\r\n");
    USART_printf("[4]  : look OTA version\r\n");
    USART_printf("[5]  : download the program to external flash\r\n");
    USART_printf("[6]  : use program in external flash\r\n");
    USART_printf("[7]  : reboot\r\n");
}
void Commandline_excute(uint8_t *data,uint16_t datalen){
        if(BootStatFlag == 0){
            if((datalen == 1)&&(data[0] == '1')){
                USART_printf("sector A has been erased!\r\n");
                USART_printf("datalen is %d,data[0] is %x\r\n",datalen,data[0]);
                erase_flash(A_SPAGE,STM32_A);
            
            }else if((datalen == 1)&&(data[0] == '7')){
                USART_printf("reboot!\r\n");
                USART_printf("datalen is %d,data[0] is %x\r\n",datalen,data[0]);
                delay_ms(100);
                memset(USART1_RXBuf,0,sizeof(USART1_RXBuf));
                NVIC_SystemReset();
            
            }else if((datalen == 1)&&(data[0] == '2')){
                USART_printf("download the program by IAP, (use the bin file)\r\n");
                erase_flash(A_SPAGE,STM32_A);
                BootStatFlag |= (IAP_MODEM_FLAG|IAP_MODEMD_FLAG);
                UPDATE_A.Xmodemtimer = 0;
                UPDATE_A.Xmodemnum = 0;
            }
            
        }
        if(BootStatFlag & IAP_MODEMD_FLAG){
            
            
            		if((datalen==133)&&(data[0]==0x01))		
		{
			BootStatFlag &=~ IAP_MODEM_FLAG ;
			UPDATE_A.Xmodemcrc=xmodem_crc16(&data[3],128);
			if(UPDATE_A.Xmodemcrc == data[131]*256 + data[132])
			{
				UPDATE_A.Xmodemnum++;
				memcpy(&UPDATE_A.updatebuffer[((UPDATE_A.Xmodemnum-1) % (PAGE_SIZE/128)) * 128], &data[3],128); //每次往一个包中写入128字节，updatebuffer最大只有1024字节
				if((UPDATE_A.Xmodemnum % (PAGE_SIZE/128))== 0) //每8个凑满写完一个1024的数据包
				{
					write_flash(STM32_SADDR  + ((UPDATE_A.Xmodemnum / (PAGE_SIZE/128) -1)* PAGE_SIZE),(uint32_t *)UPDATE_A.updatebuffer,PAGE_SIZE);  //每次写入一个包的给扇区，但是写不满，一个扇区为128*1024
				}
				USART_printf("\x06");
			}else
			{
				USART_printf("\x15");
			}
		}	
		if((datalen==1)&&(data[0]==0x04))		
		{
			USART_printf("\x06");
			if((UPDATE_A.Xmodemnum) % (PAGE_SIZE/128)!= 0)//如果还剩一点数据凑不满一个数据包
			{
					
				write_flash(STM32_SADDR + (UPDATE_A.Xmodemnum/(PAGE_SIZE/128) * PAGE_SIZE), (uint32_t *)UPDATE_A.updatebuffer,((UPDATE_A.Xmodemnum) % (PAGE_SIZE / 128)* 128));  //写入剩下的一点数据
			}			
			BootStatFlag &=~ IAP_MODEMD_FLAG ;
			delay_ms(100);
			NVIC_SystemReset();
		}
            
        }
}
void OTA_Check(void){
    if(bootloader_enter(2000) == 1)
    {
        USART_printf("enter command line environment successfully!\r\n");
        Commandline_info();
    }else{
        if(OTA_info.OTA_Flag == OTA_SET_FLAG)
            {
                USART_printf("OTA update request!\r\n");
            }
        else
            {
                USART_printf("%x\r\n",OTA_info.OTA_Flag);
                USART_printf("%x\r\n",OTA_SET_FLAG);
                USART_printf("jump to A sector\r\n");
                OTA_JUMP(STM32_SADDR);
            }
    }   
    
}
void OTA_JUMP(uint32_t addr){
    if((*(uint32_t *)addr >= 0x20000000)&&(*(uint32_t *)addr <= 0x20004FFF)){
        USART_printf("addr is %x\r\n",addr);
        USART_printf("addr in ram is %x\r\n",*(uint32_t *)(addr));
        __set_MSP(*(uint32_t *)(addr));//STM32_SADDR+A_SPAGE*PAGE_SIZE
        pc pc_jump = (pc)*(uint32_t *)(addr+4);
        pc_jump();
        // while(addr <= 0x08005ABF){
        //     USART_printf("%08x\r\n",*(uint32_t *)addr);
        //     addr+=4;
        // }
    }else{
        USART_printf("addr is %x\r\n",addr);
        USART_printf("addr in ram is %x\r\n",*(uint32_t *)(addr));
        USART_printf("Jump failed, System will reboot into bootloader soon\r\n");
        delay_ms(500);
    }
    Commandline_info();
}

uint16_t bootloader_enter(uint16_t timeout){
    USART_printf("please enter 'w' in 2 second\r\n",timeout/1000);
    while(timeout--){
        delay_ms(1);
        if(USART1_RXBuf[0] == 'w')
        {
            return 1;
        }
    }
    return 0;


}

uint16_t xmodem_crc16(uint8_t *data, uint16_t len) {
    uint16_t crc = 0;               // 初始值 = 0
    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;  // 新字节移入 CRC 高位
        for (int i = 0; i < 8; i++) {     // 处理每个字节的 8 位
            if (crc & 0x8000)             // 检查最高位
                crc = (crc << 1) ^ 0x1021; // 异或多项式
            else
                crc = crc << 1;          // 仅左移
        }
    }
    return crc;
}


