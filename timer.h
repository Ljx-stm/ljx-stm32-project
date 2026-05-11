#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x.h"                  // Device header

extern uint8_t Timer_Flag;

void TIM2_Init(uint16_t arr, uint16_t psc);
void TIM3_Init(uint16_t arr, uint16_t psc);


#endif
