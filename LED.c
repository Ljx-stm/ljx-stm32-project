#include "stm32f10x.h"
#include "LED.h"

// 全局IO状态结构体
GPIO_STATUS gpioStatus;

/**
  * 功能：LED、蜂鸣器、继电器初始化
  * 引脚：PC13 = 板载LED  
  *       PA8  = 蜂鸣器  
  *       PB12 = 继电器（你使用的引脚）
  */
void LED_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	
	// PC13 LED初始化
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Initstructure);    
	GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 默认关闭
	
	// PA8 蜂鸣器初始化
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_Initstructure);    
	GPIO_SetBits(GPIOA, GPIO_Pin_8);  // 默认关闭
	
	// PB12 继电器初始化（你的硬件引脚）
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initstructure);    
	GPIO_SetBits(GPIOB, GPIO_Pin_12);  // 默认关闭
}

// 板载LED控制
void Led0_Set(GPIO_ENUM status)
{
	if(status == GPIO_ON)
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
	gpioStatus.Led0Sta = status;
}

// 继电器控制（PB12）
void Jd0_Set(GPIO_ENUM status)
{
	if(status == GPIO_ON)
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);  // 低电平吸合
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_12);    // 高电平断开
	
	gpioStatus.Jd0Sta = status;
}

// 蜂鸣器控制（PA8）
void Beep_Set(GPIO_ENUM status)
{
	if(status == GPIO_ON)
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	else
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
	gpioStatus.BeepSta = status;
}
