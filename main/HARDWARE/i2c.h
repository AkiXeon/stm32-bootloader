#ifndef __I2C_H
#define __I2C_H
#include "delay.h"
#define Pin_SCL GPIO_Pin_6
#define Pin_SDA GPIO_Pin_7
#define SCL_H GPIO_SetBits(GPIOB,Pin_SCL);
#define SCL_L GPIO_ResetBits(GPIOB,Pin_SCL);
#define SDA_H GPIO_SetBits(GPIOB,Pin_SDA);
#define SDA_L GPIO_ResetBits(GPIOB,Pin_SDA);

void i2c_start(void);
void i2c_stop(void);
#endif
