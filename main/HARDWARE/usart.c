#include "usart.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

uint8_t USART1_RXBuf[USART1_RXBufSize];						//定义串口接收数据缓冲区
uint8_t USART1_TXBuf[USART1_TXBufSize];						//定义串口发送数据缓冲区
USART1_RXCtrlBlock USART1_RXCtrlBlockStru;


/**
 ***************************************************************************************
 * @brief       ：初始化串口1
 * @param       ：uint32_t bandrate，串口波特率
 * @return      ：无
 * @exception   ：无
 * @author      ：Owen Lee
 * @date        ：2025.04.07
 * @version     ：V1.00
 * @property    ：本函数用于完成串口1的GPIO初始化及USART外设初始化
 ***************************************************************************************
**/
void USART1_Init(uint32_t bandrate)
{
/****************************定义外设初始化结构体****************************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;	
/****************************使能相关外设时钟********************************************/	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);									// 使能DMA1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);							// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							// 使能USART1时钟
/****************************初始化串口GPIO**********************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;													// USART1_TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;												// USART1_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
/****************************配置及初始化串口1*****************************************/		
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		// 串口全双工模式
	USART_InitStructure.USART_BaudRate = bandrate;										// 串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;				// 数据位8bit
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						// 停止位1bit
																																		// 无硬件流控
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	
/*************************配置及初始化串口1DMA外设************************************/		
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);			// 串口DMA接收，外设基地址为串口数据寄存器
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// 串口DMA接收，外设数据长度为字节
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				// 串口DMA接收，外设地址不自增
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RXBuf;					// 串口DMA接收，存储器基地址为接收缓冲区数组
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					// 串口DMA接收，存储器数据长度为字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;								  // 串口DMA接收，存储器地址自增
	
	DMA_InitStructure.DMA_BufferSize = USART1_RXMax + 1;										// 串口DMA接收，DMA缓冲区大小
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											// 串口DMA接收，从串口到内存，外设做源
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														// 串口DMA接收，普通模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														// 串口DMA接收，失能存储器到存储器模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;									// 串口DMA接收，优先级最高
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);															// 串口DMA接收，初始化DMA1_Channel5
/*********************************使能相关外设********************************************/			
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;						//串口中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能NVIC串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//主优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级1
	NVIC_Init(&NVIC_InitStructure);															//初始化NVIC
/*********************************使能相关外设********************************************/			
	USART1_RXCtrlBlockInit();
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);											// 使能串口1空闲中断
	USART_Cmd(USART1,ENABLE);																					// 使能串口1
	DMA_Cmd(DMA1_Channel5,ENABLE);																		// 使能DMA
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);										  // 使能串口DMA发送
}


