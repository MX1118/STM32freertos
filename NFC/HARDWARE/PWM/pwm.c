#include "pwm.h"

void TIM4_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 1500;

    TIM_OC1Init(TIM4,&TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
}
void Servo_SetAngle(u8 angle)
{
    u16 pulse;
    
	  if(angle > 180) angle = 180;
	
    pulse = 500 + angle * 2000 / 180;

    TIM_SetCompare1(TIM4,pulse);
}

void Servo_Open(void)
{
    Servo_SetAngle(90);
}

void Servo_Close(void)
{
    Servo_SetAngle(0);
}
