#ifndef __MAIN_H
#define __MAIN_H

//necessary head file
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include "delay.h"

//Hardware head file


//__NOP();  //单周期指令 1/72M=0.01389us=13.9ns≈14ns
#define _nop_() asm("nop")


#endif

