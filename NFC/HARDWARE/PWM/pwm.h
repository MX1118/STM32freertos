#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx.h"

void TIM4_PWM_Init(u16 arr,u16 psc);
void Servo_SetAngle(u8 angle);
void Servo_Open(void);
void Servo_Close(void);

#endif
