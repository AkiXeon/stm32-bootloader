#include "i2c.h"
#include "stm32f10x.h"
//this is i2c file 

// git  test


void i2c_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIOS;
    GPIOS.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIOS.GPIO_Pin = Pin_SDA | Pin_SCL;
    GPIOS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIOS);

    /*  初始化后进入空闲状态 此处为避免SCL高电平时SDA产生波动，先释放SDA线   */
    SDA_H
    SCL_H
}

/*  start波形   */
void i2c_start(){
    SCL_H
    SDA_H
    delay_us(2);    
    SDA_L
    delay_us(2); 
    SCL_H       //拉低SCL，正式开始通信，等待SDA
}
/*  stop波形    */
void i2c_stop(){
    SDA_L
    SCL_H
    delay_us(2); 
    SDA_H
    SCL_L
    
}

/*  iic发送一字节数据   */
void i2c_sendbyte(uint8_t byte){
    for(uint8_t i = 0;i < 8;i++){
        SCL_L
        if(byte & (0x80>>i)){
            SDA_H
        }else{
            SDA_L
        }
        delay_us(2);
        SCL_H
        delay_us(2);
    }

    SCL_L
    SDA_H
    delay_us(1);    //隐含超时判断
    SCL_H
    // if (GPIO_ReadInputDataBit(GPIOB,Pin_SDA) == 0)
    SCL_L   //结束ack
}

uint8_t i2c_waitack(uint8_t timeout){
    do{ 
        timeout--;
        delay_us(1);
    }while((GPIO_ReadInputDataBit(GPIOB,Pin_SDA) == 0)&&(timeout > 0));
    if(timeout == 0)    return 1;
    SCL_H
    delay_us(2);
    if(GPIO_ReadInputDataBit(GPIOB,Pin_SDA) == 1)  return 2; //说明不稳定
    SCL_L
    return 0;
}

uint8_t i2c_receivebyte(uint8_t ack){
    uint8_t byte = 0x00;
    for(uint8_t i = 0;i < 8;i++){
        SCL_L
        delay_us(2);
        SCL_H
        if(GPIO_ReadInputDataBit(GPIOB,Pin_SDA) == 1){
            byte |= (0x80>>i);
        }
    }
    SCL_L
    delay_us(2);
    if(ack){
        SDA_L
        SCL_H
        delay_us(2);
        SCL_L   /* 释放总线 */
        SDA_H
        delay_us(2);
    }{
        SDA_H
        SCL_H
        delay_us(2);
        SCL_L
    }
    return byte;


}

