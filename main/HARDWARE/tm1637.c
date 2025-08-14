/*                        -- 渡河蚂蚁电子工作室 --                        */
/*
*   说    明: TM1637DEMO
*   开发平台:     
*   淘宝网店: 
*
//  文件名：MAIN.C                                                             
//  说明：供客户测试模块使用程序                                                                  
//  编写人员：Duhemayi                                                                   
//  编写日期：2019-6-19                                                             
//  程序维护：
//  维护记录：
//	版    本: V1.0
//                                                          
// 免责声明：该程序仅用于学习与交流 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/

#include "tm1637.h"
#include "delay.h"

unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//共阴极
unsigned char table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};//共阴极

void TM1637_DIO_SetInput(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				 //PB.5 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}

void TM1637_DIO_SetOutput(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				 //PB.5 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}

///======================================
void I2CStart(void) //1637 开始
{
	TM1637_DIO_SetOutput();
	
	TM1637_CLK_H;//clk = 1;
	TM1637_DIO_H;//dio = 1;
	delay_us(2);
	TM1637_DIO_L;//dio = 0;
}

///=============================================
void I2Cask(void) //1637 应答
{
	TM1637_CLK_L;//clk = 0;
	delay_us(5); //在第八个时钟下降沿之后延时5us，开始判断ACK 信号
	TM1637_DIO_SetInput();////while(dio);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
	TM1637_CLK_H;//clk = 1;
	delay_us(2);
	TM1637_CLK_L;//clk=0;
	
	TM1637_DIO_SetOutput();
}

///========================================
void I2CStop(void) // 1637 停止
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
void I2CWrByte(unsigned char oneByte) //写一个字节
{
	unsigned char i;
	
	TM1637_DIO_SetOutput();

	for(i=0;i<8;i++)
	{ 	
		TM1637_CLK_L;//clk = 0;
		if(oneByte&0x01) //低位在前
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
unsigned char ScanKey(void) //读按键
{
	unsigned char rekey,rkey,i;
	I2CStart();
	I2CWrByte(0x42); //读按键命令
	I2Cask();		 //读取信号
	TM1637_DIO_H;//dio=1; // 在读按键前拉高数据线	
	TM1637_DIO_SetInput();////while(dio);
	for(i=0;i<8;i++) //从低位开始读
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
void SmgDisplay(void) //写显示寄存器
{
	unsigned char i;
	
	I2CStart();
	I2CWrByte(0x40); // 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1 模式
	I2Cask();
	I2CStop();
	I2CStart();
	I2CWrByte(0xc0); //设置首地址，
	I2Cask();
	for(i=0;i<6;i++) //地址自加，不必每次都写地址
	{
//		I2CWrByte(0x06); //送数据
		I2CWrByte(table[i]); //送数据
//		I2CWrByte(0X00); //送数据
		I2Cask();
	}
	I2CStop();
	I2CStart();
	I2CWrByte(0x8A); //开显示 ，最大亮度 8级亮度可调
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
	I2CWrByte(0x40); // 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1 模式
	I2Cask();
	I2CStop();
	I2CStart();
	I2CWrByte(0xc0); //设置首地址，
	I2Cask();
	I2CWrByte(table1[a]); //送数据
	I2Cask();
	I2CWrByte(table[b]); //送数据
	I2Cask();
	I2CWrByte(table[c]); //送数据
	I2Cask();
	I2CWrByte(table[d]); //送数据
	I2Cask();
	I2CWrByte(table[e]); //送数据
	I2Cask();
	I2CWrByte(table[f]); //送数据
	I2Cask();
	
	I2CStop();
	I2CStart();
	I2CWrByte(0x8A); //开显示 ，最大亮度 8级亮度可调
	I2Cask();
	I2CStop();
}
///==============================================
//void init() //初始化子程序
//{
////初始化略
//}

/////==============================================
//void main(void)
//{
//	unsigned char keydate;
//
//	init(); //初始化
//	SmgDisplay(); //写寄存器并开显示
//	while(1)
//	{
//		keydate=Scankey(); //读按键值 ，读出的按键值不作处理。
//	}
//}
//===========end==================================

