#include "dac8552.h"
#include "delay.h"

//-----------------------------------------------------------------
// 函数功能: DAC8552引脚初始化
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
//-----------------------------------------------------------------
void DAC8552_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 开启GPIOC时钟

	// SYNC->PC0  SCLK->PC2  DIN->PC4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 设置为上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 时钟50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  // 初始化

	DAC8552_SYNC_H;
	DAC8552_Write_Reg(0x000000 | 32768);
	DAC8552_Write_Reg(0x340000 | 32768);
}

//-----------------------------------------------------------------
// 函数功能: DAC8552写数据
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
//-----------------------------------------------------------------
void DAC8552_Write_Reg(u32 data)
{
	u8 i;

	DAC8552_SYNC_L;
	for (i = 0; i < 24; i++)
	{
		DAC8552_SCLK_H;
		if (data & 0x800000)
			DAC8552_DIN_H;
		else
			DAC8552_DIN_L;
		DAC8552_SCLK_L;
		data <<= 1;
	}
	DAC8552_SYNC_H;
}

//-----------------------------------------------------------------
// 函数功能: DAC8552通道A数据更新
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
//-----------------------------------------------------------------
void DAC8552_Write_CHA(u16 data)
{
	// 功能					D23  D22  D21  D20  D19  D18  D17  D16  D15~D0
	DAC8552_Write_Reg(0x100000 | data);
}

//-----------------------------------------------------------------
// 函数功能: DAC8552通道B数据更新
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
//-----------------------------------------------------------------
void DAC8552_Write_CHB(u16 data)
{
	DAC8552_Write_Reg(0x240000 | data);
}
