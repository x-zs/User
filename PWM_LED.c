#include "PWM_LED.h"
void TIM3_Config(uint8_t pulse)
{
TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_TimBaseStructure.TIM_Period=99;
TIM_TimBaseStructure.TIM_Prescaler=72;
TIM_TimBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimBaseStructure);
   TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	if(pulse==1)pulse=90;
	TIM_OCInitStructure.TIM_Pulse=pulse;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}
void PWM_GPIO_Config()
{ GPIO_InitTypeDef GPIO_InitStructure;//函数声明要放在程序的前面
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void TIM3_PWM_Config(uint8_t pulse)
{
	PWM_GPIO_Config();//gaigai
	TIM3_Config(pulse);
}