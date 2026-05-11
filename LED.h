#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

typedef enum
{
    GPIO_OFF = 0,
    GPIO_ON = 1
} GPIO_ENUM;

typedef struct
{
    GPIO_ENUM Led0Sta;
    GPIO_ENUM Jd0Sta;
    GPIO_ENUM BeepSta;
} GPIO_STATUS;

// 4个函数全部声明
void LED_Init(void);
void Led0_Set(GPIO_ENUM status);
void Jd0_Set(GPIO_ENUM status);
void Beep_Set(GPIO_ENUM status);

#endif
