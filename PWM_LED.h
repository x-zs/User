#ifndef PWM_LED_H
#define PWM_LED_H
#include "stm32f10x.h"
void TIM3_Config(uint8_t pulse);
void PWM_GPIO_Config(void);
void TIM3_PWM_Config(uint8_t pulse);
#endif
