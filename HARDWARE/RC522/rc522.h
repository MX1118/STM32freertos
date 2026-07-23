#ifndef _RC522_H_
#define _RC522_H_
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

#define RC522_CS_L()  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define RC522_CS_H() GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define RC522_RST_L() GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define RC522_RST_H() GPIO_SetBits(GPIOA,GPIO_Pin_1)

void RC522_Init(void);

void Action_Success(void);
void Action_Error(void);
uint8_t RC522_CheckCard(uint8_t *uid);
uint8_t RC522_ReadUID(uint8_t *uid);
#define MI_OK       0
#define MI_ERR      1


#define PICC_REQIDL 0x26
#define PICC_REQALL 0x52
#endif

