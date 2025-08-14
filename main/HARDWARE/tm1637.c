/*                        -- �ɺ����ϵ��ӹ����� --                        */
/*
*   ˵    ��: TM1637DEMO
*   ����ƽ̨:     
*   �Ա�����: 
*
//  �ļ�����MAIN.C                                                             
//  ˵�������ͻ�����ģ��ʹ�ó���                                                                  
//  ��д��Ա��Duhemayi                                                                   
//  ��д���ڣ�2019-6-19                                                             
//  ����ά����
//  ά����¼��
//	��    ��: V1.0
//                                                          
// �����������ó��������ѧϰ�뽻�� 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/

#include "tm1637.h"
#include "delay.h"

unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//������
unsigned char table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};//������

void TM1637_DIO_SetInput(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				 //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}

void TM1637_DIO_SetOutput(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				 //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}

///======================================
void I2CStart(void) //1637 ��ʼ
{
	TM1637_DIO_SetOutput();
	
	TM1637_CLK_H;//clk = 1;
	TM1637_DIO_H;//dio = 1;
	delay_us(2);
	TM1637_DIO_L;//dio = 0;
}

///=============================================
void I2Cask(void) //1637 Ӧ��
{
	TM1637_CLK_L;//clk = 0;
	delay_us(5); //�ڵڰ˸�ʱ���½���֮����ʱ5us����ʼ�ж�ACK �ź�
	TM1637_DIO_SetInput();////while(dio);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
	TM1637_CLK_H;//clk = 1;
	delay_us(2);
	TM1637_CLK_L;//clk=0;
	
	TM1637_DIO_SetOutput();
}

///========================================
void I2CStop(void) // 1637 ֹͣ
{
	TM1637_DIO_SetOutput();
	
	TM1637_CLK_L;//clk = 0;
	delay_us(2);
	TM1637_DIO_L;//dio = 0;
	delay_us(2);
	TM1637_CLK_H;//clk = 1;
	delay_us(2);
	TM1637_DIO_H;//dio = 1;
}

///=========================================
void I2CWrByte(unsigned char oneByte) //дһ���ֽ�
{
	unsigned char i;
	
	TM1637_DIO_SetOutput();

	for(i=0;i<8;i++)
	{ 	
		TM1637_CLK_L;//clk = 0;
		if(oneByte&0x01) //��λ��ǰ
		{
			TM1637_DIO_H;//dio = 1;
		}
		else
		{
			TM1637_DIO_L;//dio = 0;
		}
		delay_us(3);
		oneByte=oneByte>>1;
		TM1637_CLK_H;//clk=1;
		delay_us(3);
	}
}

///-------------------------------------------------
unsigned char ScanKey(void) //������
{
	unsigned char rekey,rkey,i;
	I2CStart();
	I2CWrByte(0x42); //����������
	I2Cask();		 //��ȡ�ź�
	TM1637_DIO_H;//dio=1; // �ڶ�����ǰ����������	
	TM1637_DIO_SetInput();////while(dio);
	for(i=0;i<8;i++) //�ӵ�λ��ʼ��
	{ 
		TM1637_CLK_L;//clk=0;
		rekey=rekey>>1;
		delay_us(30);
		TM1637_CLK_H;//clk=1;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
		{
			rekey=rekey|0x80;
		}
		else
		{
			rekey=rekey|0x00;
		}
		delay_us(30);
	}
	I2Cask();
	I2CStop();
	
	return (rekey);
}

///================================================
void SmgDisplay(void) //д��ʾ�Ĵ���
{
	unsigned char i;
	
	I2CStart();
	I2CWrByte(0x40); // 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1 ģʽ
	I2Cask();
	I2CStop();
	I2CStart();
	I2CWrByte(0xc0); //�����׵�ַ��
	I2Cask();
	for(i=0;i<6;i++) //��ַ�Լӣ�����ÿ�ζ�д��ַ
	{
//		I2CWrByte(0x06); //������
		I2CWrByte(table[i]); //������
//		I2CWrByte(0X00); //������
		I2Cask();
	}
	I2CStop();
	I2CStart();
	I2CWrByte(0x8A); //����ʾ ��������� 8�����ȿɵ�
	I2Cask();
	I2CStop();
}

void TM1637_SHOW(unsigned long dat)
{
	unsigned char a,b,c,d,e,f;
	unsigned long tem;
	
	tem = dat/10;
	
	a = tem%1000000/100000;
	b = tem%100000/10000;
	c = tem%10000/1000;
	d = tem%1000/100;
	e = tem%100/10;
	f = tem%10;
	
	I2CStart();
	I2CWrByte(0x40); // 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1 ģʽ
	I2Cask();
	I2CStop();
	I2CStart();
	I2CWrByte(0xc0); //�����׵�ַ��
	I2Cask();
	I2CWrByte(table1[a]); //������
	I2Cask();
	I2CWrByte(table[b]); //������
	I2Cask();
	I2CWrByte(table[c]); //������
	I2Cask();
	I2CWrByte(table[d]); //������
	I2Cask();
	I2CWrByte(table[e]); //������
	I2Cask();
	I2CWrByte(table[f]); //������
	I2Cask();
	
	I2CStop();
	I2CStart();
	I2CWrByte(0x8A); //����ʾ ��������� 8�����ȿɵ�
	I2Cask();
	I2CStop();
}
///==============================================
//void init() //��ʼ���ӳ���
//{
////��ʼ����
//}

/////==============================================
//void main(void)
//{
//	unsigned char keydate;
//
//	init(); //��ʼ��
//	SmgDisplay(); //д�Ĵ���������ʾ
//	while(1)
//	{
//		keydate=Scankey(); //������ֵ �������İ���ֵ��������
//	}
//}
//===========end==================================

