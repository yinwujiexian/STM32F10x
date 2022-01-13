#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 200 //定义最大接收字节数 200
#define EN_USART1_RX 1    //使能（1）/禁止（0）串口1接收

extern char USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                 //接收状态标记

extern int tri_or_sin;     //0：三角波；1：正弦波
extern float wave_voltage; //正负电压V，默认输出正负2.5V，DAC最高输出正负5V
extern int wave_frequency; //默认输出频率为1Hz，最高980Hz
extern int change_flag;

//如果想 串口中断 接收，请不要注释以下宏定义
void uart_init(u32 bound);
#endif
