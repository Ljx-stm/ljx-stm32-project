#include "stm32f10x.h"                  // Device header

uint8_t Timer_Flag = 0;


/************************************************************************
* 函数名称：void TIM2_Int_Init(u16 arr,u16 psc)
* 功能说明：定时器2 中断初始化
* 入口参数：arr：自动重装载值   psc：预分频系数
* 返回值：无
************************************************************************/
void TIM2_Init(uint16_t arr, uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}
	
// 定时器2中断函数
void TIM2_IRQHandler(void)
{
	static uint16_t Cnt = 0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		Cnt++;
		if(Cnt >= 10)
		{
			Cnt = 0;
			Timer_Flag = 1;
		}
	}
}




/************************************************************************
* 函数名称：void TIM3_Int_Init(u16 arr,u16 psc)
* 功能说明：定时器3 中断初始化 → 用于机智云心跳+蜂鸣器控制
************************************************************************/
void TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=9;
	TIM_TimeBaseStructure.TIM_Prescaler=71;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}


/************************************************************************
* 函数名称：void TIM3_IRQHandler(void) 
* 功能说明：定时器3 中断服务函数
* 核心作用：1. 机智云毫秒级心跳  2. 蜂鸣器间歇报警
************************************************************************/
void TIM3_IRQHandler(void)
{
	static uint16_t Cnt = 0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	 Cnt ++;
	
	
	}
}
