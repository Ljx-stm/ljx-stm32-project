#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "stdint.h"

#define DHT11_PIN    GPIO_Pin_15
#define DHT11_PORT   GPIOB
#define DHT11_RCC    RCC_APB2Periph_GPIOB

void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);
uint8_t DHT11_Init(void);

#endif
