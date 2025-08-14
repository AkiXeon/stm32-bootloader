#include "stm32f10x.h"  // Device head
#include "Delay.h"

void MYSPI_W_CS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);    
}

void MYSPI_Init(void)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
}

void MYSPI_START(void)
{
    delay_us(5); 
    MYSPI_W_CS(0);
    delay_us(5);
}

void MYSPI_END(void)
{
    delay_us(5);
    MYSPI_W_CS(1);
    delay_us(5);
}

void SPI_W_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

uint8_t SPI_R_MISO(void)
{
   return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
}

void MYSPI_CLK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)(BitValue));
}

uint8_t SPI_SWAP(uint8_t DATA)
{
    uint8_t i;
    uint8_t VALUE;
     MYSPI_START();
    for(i=0;i<8;i++)
    {
       SPI_W_MOSI(DATA&0X80>>i);
 MYSPI_CLK(1);
    if(SPI_R_MISO() ==1)
    {
        VALUE |= (0X80>>i);
    }
    MYSPI_CLK(0);
    }
    return VALUE;
}


uint8_t SPI1_SWAP(uint8_t Data)
{
    uint8_t i;
    uint8_t DATA;
    for(i=0;i<8;i++)
    {
      SPI_W_MOSI(Data&(0x80>>i));
        Data<<=1;//向左进行移位操作
      MYSPI_CLK(1);
        if(SPI_R_MISO()==1)
        {
            DATA=(Data |=0x01<<i);//进行左移赋值操作
        }
        MYSPI_CLK(0);
        
    }
    return DATA;
  
}

//uint8_t MySPI_SWAP(uint16_t value)

//{
//     
//}



/*
uint8_t MYSPI_SWAP(uint16_t SendData)
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
    SPI_I2S_SendData(SPI1,SendData);

    SPI1->SR&=(1<<0);

    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)==SET);
    //while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
   //return SPI_I2S_ReceiveData(SPI1);
}
*/









/*
void spi_w_sck(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)BitValue);
}

void spi_w_mosi(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void spi_w_cs(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)BitValue);
}


void my_spi_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
   GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6);	
}

void SPI_START(void)
{
    delay_us(8);
    spi_w_cs(0);
    delay_us(8);
}

void SPI_END(void)
{
     delay_us(8);
    spi_w_cs(1);
    delay_us(8);
}

void spi_swap(uint8_t SendByte)
{
    
    uint8_t i;
    for(i=0;i<8;i++)
    {
    
    spi_w_mosi(SendByte & (0x80>>i));
    spi_w_sck(1);
  delay_us(20);
   spi_w_sck(0);
    }
    
}
*/