/**
 ***************************************************************************************
 * @brief       ：串口1中断函数
 * @param       ：无
 * @return      ：无
 * @exception   ：无
 * @author      ：Owen Lee
 * @date        ：2024.12.24
 * @version     ：V1.00
 * @property    ：本函数实现串口1的中断响应函数
 ***************************************************************************************
**/
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_IDLE))								//产生空闲中断
	{
																														//清除空闲中断，分两步，第一步先读USART_FLAG_IDLE标志位，第二步再获取接收数据
		USART_GetFlagStatus(USART1,USART_FLAG_IDLE);						//清除空闲中断--先读USART_FLAG_IDLE标志位			
		USART_ReceiveData(USART1);															//清除空闲中断--再获取接收数据
																														//计算本次传输的数据量
																														//DMA单次传输最多（USART1_RXMax + 1）个数据，如果传输100个数据就产生停止了，
																														//那么DMA的NDTR寄存器剩余值就是 ：USART1_RXMax + 1 - 100
																														//用(USART1_RXMax + 1) - (DMA->NDTR)即可得到本次传输的数据量 
		USART1_RXCtrlBlockStru.USART_RX_Counter += ((USART1_RXMax + 1) - DMA_GetCurrDataCounter(DMA1_Channel5));		
																														//标记end指针位置
		USART1_RXCtrlBlockStru.USART1_RX_IN->end_ptr = &USART1_RXBuf[USART1_RXCtrlBlockStru.USART_RX_Counter - 1];																									
		USART1_RXCtrlBlockStru.USART1_RX_IN ++;									//IN指针指向下一个位置
																														//判断IN指针是否指向END指针,如果指向了END指针，则IN指针重新指向USART1_RXBufSE[0]
		if(USART1_RXCtrlBlockStru.USART1_RX_IN == USART1_RXCtrlBlockStru.USART1_RX_END)
		{
			USART1_RXCtrlBlockStru.USART1_RX_IN =  &(USART1_RXCtrlBlockStru.USART1_RXBufSE[0]);
		}
																														//判断USART1_RXBuf剩余值是否能够接收一次单次接收最大数据量
		if(USART1_RXBufSize - USART1_RXCtrlBlockStru.USART_RX_Counter >= USART1_RXMax)
		{																												//如果剩余值能够再接收一次，则在剩余值位置继续接收
			USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr = &USART1_RXBuf[USART1_RXCtrlBlockStru.USART_RX_Counter];	
		}
		else
		{																												//如果剩余值不够再接收一次，则重新从0位置开始接收																		
			USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr = USART1_RXBuf;
			USART1_RXCtrlBlockStru.USART_RX_Counter = 0;					//接收数据累加值清零
		}
		DMA_Cmd(DMA1_Channel5,DISABLE);													//重新配置DMA，先失能
		while(((DMA1_Channel5->CCR) & DMA_CCR1_EN) == 1);				//确保DMA可以被设置 
//		DMA_ClearFlag(DMA1_FLAG_TC5);														//清除传输完成中断标志位，这里DMA没有使能中断，但是要重新开启DMA必须先清除DMA的中断【参见DMA的数据手册】[F1系列没找到相关说明，应该不用配置]

		DMA1_Channel5->CNDTR = USART1_RXMax + 1;									//再次设置DMA传输数据量
																														//设置DMA传输内存地址
		DMA1_Channel5->CMAR = (uint32_t)(USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr);
		DMA_Cmd(DMA1_Channel5,ENABLE);														//使能DMA
	}
}

/**
 ***************************************************************************************
 * @brief       ：串口1接收数据控制结构体初始化
 * @param       ：无
 * @return      ：无
 * @exception   ：无
 * @author      ：Owen Lee
 * @date        ：2024.12.24
 * @version     ：V1.00
 * @property    ：本函数实现串口1接收数据控制结构体的初始化
 ***************************************************************************************
**/
void USART1_RXCtrlBlockInit(void)
{
	USART1_RXCtrlBlockStru.USART1_RX_IN = &(USART1_RXCtrlBlockStru.USART1_RXBufSE[0]);
	USART1_RXCtrlBlockStru.USART1_RX_OUT = &(USART1_RXCtrlBlockStru.USART1_RXBufSE[0]);
	USART1_RXCtrlBlockStru.USART1_RX_END = &(USART1_RXCtrlBlockStru.USART1_RXBufSE[USART1_RXBufSESize - 1]);
	USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr = USART1_RXBuf;
	USART1_RXCtrlBlockStru.USART_RX_Counter = 0;
}



/**
 ***************************************************************************************
 * @brief       ：串口打印函数
 * @param       ：
									@param1 : uint8_t *format		参数格式
									@param2 : ...								不确定的类型及参数
 * @return      ：无
 * @exception   ：无
 * @author      ：Owen Lee
 * @date        ：2024.12.24
 * @version     ：V1.00
 * @property    ：本函数实现串口的printf函数
 ***************************************************************************************
**/
void USART_printf(char *format,...)
{
	uint16_t i;
	va_list listdata;
	va_start(listdata,format);
	vsprintf((char *)USART1_TXBuf,format,listdata);
	va_end(listdata);
	for(i = 0;i < strlen((const char*)USART1_TXBuf);i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == 0);							//等待发送寄存器为空
		USART_SendData(USART1,USART1_TXBuf[i]);															//发送数据
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == 0);								//等待最后一个数据发送完毕
}




/*********************************************END OF FILE**********************/

