#include "stm32f10x.h"
#include "Delay.h"
#include "OLED_I2C.h"
#include "stdio.h"

// 按键宏定义：PA2 / PA5 / PB0
#define KEY0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

// 4个报警上限（全局变量 → 唯一在这里定义）
uint16_t  Tempup = 30;
uint16_t  Humiup = 80;
uint16_t  Airup  = 30;
uint16_t  PM25up = 200;

// 按键初始化
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 初始化 PA2、PA5（上拉输入）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始化 PB0（上拉输入）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 按键扫描函数
uint8_t KEY_Scan(uint8_t mode)
{ 
	static uint8_t Key_flag = 0;
	uint8_t Status = 0;		
	
	if(mode==1)	
		Key_flag=0; 
	
	if((KEY0==0||KEY1==0||KEY2==0)&&Key_flag==0)
	{
		Delay_ms(10);
		Key_flag=1; 
		
		if(KEY0==0) Status = 1;
		else if(KEY1==0) Status = 2;
		else if(KEY2==0) Status = 3;
	}
	else if (KEY0==1&&KEY1==1&&KEY2==1)
	{
		Key_flag = 0;
		Status = 0;
	}
	return Status;
}

// OLED初始化界面
void OLED_DisplayInit(void)
{
    OLED_ShowCN(0,0,0); 
    OLED_ShowCN(16,0,2);    //温度

    OLED_ShowCN(0,2,1); 
    OLED_ShowCN(16,2,2);    //湿度

    OLED_ShowCN(0,4,3); 
    OLED_ShowCN(16,4,4);    
    OLED_ShowCN(16*2,4,5); 
    OLED_ShowCN(16*3,4,6);    //空气质量
}

// 阈值设置函数
void KEY_Set(void)
{
	char buf[18];
	static uint8_t ic = 0;
	uint8_t key_value=0;
	uint8_t key_value1 = 0;
	
	OLED_CLS();
	OLED_DisplayInit();
	
	while(1)
	{
		key_value = KEY_Scan(0);
		key_value1 = KEY_Scan(1);
		
		// KEY0 切换参数
		if(key_value==1)
		{
			ic++;
			if(ic>4)ic=0;
		}
		// 设置温度
		if(ic == 0)
		{
			if(key_value1 == 2)
			{
				Tempup++;
				Tempup %= 100;
			}
			if(key_value1 == 3 && Tempup > 0)
			{
				Tempup--;
			}
			sprintf(buf, ">%02dC", Tempup);
			OLED_ShowStr(32,0,(uint8_t*)buf,2);
			sprintf(buf, ":%02d%%RH", Humiup);
			OLED_ShowStr(32,2,(uint8_t*)buf,2);	
			sprintf(buf, ":%02d%%RH", Airup);
			OLED_ShowStr(64,4,(uint8_t*)buf,2);
			sprintf(buf, "PM2.5:%4dug/m3", PM25up);
			OLED_ShowStr(0,6,(uint8_t*)buf,2);
		}
		// 设置湿度
		else if(ic == 1)
		{
			if(key_value1 == 2)
			{
				Humiup++;
				Humiup %=100;
			}
			if(key_value1 == 3 && Humiup>0)
			{
				Humiup--;
			}
			sprintf(buf, ":%02dC", Tempup);
			OLED_ShowStr(32,0,(uint8_t*)buf,2);
			sprintf(buf, ">%02d%%RH", Humiup);
			OLED_ShowStr(32,2,(uint8_t*)buf,2);	
			sprintf(buf, ":%02d%%RH", Airup);
			OLED_ShowStr(64,4,(uint8_t*)buf,2);
			sprintf(buf, "PM2.5:%4dug/m3", PM25up);
			OLED_ShowStr(0,6,(uint8_t*)buf,2);
		}
		// 设置空气质量
		else if(ic == 2)
		{
			if(key_value1==2)
			{
				Airup++;
				Airup %= 100;
			}
			if(key_value1 == 3 && Airup > 0)
			{
				Airup--;
			}
			sprintf(buf, ":%02dC", Tempup);
			OLED_ShowStr(32,0,(uint8_t*)buf,2);
			sprintf(buf, ":%02d%%RH", Humiup);
			OLED_ShowStr(32,2,(uint8_t*)buf,2);	
			sprintf(buf, ">%02d%%RH", Airup);
			OLED_ShowStr(64,4,(uint8_t*)buf,2);
			sprintf(buf, "PM2.5:%4dug/m3", PM25up);
			OLED_ShowStr(0,6,(uint8_t*)buf,2);		
		}
		// 设置PM2.5
		else if(ic == 3)
		{
			if(key_value1 == 2)
			{
				PM25up++;
				PM25up %= 1000;
			}
			if(key_value1 == 3 && PM25up > 0)
			{
				PM25up--;
			}
			sprintf(buf, ":%02dC", Tempup);
			OLED_ShowStr(32,0,(uint8_t*)buf,2);
			sprintf(buf, ":%02d%%RH", Humiup);
			OLED_ShowStr(32,2,(uint8_t*)buf,2);	
			sprintf(buf, ":%02d%%RH", Airup);
			OLED_ShowStr(64,4,(uint8_t*)buf,2);
			sprintf(buf, "PM2.5>%4dug/m3", PM25up);
			OLED_ShowStr(0,6,(uint8_t*)buf,2);
		}	
		// 退出设置
		else if(ic == 4)
		{
			ic = 0;
			OLED_CLS();
			OLED_DisplayInit();
			break;
		}		
	}	
}
