#ifndef __SPI_H
#define __SPI_H


void MYSPI_W_CS(uint8_t BitValue);
void MYSPI_Init(void);
void MYSPI_START(void);
uint8_t SPI_SWAP(uint8_t DATA);
void SPI_W_MOSI(uint8_t BitValue);
uint8_t SPI_R_MISO(void);
void MYSPI_END(void);
uint8_t SPI1_SWAP(uint8_t Data);


/*
void spi_w_sck(uint8_t BitValue);
void spi_w_mosi(uint8_t BitValue);
void spi_w_cs(uint8_t BitValue);
void my_spi_Init(void);
uint8_t spi_r_miso(void);
void SPI_START(void);
void SPI_END(void);
uint8_t spi_swap(uint8_t SendByte);
//uint8_t SPI_SWAP(uint8_t DataBits);
uint8_t MY_SPI_16SWAP(uint16_t SendData);
*/
#endif
