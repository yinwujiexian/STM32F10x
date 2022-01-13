#include "main.h"
#include "timer.h"
#include "gui.h"
#include "tft.h"
#include "math.h"

double wave_fre_div = 0;
int wave_timer = 0;	  //定时器计数设定值
float wave_start = 0; //DAC输出的起始部分

/*******************
//Fout= Fclk/[2*(arr+1)*(psc+1)]
//Fout: TIM3的三角波频率(单位为Hz）
//Fclk: TIM3的输入时钟频率--72Mhz
//arr:设置自动重装载寄存器周期的值
//psc:设置时钟频率除数的预分值
通用定时器3初始化
********************/
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

/*******************
生成sin数据表--360个数据
角度0->359度
sin值-1-> +1
********************/
void sin_data_init(float *sindata)
{
	int i;
	for (i = 0; i < 360; i++)
	{
		sindata[i] = sin(i * PI / 180);
		sindata[i] = sindata[i] * wave_start + 16384;
	}
}

/*******************
TFT_LCD显示内容初始化，默认输出的波形初始化
********************/
void wave_init(void)
{
	TFT_CS(0);		  //打开LCD片选使能
	GUI_Clear(White); //清屏
	GUI_sprintf_hzstr16x(10, 0, "Wave_Frequency S3+ S4-", Red, White);
	GUI_sprintf_hzstr16x(10, 20, "Wave_or_Sin:", Red, White);
	GUI_sprintf_hzstr16x(10, 40, "Wave_Voltage:", Red, White);
	GUI_sprintf_hzstr16x(150, 40, "V", Red, White);
	GUI_sprintf_hzstr16x(10, 60, "Wave_Frequency:", Red, White);
	GUI_sprintf_hzstr16x(150, 60, "Hz", Red, White);

	GUI_sprintf_hzstr16x(10, 80, "PC.0----SYNC", Red, White);
	GUI_sprintf_hzstr16x(10, 100, "PC.2----SCLK", Red, White);
	GUI_sprintf_hzstr16x(10, 120, "PC.4----DIN", Red, White);

	//默认输出三角波：±2.5V，1Hz
	wave_timer = wave_voltage * 65536 / 5;
	wave_fre_div = wave_timer * wave_frequency;
	wave_fre_div = (36000000 / wave_fre_div) - 1;
	wave_start = (5 - wave_voltage) * 65536 / 10;
	TIM3_init(wave_timer, (int)wave_fre_div); //arr:设置自动重装载寄存器周期的值 //psc:设置时钟频率除数的预分值

	GUI_sprintf_hzstr16x(115, 40, "    ", Red, White);
	GUI_number(115, 40, wave_voltage, Red, White);
	GUI_sprintf_hzstr16x(125, 40, ".", Red, White);
	GUI_number(135, 40, wave_voltage * 10 - (int)wave_voltage * 10, Red, White); //显示电压
	GUI_sprintf_hzstr16x(130, 60, "  ", Red, White);
	GUI_number(130, 60, wave_frequency, Red, White); //显示频率

	if (tri_or_sin) 
	{
		GUI_sprintf_hzstr16x(110, 20, "   ", Red, White);
		GUI_sprintf_hzstr16x(110, 20, "Sin", Red, White);
	}
	else 
	{
		GUI_sprintf_hzstr16x(110, 20, "   ", Red, White);
		GUI_sprintf_hzstr16x(110, 20, "Tri", Red, White);
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
