#include "main.h"
#include <timer.h>
#include <LED.h>
#include <sys.h>

/*******************
//Fout= Tclk/[2*(arr+1)*(psc+1)]
//Fout: TIM3的三角波频率(单位为Hz）
//Tclk: TIM3的输入时钟频率(单位为144Mhz)
//arr:设置自动重装载寄存器周期的值
//psc:设置时钟频率除数的预分值
通用定时器3初始化
******************/
void TIM3_init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStr;
	NVIC_InitTypeDef NVIC_InitStr;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//初始化定时器TIM
	TIM_TimeBaseStr.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStr.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
	TIM_TimeBaseStr.TIM_Period = arr;
	TIM_TimeBaseStr.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStr);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitStr.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStr.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStr);

	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
