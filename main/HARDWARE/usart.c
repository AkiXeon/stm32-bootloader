#include "usart.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

uint8_t USART1_RXBuf[USART1_RXBufSize];						//���崮�ڽ������ݻ�����
uint8_t USART1_TXBuf[USART1_TXBufSize];						//���崮�ڷ������ݻ�����
USART1_RXCtrlBlock USART1_RXCtrlBlockStru;


/**
 ***************************************************************************************
 * @brief       ����ʼ������1
 * @param       ��uint32_t bandrate�����ڲ�����
 * @return      ����
 * @exception   ����
 * @author      ��Owen Lee
 * @date        ��2025.04.07
 * @version     ��V1.00
 * @property    ��������������ɴ���1��GPIO��ʼ����USART�����ʼ��
 ***************************************************************************************
**/
void USART1_Init(uint32_t bandrate)
{
/****************************���������ʼ���ṹ��****************************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;	
/****************************ʹ���������ʱ��********************************************/	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);									// ʹ��DMA1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);							// ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							// ʹ��USART1ʱ��
/****************************��ʼ������GPIO**********************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;													// USART1_TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;												// USART1_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
/****************************���ü���ʼ������1*****************************************/		
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		// ����ȫ˫��ģʽ
	USART_InitStructure.USART_BaudRate = bandrate;										// ���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;				// ����λ8bit
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ��У��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;						// ֹͣλ1bit
																																		// ��Ӳ������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	
/*************************���ü���ʼ������1DMA����************************************/		
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);			// ����DMA���գ��������ַΪ�������ݼĴ���
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// ����DMA���գ��������ݳ���Ϊ�ֽ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				// ����DMA���գ������ַ������
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RXBuf;					// ����DMA���գ��洢������ַΪ���ջ���������
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					// ����DMA���գ��洢�����ݳ���Ϊ�ֽ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;								  // ����DMA���գ��洢����ַ����
	
	DMA_InitStructure.DMA_BufferSize = USART1_RXMax + 1;										// ����DMA���գ�DMA��������С
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											// ����DMA���գ��Ӵ��ڵ��ڴ棬������Դ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														// ����DMA���գ���ͨģʽ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														// ����DMA���գ�ʧ�ܴ洢�����洢��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;									// ����DMA���գ����ȼ����
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);															// ����DMA���գ���ʼ��DMA1_Channel5
/*********************************ʹ���������********************************************/			
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;						//�����ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ��NVIC�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�1
	NVIC_Init(&NVIC_InitStructure);															//��ʼ��NVIC
/*********************************ʹ���������********************************************/			
	USART1_RXCtrlBlockInit();
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);											// ʹ�ܴ���1�����ж�
	USART_Cmd(USART1,ENABLE);																					// ʹ�ܴ���1
	DMA_Cmd(DMA1_Channel5,ENABLE);																		// ʹ��DMA
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);										  // ʹ�ܴ���DMA����
}


