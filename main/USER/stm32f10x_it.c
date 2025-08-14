/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h" 
#include "usart.h"

// void USART1_IRQHandler(void)
// {
//     if(USART_GetITStatus(USART1,USART_IT_IDLE)!=0)
//     {
//         USART_GetFlagStatus(USART1,USART_FLAG_IDLE);//清除标志位
//         USART_ReceiveData(USART1);
        
//         U1_CB.Count+=(U1_RxMAX+1)-DMA_GetCurrDataCounter(DMA1_Channel5);
//         U1_CB.U1_ptrIN->end = &U1_RxBuff[U1_CB.Count-1];
//         U1_CB.U1_ptrIN++;
//         if(U1_CB.U1_ptrIN == U1_CB.U1_ptrEND)
//         {
//             U1_CB.U1_ptrIN = &U1_CB.U1_ptr[0];
//         }
//         if((U1_RxSize-U1_CB.Count)>=U1_RxMAX)
//         {
//             U1_CB.U1_ptrIN->start=&U1_RxBuff[U1_CB.Count];
            
//         }
//         else{
//             U1_CB.U1_ptrIN->start = U1_RxBuff;
//             U1_CB.Count = 0;
//         }
        
//         DMA_DeInit(DMA1_Channel5);
//         DMA_InitTypeDef DMA_InitStructure;
//         DMA_InitStructure.DMA_BufferSize = U1_RxMAX+1;
//         DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//         DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//         DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)U1_CB.U1_ptrIN->start;
//         DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_Byte;
//         DMA_InitStructure.DMA_MemoryInc  = DMA_MemoryInc_Enable;
//         DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//         DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE+0x04;
//         DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//         DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//         DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//         DMA_Init(DMA1_Channel5,&DMA_InitStructure);
//         DMA_Cmd(DMA1_Channel5,ENABLE);
//     }
    
    
// }




/*
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != 0){	//判断接收是否为串口空闲中断
		USART_GetFlagStatus(USART1, USART_FLAG_IDLE);
		USART_ReceiveData(USART1);										//这两句其实并不是为了读取数据，而是为了满足清除空闲中断的条件：“软件序列清除该位(先读USART_SR，然后读USART_DR)”
		
		
		
		U1_CB.URxCount += (USART1_REC_LEN + 1) - DMA_GetCurrDataCounter(DMA1_Channel5);	//记录接收缓冲区内数据量		
		U1_CB.URxDataIN->end = &USART1_RX_BUF[U1_CB.URxCount - 1];	//指向当前接收的不定长数据的末尾
		U1_CB.URxDataIN++;	//指向下一个接收缓冲区		初值是U1_CB.URxDataIN = &U1_CB.URxDataPtr[0];此处自增就相当于URxDataIN = &U1_CB.URxDataPtr[1];
		
		if(U1_CB.URxDataIN == U1_CB.URxDataEND)	//如果下一个接收缓冲区到了末尾
		{
			U1_CB.URxDataIN = &U1_CB.URxDataPtr[0];	//如果已经到缓冲末尾，则重新回到缓冲区头部
		}
		
		if((USART1_RX_SIZE - U1_CB.URxCount) >= USART1_REC_LEN)	//如果剩余空间满足一次最大的接收需求
		{
			U1_CB.URxDataIN->start = &USART1_RX_BUF[U1_CB.URxCount];	//写入指针后移
		}else{																		//如果不满足一次需求
			U1_CB.URxDataIN->start = USART1_RX_BUF;	//写入指针回到起始位置
			U1_CB.URxCount = 0;											//缓冲区数目清空
		}
		
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_InitTypeDef DMA_InitStructure;
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 0x04;  //DMA外设基地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)U1_CB.URxDataIN->start;  //DMA内存基地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
		DMA_InitStructure.DMA_BufferSize = USART1_REC_LEN + 1;  //DMA通道的DMA缓存的大小，由于采用空闲中断来获取不定长接收，所以设定接收buf大于可接收最大值，用不产生接收完成终端
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有中优先级 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
		
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
		
	}
	
	
}
*/

/*
#include "stm32f10x_it.h" 
#include "stm32f10x_dma.h"
#include "usart.h"
		

void USART1_IRQnHandler(void)
{
   
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=0)
    {
       
        USART_GetFlagStatus(USART1, USART_FLAG_IDLE);
		USART_ReceiveData(USART1);
        U1_CB.count += (U1_MAX+1)-DMA_GetCurrDataCounter(DMA1_Channel5);
        U1_CB.U1_Rxptr_IN ->end =& U1_RxBuff[U1_CB.count-1];
        U1_CB.U1_Rxptr_IN ++;
        
        if(U1_CB.U1_Rxptr_IN == U1_CB.U1_Rxptr_END)
        {
            U1_CB.U1_Rxptr_IN = U1_CB.U1_Rxptr[0];
        }
        if((U1_Rx_Size - U1_CB.count) >= U1_MAX)///如果超出了范围
        {
            U1_CB.U1_Rxptr_IN->start =&U1_RxBuff[U1_CB.count];
            
        }
        else{
            U1_CB.U1_Rxptr_IN->start = U1_RxBuff;
            U1_CB.count = 0;
        }
   
    }
    DMA_Cmd(DMA1_Channel5,DISABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_BufferSize = U1_MAX+1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)U1_CB.U1_Rxptr_IN->start;
    DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE+0x04;
    DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel5,&DMA_InitStructure);
    DMA_Cmd(DMA1_Channel5,ENABLE);
}

 */
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


