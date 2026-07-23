#include "stm32f10x.h"                  // Device header
#include "Variable.h"

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIOAʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);//TIM3�л�Ϊ�ڲ���ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
		//TIM3��ʱ�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//����Ƚ�ģʽ����PWM1
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//��ʼ��CCR��ֵΪ0

	TIM_OC1Init(TIM3,&TIM_OCInitStructure);//TIM3����ͨ��1����
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);//TIM3����ͨ��3����
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);//TIM3����ͨ��4����
	
	TIM_Cmd(TIM3,ENABLE);//ʹ��TIM3
}


void PWM_WSetCompare(uint16_t Compare)
{
	TIM_SetCompare1(TIM3, Compare);//����β��CCR1��ֵ
}


void WServo_Angle(float Angle)//β��
{
	PWM_WSetCompare(Angle / 180 * 2000 + 500);			
}

void PWM_LED1(uint16_t Compare)
{
	TIM_SetCompare3(TIM3,Compare);
}

void PWM_LED2(uint16_t Compare)
{
	TIM_SetCompare4(TIM3,Compare);
}

