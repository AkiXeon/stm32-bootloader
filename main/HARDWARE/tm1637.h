#ifndef __TM1637_H
#define __TM1637_H

#include "sys.h"

unsigned char ScanKey(void); //������
void SmgDisplay(void); //д��ʾ�Ĵ���

#define TM1637_CLK_H GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define TM1637_CLK_L GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define TM1637_DIO_H GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define TM1637_DIO_L GPIO_ResetBits(GPIOB,GPIO_Pin_14)

void SmgDisplay(void); //д��ʾ�Ĵ���

void TM1637_SHOW(unsigned long dat);

#endif