/**
 ***************************************************************************************
 * @brief       ������1�жϺ���
 * @param       ����
 * @return      ����
 * @exception   ����
 * @author      ��Owen Lee
 * @date        ��2024.12.24
 * @version     ��V1.00
 * @property    ��������ʵ�ִ���1���ж���Ӧ����
 ***************************************************************************************
**/
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_IDLE))								//���������ж�
	{
																														//��������жϣ�����������һ���ȶ�USART_FLAG_IDLE��־λ���ڶ����ٻ�ȡ��������
		USART_GetFlagStatus(USART1,USART_FLAG_IDLE);						//��������ж�--�ȶ�USART_FLAG_IDLE��־λ			
		USART_ReceiveData(USART1);															//��������ж�--�ٻ�ȡ��������
																														//���㱾�δ����������
																														//DMA���δ�����ࣨUSART1_RXMax + 1�������ݣ��������100�����ݾͲ���ֹͣ�ˣ�
																														//��ôDMA��NDTR�Ĵ���ʣ��ֵ���� ��USART1_RXMax + 1 - 100
																														//��(USART1_RXMax + 1) - (DMA->NDTR)���ɵõ����δ���������� 
		USART1_RXCtrlBlockStru.USART_RX_Counter += ((USART1_RXMax + 1) - DMA_GetCurrDataCounter(DMA1_Channel5));		
																														//���endָ��λ��
		USART1_RXCtrlBlockStru.USART1_RX_IN->end_ptr = &USART1_RXBuf[USART1_RXCtrlBlockStru.USART_RX_Counter - 1];																									
		USART1_RXCtrlBlockStru.USART1_RX_IN ++;									//INָ��ָ����һ��λ��
																														//�ж�INָ���Ƿ�ָ��ENDָ��,���ָ����ENDָ�룬��INָ������ָ��USART1_RXBufSE[0]
		if(USART1_RXCtrlBlockStru.USART1_RX_IN == USART1_RXCtrlBlockStru.USART1_RX_END)
		{
			USART1_RXCtrlBlockStru.USART1_RX_IN =  &(USART1_RXCtrlBlockStru.USART1_RXBufSE[0]);
		}
																														//�ж�USART1_RXBufʣ��ֵ�Ƿ��ܹ�����һ�ε��ν������������
		if(USART1_RXBufSize - USART1_RXCtrlBlockStru.USART_RX_Counter >= USART1_RXMax)
		{																												//���ʣ��ֵ�ܹ��ٽ���һ�Σ�����ʣ��ֵλ�ü�������
			USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr = &USART1_RXBuf[USART1_RXCtrlBlockStru.USART_RX_Counter];	
		}
		else
		{																												//���ʣ��ֵ�����ٽ���һ�Σ������´�0λ�ÿ�ʼ����																		
			USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr = USART1_RXBuf;
			USART1_RXCtrlBlockStru.USART_RX_Counter = 0;					//���������ۼ�ֵ����
		}
		DMA_Cmd(DMA1_Channel5,DISABLE);													//��������DMA����ʧ��
		while(((DMA1_Channel5->CCR) & DMA_CCR1_EN) == 1);				//ȷ��DMA���Ա����� 
//		DMA_ClearFlag(DMA1_FLAG_TC5);														//�����������жϱ�־λ������DMAû��ʹ���жϣ�����Ҫ���¿���DMA���������DMA���жϡ��μ�DMA�������ֲ᡿[F1ϵ��û�ҵ����˵����Ӧ�ò�������]

		DMA1_Channel5->CNDTR = USART1_RXMax + 1;									//�ٴ�����DMA����������
																														//����DMA�����ڴ��ַ
		DMA1_Channel5->CMAR = (uint32_t)(USART1_RXCtrlBlockStru.USART1_RX_IN->start_ptr);
		DMA_Cmd(DMA1_Channel5,ENABLE);														//ʹ��DMA
	}
}

/**
 ***************************************************************************************
 * @brief       ������1�������ݿ��ƽṹ���ʼ��
 * @param       ����
 * @return      ����
 * @exception   ����
 * @author      ��Owen Lee
 * @date        ��2024.12.24
 * @version     ��V1.00
 * @property    ��������ʵ�ִ���1�������ݿ��ƽṹ��ĳ�ʼ��
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
 * @brief       �����ڴ�ӡ����
 * @param       ��
									@param1 : uint8_t *format		������ʽ
									@param2 : ...								��ȷ�������ͼ�����
 * @return      ����
 * @exception   ����
 * @author      ��Owen Lee
 * @date        ��2024.12.24
 * @version     ��V1.00
 * @property    ��������ʵ�ִ��ڵ�printf����
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
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == 0);							//�ȴ����ͼĴ���Ϊ��
		USART_SendData(USART1,USART1_TXBuf[i]);															//��������
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == 0);								//�ȴ����һ�����ݷ������
}




/*********************************************END OF FILE**********************/

