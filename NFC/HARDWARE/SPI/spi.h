#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"
#include "sys.h"
void SPI1_Init(void);
uint8_t SPI1_ReadWrite(uint8_t tx_data);
void SPI1_SetSpeed(uint8_t SpeedSet); //设置SPI1速度   
uint8_t SPI1_ReadWriteByte(uint8_t TxData);//SPI1总线读写一个字节


#endif


