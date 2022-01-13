#ifndef __TIMER_H
#define __TIMER_H

#include <sys.h>

void TIM3_init(u16 arr, u16 psc);
void wave_init(void);

extern double wave_fre_div;
extern int wave_timer;   //定时器计数设定值
extern float wave_start; //DAC输出的起始部分

#endif
