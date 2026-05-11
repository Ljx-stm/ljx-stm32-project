#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

// 外部声明全局变量（让main.c可以调用）
extern uint16_t Tempup;
extern uint16_t Humiup;
extern uint16_t Airup;
extern uint16_t PM25up;

// 函数声明
void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
void KEY_Set(void);
void OLED_DisplayInit(void);

#endif
