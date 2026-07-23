#include "SPI.h"

void SPI1_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
		SPI1_ReadWrite(0xff);
}
/**
 * @brief  SPI1读写一个字节（全双工模式）
 * @param  TxData: 要发送的数据字节
 * @retval 返回接收到的数据字节
 * @note   SPI是全双工通信，发送的同时会接收数据
 *         此函数实现了完整的SPI单次字节传输
 */
u8 SPI1_ReadWrite(u8 TxData)
{          
  // 等待发送缓冲区为空（TXE标志置位）
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	
  // 通过SPI1发送一个字节数据
  SPI_I2S_SendData(SPI1, TxData); 
		
  // 等待接收缓冲区非空（RXNE标志置位）
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} 
 
  // 返回通过SPI1接收到的数据
  return SPI_I2S_ReceiveData(SPI1); 
          
}
/**
 * @brief  设置SPI1的通信速率
 * @param  SPI_BaudRatePrescaler: SPI波特率预分频系数
 *         可选值: SPI_BaudRatePrescaler_2/4/8/16/32/64/128/256
 * @retval 无
 * @note   SPI时钟频率 = APB2_CLK / 预分频系数
 *         APB2总线时钟通常为84MHz(STM32F4系列)
 */
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));  // 检查参数合法性
  SPI1->CR1 &= 0XFFC7;                                             // 清除CR1寄存器的位3-5(BR[2:0]位域)
  SPI1->CR1 |= SPI_BaudRatePrescaler;                               // 设置新的波特率预分频值
  SPI_Cmd(SPI1, ENABLE);                                           // 确保SPI1处于使能状态
}