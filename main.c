#include "stm32f10x.h"
#include "Delay.h"
#include "OLED_I2C.h"
#include "LED.h"
#include "KEY.h"
#include "adc.h"
#include "PM25.h"
#include "dht11.h"
#include <stdio.h>

extern uint16_t Tempup;
extern uint16_t Humiup;
extern uint16_t Airup;
extern uint16_t PM25up;

int main(void)
{
	uint8_t temperature = 25;
	uint8_t humidity = 50;
	uint16_t adc_val;
	uint16_t air_quality;
	uint16_t pm25_val;
	char buf[24];
	uint8_t cnt = 0;
	
	Delay_Init();
	LED_Init();
	OLED_Init();
	KEY_Init();
	Adc_Init();
	DHT11_Init();
	
	OLED_ShowStr(0, 0, "SYSTEM READY", 2);
	OLED_ShowStr(0, 2, "KEY0 SET PARAM", 2);
	Delay_ms(1500);
	OLED_CLS();
	OLED_DisplayInit();
	
	while(1)
	{
		if(KEY_Scan(0) == 1)
		{
			KEY_Set();
		}
		
		cnt++;
		if(cnt >= 10)
		{
			cnt = 0;
			DHT11_Read_Data(&temperature, &humidity);
		}

		adc_val = Get_Adc_Average(ADC_Channel_0, 10);
		air_quality = adc_val / 65;
		if(air_quality < 10)  air_quality = 10;
		if(air_quality > 98)  air_quality = 98;

		pm25_val = PM25_GetDATA();

		sprintf(buf, "%dC", temperature);
		OLED_ShowStr(32, 0, (uint8_t*)buf, 2);
		
		sprintf(buf, "%d%%", humidity);
		OLED_ShowStr(32, 2, (uint8_t*)buf, 2);
		
		sprintf(buf, "%d%%", air_quality);
		OLED_ShowStr(64, 4, (uint8_t*)buf, 2);
		
		sprintf(buf, "PM2.5:%d ug/m3", pm25_val);
		OLED_ShowStr(0, 6, (uint8_t*)buf, 2);

		if(temperature > Tempup || humidity > Humiup || air_quality > Airup || pm25_val > PM25up)
		{
			Led0_Set(GPIO_ON);
			Beep_Set(GPIO_ON);
			Jd0_Set(GPIO_ON);
		}
		else
		{
			Led0_Set(GPIO_OFF);
			Beep_Set(GPIO_OFF);
			Jd0_Set(GPIO_OFF);
		}
		
		Delay_ms(100);
	}
}
