#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"
#include "Delay.h"

void Adc_Init(void);
uint16_t ADC_GetValue(uint8_t ch);
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);

#endif
