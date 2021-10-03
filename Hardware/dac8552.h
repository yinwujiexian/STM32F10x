#ifndef _DAC8552_H_
#define _DAC8552_H_
#include "stm32f10x.h"

#define DAC8552_SYNC_L      GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define DAC8552_SYNC_H     	GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define DAC8552_SCLK_L     	GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define DAC8552_SCLK_H     	GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define DAC8552_DIN_L      	GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define DAC8552_DIN_H     	GPIO_SetBits(GPIOC, GPIO_Pin_4)


extern void DAC8552_GPIO_Init(void);
extern void DAC8552_Write_Reg(u32 data);
extern void DAC8552_Write_CHA(u16 data);
extern void DAC8552_Write_CHB(u16 data);

#endif
