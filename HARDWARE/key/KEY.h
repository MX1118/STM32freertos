#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "sys.h"

#define KEY_GPIO_PORT   GPIOF
#define KEY_RCC         RCC_AHB1Periph_GPIOF

// PF0-PF3
#define H1  GPIO_Pin_0
#define H2  GPIO_Pin_1
#define H3  GPIO_Pin_2
#define H4  GPIO_Pin_3

// PF4-PF7
#define L1  GPIO_Pin_4
#define L2  GPIO_Pin_5
#define L3  GPIO_Pin_6
#define L4  GPIO_Pin_7

void KEY_Init(void);
u8 KEY_Scan(void);
#endif


